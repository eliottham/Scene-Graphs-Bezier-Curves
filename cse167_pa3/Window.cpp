#include "Window.h"

const char* window_title = "GLFW Starter Project";
GLint skyboxShader;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "./shader.vert"
#define FRAGMENT_SHADER_PATH "./shader.frag"
#define ROTATE_VELOCITY 50.0f
#define SKYBOX_SIZE 500.0f
// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 150.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
float Window::xpos;
float Window::ypos;
glm::vec3 Window::last_point;
bool Window::movement;
glm::mat4 Window::P;
glm::mat4 Window::V;
GLuint Window::robotShader;

Skybox* skybox;
Transform* squad;
Bezier* c0;
Bezier* c1;

vector<string> skybox_faces = {
    "./right.jpg",
    "./left.jpg",
    "./top.jpg",
    "./bottom.jpg",
    "./front.jpg",
    "./back.jpg"
};

// Function declaration for creating a robot
Transform* makeRobot();
// Function declaration for creating a robot squad
Transform* makeRobotSquad();

void Window::initialize_objects()
{
    skybox = new Skybox(skybox_faces);
    Window::robotShader = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    skyboxShader = LoadShaders("./skybox_shader.vert", "./skybox_shader.frag");
    glUseProgram(skyboxShader);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
    
    squad = new Transform(glm::translate(glm::mat4(1.0f),
                                         glm::vec3(-75.0f, 0.0f, -150.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
    squad->addChild(makeRobotSquad());
    
    c0 = new Bezier(glm::vec3(20.0f, 10.0f, 38.0f),
                    glm::vec3(10.4f, 19.2f, 18.3f),
                    glm::vec3(39.2f, 28.5f, 28.3f),
                    glm::vec3(45.3f, 39.9f, 36.0f));
    
    c1 = new Bezier(glm::vec3(20.0f, 10.0f, 38.0f),
                    glm::vec3(32.4f, 69.2f, 18.3f),
                    glm::vec3(12.2f, -28.5f, 28.3f),
                    glm::vec3(-32.3f, -49.9f, 36.0f));

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    delete(skybox);
    delete(squad);
    glDeleteProgram(Window::robotShader);
    glDeleteProgram(skyboxShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
    squad->update();
}

void Window::display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    
	// Use the shader of programID
    glUseProgram(Window::robotShader);
    //squad->draw(glm::mat4(1.0f));
    c0->draw(robotShader);
    c1->draw(robotShader);
    
    glDepthFunc(GL_LEQUAL);
    glUseProgram(skyboxShader);
    skybox->draw(skyboxShader);
    glDepthFunc(GL_LESS);
    
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
}

// Function to track the coordinates of the mouse in terms of a sphere
glm::vec3 trackBallMapping(float x, float y)
{
    // Vec v is the synthesized 3D position of the mouse location on the trackball
    glm::vec3 v;
    // d is the depth of the mouse location
    float d;
    
    // Caluclate mouse x and y position in trackball coordinates
    v.x = (2.0f * x - (float)Window::width) / (float)Window::width;
    v.y = ((float)Window::height - 2.0f * y) / (float)Window::height;
    
    // Initially mouse z position is set to zero
    v.z = 0.0f;
    
    // This is the distance from trackball origin to mouse location w/o considering depth
    d = glm::length(v);
    // This limits d to values 1.0 or less
    d = (d < 1.0f) ? d : 1.0f;
    
    // Calculate the z coordinate of the mouse position on the trackball
    v.z = sqrtf(1.001f - d * d);
    
    // Return the mouse location on the surface of the trackball
    return glm::normalize(v);
}

// Get x and y position of mouse in Window coordinates
void Window::cursor_pos_callback(GLFWwindow *window, double x, double y)
{
    Window::xpos = x;
    Window::ypos = y;
    
    glm::vec3 direction;

    glm::vec3 cur_point;
    
    if(Window::movement) {
        cur_point = trackBallMapping(Window::xpos, Window::ypos);
        direction = cur_point - Window::last_point;
        float velocity = glm::length(direction) * ROTATE_VELOCITY;
        
        if(velocity > 0.0001f) {
            glm::vec3 rot_axis = glm::cross(Window::last_point, cur_point);
            cam_pos = glm::rotate(glm::mat4(1.0f), glm::radians(velocity), rot_axis) * glm::vec4(cam_pos, 1.0f);
        }
        Window::last_point = cur_point;
    }
}

// Get mouse presses for rotating camera
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Rotate the object with left click
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == GLFW_PRESS) {
            Window::movement = true;
            Window::last_point = trackBallMapping(Window::xpos, Window::ypos);
        }
        else {
            Window::movement = false;
        }
    }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam_pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, yoffset)) * glm::vec4(cam_pos, 1.0f);
}


Transform* makeRobot()
{
    /* Right/Left Y-axis/Z-axis are swapped in labels/transformations because
       original robot layout has to be rotated about x-axis by -90 and y-axis by 180 */
    // Group to hold the robot scene graph
    Transform* robot = new Transform(glm::mat4(1.0f));
    
    // Body of robot in world coordinates
    Transform* body2wld = new Transform(glm::translate(glm::mat4(1.0f),
                                                       glm::vec3(0.0f, -50.0f, 0.0f)) *
                                        glm::rotate(glm::mat4(1.0f),
                                                    glm::radians(180.0f),
                                                    glm::vec3(0.0f, 1.0f, 0.0f)) *
                                        glm::rotate(glm::mat4(1.0f),
                                                    glm::radians(-90.0f),
                                                    glm::vec3(1.0f, 0.0f, 0.0f)));
    body2wld->addChild(new Geometry("./body.obj"));
    robot->addChild(body2wld);
    
    // Head of robot in terms of body coordinates
    Transform* head2body = new Transform(glm::translate(glm::mat4(1.0f),
                                                        glm::vec3(0.0f, 0.0f, -2.0f)));
    head2body->addChild(new Geometry("./head.obj"));
    body2wld->addChild(head2body);
    
    // Eyes of robot in terms of head coordinates
    Geometry* eye = new Geometry("./eyeball.obj");
    
    // Left eye
    Transform* l_eye2head = new Transform(glm::translate(glm::mat4(1.0f),
                                                         glm::vec3(-7.5f, 29.0f, 85.0f)));
    l_eye2head->addChild(eye);
    head2body->addChild(l_eye2head);
    
    // Right eye
    Transform* r_eye2head = new Transform(glm::translate(glm::mat4(1.0f),
                                                         glm::vec3(7.5f, 29.0f, 85.0f)));
    r_eye2head->addChild(eye);
    head2body->addChild(r_eye2head);
    
    // Limbs of robot in terms of body coordinates
    Geometry* limb = new Geometry("./limb.obj");
    float pivot = limb->findPivotPoint(); // Pivot point for the limbs

    // Left arm
    Transform* l_arm2body = new Transform(glm::mat4(1.0f));
     
    l_arm2body->addChild(limb);
    l_arm2body->setAnimate(0.25f, 45.0f, pivot);
    body2wld->addChild(l_arm2body);
    
    // Right arm
    Transform* r_arm2body = new Transform(glm::translate(glm::mat4(1.0f),
                                                         glm::vec3(53.5f, 0.0f, 0.0f)));
    r_arm2body->addChild(limb);
    r_arm2body->setAnimate(-0.25f, -45.0f, pivot);
    body2wld->addChild(r_arm2body);
    
    // Left leg
    Transform* l_leg2body = new Transform(glm::translate(glm::mat4(1.0f),
                                                         glm::vec3(17.5f, 0.0f, -45.0f)));
    l_leg2body->addChild(limb);
    l_leg2body->setAnimate(0.25f, 45.0f, pivot);
    body2wld->addChild(l_leg2body);
    
    // Right leg
    Transform* r_leg2body = new Transform(glm::translate(glm::mat4(1.0f),
                                                         glm::vec3(37.0f, 0.0f, -45.0f)));
    r_leg2body->addChild(limb);
    r_leg2body->setAnimate(-0.25f, -45.0f, pivot);
    body2wld->addChild(r_leg2body);
    
    return robot;
}

Transform* makeRobotSquad()
{
    Transform* group = new Transform(glm::mat4(1.0f));
    Transform* robot = makeRobot();
    // Create a 5 x 5 squad of robots
    float x_dist = 75.0f;
    float z_dist = 75.0f; // y_dist is z-depth due to original orientation of robot
    for(unsigned int i = 0; i < 5; i++) {
        for(unsigned int j = 0; j < 5; j++) {
            Transform* temp = new Transform(glm::translate(glm::mat4(1.0f),
                                                           glm::vec3((float) i * x_dist,
                                                                     0.0f,
                                                                     (float) j * z_dist)));
            temp->addChild(robot);
            group->addChild(temp);
        }
    }
    return group;
}
