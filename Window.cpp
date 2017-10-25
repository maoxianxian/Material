#define _USE_MATH_DEFINES
#include "window.h"
#include <math.h>
#include "OBJObject.h"
const char* window_title = "GLFW Starter Project";
OBJObject * obj;
OBJObject * bunny;
OBJObject * bear;
OBJObject * dragon;
light* currlit;
light direction(glm::vec3(0, -1, 0), glm::vec3(1.0f));
light point(glm::vec3(-5.0f, 5.0f, -5.0f), glm::vec3(1.0f), 0.1f);
light spot(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f), 0.1f, glm::vec3(0.0f, -1.0f, 0.0f), (float)M_PI / 3.0f, 3.0f);
GLint shaderProgram;
bool normalmode = false;
bool conrollight = false;
// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
double prexpos, preypos, prerightx, prerighty;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{

	bunny = new OBJObject("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\bunny.obj",0);
	bear = new OBJObject("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\bear.obj",2);
	dragon = new OBJObject("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\dragon.obj",1);
	currlit = &direction;
	obj = bunny;
	//  the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(obj);
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
	obj->update();
}
void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &prexpos, &preypos);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &prerightx, &prerighty);
	}
}
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset != 0)
	{
		obj->translateAfter(0, 0, yoffset);
	}
}
void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube	
	int temp = glGetUniformLocation(shaderProgram, "mode");
	if (normalmode)
	{
		glUniform1i(temp, 1);
	}
	else
	{
		glUniform1i(temp, 0);
	}
	//std::cout << mode << std::endl;
	currlit->draw(shaderProgram,Window::V*obj->getMat());
	obj->draw(shaderProgram);
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		double xpos, ypos;

		if (!conrollight)
		{
			glfwGetCursorPos(window, &xpos, &ypos);
			if (xpos != prexpos || ypos != preypos)
			{
				glm::vec3 prevec = trackmap(prexpos, preypos);
				glm::vec3 curvec = trackmap(xpos, ypos);
				glm::vec3 res = glm::cross(prevec, curvec);
				float cos = glm::dot(prevec, curvec) / (glm::length(prevec)*glm::length(curvec));
				if (cos > 1)
				{
					cos = 1;
				}
				float deg = acos(cos);
				obj->rotate(res, deg);
			}
		}
		else
		{
			glfwGetCursorPos(window, &xpos, &ypos);
			if (xpos != prexpos || ypos != preypos)
			{
				glm::vec3 prevec = trackmap(prexpos, preypos);
				glm::vec3 curvec = trackmap(xpos, ypos);
				glm::vec3 res = glm::cross(prevec, curvec);
				float cos = glm::dot(prevec, curvec) / (glm::length(prevec)*glm::length(curvec));
				if (cos > 1)
				{
					cos = 1;
				}
				float deg = acos(cos);
				//currlit->rotate(res, deg);
			}
		}
		prexpos = xpos;
		preypos = ypos;
	}
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (!conrollight)
		{
			if (xpos != prerightx || ypos != prerighty)
			{
				obj->translateAfter((xpos - prerightx)*28.0f / Window::width, (prerighty - ypos)*28.0f / Window::height, 0);
			}
		}
		prerightx = xpos;
		prerighty = ypos;
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}
glm::vec3 Window::trackmap(double x, double y)
{
	glm::vec3 result( (2.0f * x - width) / width, (height- 2.0f*y)/height, 0.0f);
	float length = glm::length(result);
	if (length > 1.0f)
	{
		length = 1.0;
	}
	result[2] = sqrt(1.001 - length*length);
	return glm::normalize(result);
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
		else if (key == GLFW_KEY_0)
		{
			conrollight = !conrollight;
		}
		else if (key == GLFW_KEY_F1)
		{
			obj = bunny;
		}
		else if (key == GLFW_KEY_F2)
		{
			obj = dragon;
		}
		else if (key == GLFW_KEY_F3)
		{
			obj = bear;
		}
		else if (key == GLFW_KEY_N)
		{
			normalmode = !normalmode;
		}
		else if (key == GLFW_KEY_1)
		{
			currlit = &direction;
		}
		else if (key == GLFW_KEY_2)
		{
			currlit = &point;
		}
		else if (key == GLFW_KEY_3)
		{
			currlit = &spot;
		}
		else if (key == GLFW_KEY_S)
		{
			if (mods)
			{
				obj->scale(1.1, 1.1, 1.1);
			}
			else
			{
				obj->scale(0.9, 0.9, 0.9);
			}
		}
		else if (key == GLFW_KEY_R)
		{
			if (!mods)
			{
				obj->resetPosition();
			}
		}
	}
	if (action == GLFW_REPEAT)
	{
		if (key == GLFW_KEY_S)
		{
			if (mods)
			{
				obj->scale(1.1, 1.1, 1.1);
			}
			else
			{
				obj->scale(0.9, 0.9, 0.9);
			}
		}
	}
}
