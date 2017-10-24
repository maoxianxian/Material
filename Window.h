#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "light.h"
/*struct directionalLight {
	glm::vec3 Color;
	glm::vec3 Direction;
};
struct pointLight {
	glm::vec3 Color;
	glm::vec3 Position;
	float attenuation;
};
struct spotLight {
	glm::vec3 Color;
	glm::vec3 Position;
	float attenuation;
	float exponent;
	float coneAngle;
	glm::vec3 coneDirection;
};*/
class Window
{
public:
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackmap(double x, double y);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
