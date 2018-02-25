#include "Window.h"

const char* window_title = "GLFW Starter Project";
Cube * cube;
GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "./shader.vert"
#define FRAGMENT_SHADER_PATH "./shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
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

OBJObject* bunny;

void Window::initialize_objects()
{
	cube = new Cube();
    bunny = new OBJObject("./bunny.obj");
	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
    delete(bunny);
	glDeleteProgram(shaderProgram);
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
	// Call the update function the cube
	//cube->update();
}

void Window::display_callback(GLFWwindow* window)
{
    Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
   
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
     bunny->draw(shaderProgram);
	//cube->draw(shaderProgram);

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
        float velocity = glm::length(direction);
        
        if(velocity > 0.0001f) {
            glm::vec3 rot_axis = glm::cross(Window::last_point, cur_point);
            cam_pos = glm::rotate(glm::mat4(1.0f), velocity, rot_axis) * glm::vec4(cam_pos, 1.0f);
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
    cam_pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -yoffset)) * glm::vec4(cam_pos, 1.0f);
}
