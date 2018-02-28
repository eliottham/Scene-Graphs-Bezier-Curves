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
Transform* robot;

vector<string> skybox_faces = {
    "./right.jpg",
    "./left.jpg",
    "./top.jpg",
    "./bottom.jpg",
    "./front.jpg",
    "./back.jpg"
};

void Window::initialize_objects()
{
    skybox = new Skybox(skybox_faces);
    Window::robotShader = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    skyboxShader = LoadShaders("./skybox_shader.vert", "./skybox_shader.frag");
    glUseProgram(skyboxShader);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
    
    // Group to hold robot scene graph
    robot = new Transform(glm::mat4(1.0f));
    
    // Body of robot
    Transform* body_rot = new Transform(glm::rotate(glm::mat4(1.0f),
                                                    glm::radians(180.0f),
                                                    glm::vec3(0.0f, 0.0f, 1.0f)) *
                                        glm::rotate(glm::mat4(1.0f),
                                                    glm::radians(90.0f),
                                                    glm::vec3(1.0f, 0.0f, 0.0f)));
    Transform* body_trans = new Transform(glm::translate(glm::mat4(1.0f),
                                                        glm::vec3(0.0f, -50.0f, 0.0f)));
    body_rot->addChild(new Geometry("./body.obj"));
    body_trans->addChild(body_rot);
    robot->addChild(body_trans);
    
    
    // Head of robot
    Transform* head_rot = new Transform(glm::rotate(glm::mat4(1.0f),
                                                    glm::radians(180.0f),
                                                    glm::vec3(0.0f, 0.0f, 1.0f)) *
                                        glm::rotate(glm::mat4(1.0f),
                                                    glm::radians(90.0f),
                                                    glm::vec3(1.0f, 0.0f, 0.0f)));
                                       
    Transform* head_trans = new Transform(glm::translate(glm::mat4(1.0f),
                                                         glm::vec3(0.0f, -52.5f, 0.0f)));
    head_rot->addChild(new Geometry("./head.obj"));
    head_trans->addChild(head_rot);
    robot->addChild(head_trans);

    // Left eye of robot
    Geometry* eyeball = new Geometry("./eyeball.obj");
    Transform* l_eye_trans = new Transform(glm::translate(glm::mat4(1.0f),
                                                        glm::vec3(-7.5f, 50.0f, 12.5f)));
    l_eye_trans->addChild(eyeball);
    robot->addChild(l_eye_trans);
    
    // Right eye of robot
    Transform* r_eye_trans = new Transform(glm::translate(glm::mat4(1.0f),
                                                          glm::vec3(7.5f, 50.0f, 12.5f)));
    r_eye_trans->addChild(eyeball);
    robot->addChild(r_eye_trans);
    
    // Left arm of robot
    Geometry* limb = new Geometry("./limb.obj");
    Transform* l_arm_rot = new Transform(glm::rotate(glm::mat4(1.0f),
                                                   glm::radians(180.0f),
                                                   glm::vec3(0.0f, 0.0f, 1.0f)) *
                                       glm::rotate(glm::mat4(1.0f),
                                                   glm::radians(90.0f),
                                                   glm::vec3(1.0f, 0.0f, 0.0f)));
    Transform* l_arm_trans = new Transform(glm::translate(glm::mat4(1.0f),
                                                          glm::vec3(-53.5f, -55.0f, 0.0f)));
    l_arm_rot->addChild(limb);
    l_arm_trans->addChild(l_arm_rot);
    robot->addChild(l_arm_trans);
    
    // Right arm of robot
    Transform* r_arm_rot = new Transform(glm::rotate(glm::mat4(1.0f),
                                                     glm::radians(180.0f),
                                                     glm::vec3(0.0f, 0.0f, 1.0f)) *
                                         glm::rotate(glm::mat4(1.0f),
                                                     glm::radians(90.0f),
                                                     glm::vec3(1.0f, 0.0f, 0.0f)));
    Transform* r_arm_trans = new Transform(glm::translate(glm::mat4(1.0f),
                                                          glm::vec3(0.0f, -55.0f, 0.0f)));
    r_arm_rot->addChild(limb);
    r_arm_trans->addChild(r_arm_rot);
    robot->addChild(r_arm_trans);
    
    // Left leg of robot
    Transform* l_leg_rot = new Transform(glm::rotate(glm::mat4(1.0f),
                                                     glm::radians(180.0f),
                                                     glm::vec3(0.0f, 0.0f, 1.0f)) *
                                         glm::rotate(glm::mat4(1.0f),
                                                     glm::radians(90.0f),
                                                     glm::vec3(1.0f, 0.0f, 0.0f)));
    Transform* l_leg_trans = new Transform(glm::translate(glm::mat4(1.0f),
                                                          glm::vec3(-38.5f, -95.0f, 0.0f)));
    l_leg_rot->addChild(limb);
    l_leg_trans->addChild(l_leg_rot);
    robot->addChild(l_leg_trans);
    
    // Right leg of robot
    Transform* r_leg_rot = new Transform(glm::rotate(glm::mat4(1.0f),
                                                     glm::radians(180.0f),
                                                     glm::vec3(0.0f, 0.0f, 1.0f)) *
                                         glm::rotate(glm::mat4(1.0f),
                                                     glm::radians(90.0f),
                                                     glm::vec3(1.0f, 0.0f, 0.0f)));
    Transform* r_leg_trans = new Transform(glm::translate(glm::mat4(1.0f),
                                                          glm::vec3(-15.5f, -95.0f, 0.0f)));
    r_leg_rot->addChild(limb);
    r_leg_trans->addChild(r_leg_rot);
    robot->addChild(r_leg_trans);
    
    
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
    delete(skybox);
    delete(robot);
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
}

void Window::display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    
	// Use the shader of programID
    glUseProgram(Window::robotShader);
    robot->draw(glm::mat4(1.0f));
    
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
