#ifndef _LIGHT_H_
#define _LIGHT_H_
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "OBJObject.h"
class light
{
public:
	int type;//0 for directional, 1 for point light, 2 for spotlight
	glm::vec3 position;
	glm::vec3 color;
	float attenuation;
	float ambientCoefficient;
	float coneAngle;
	glm::vec3 Direction;
	float exponent;
	OBJObject * obj;
	light(glm::vec3 Direction, glm::vec3 color);
	light(glm::vec3 position, glm::vec3 color, float attenuation);
	light(glm::vec3 position, glm::vec3 color, float attenuation, glm::vec3 coneDirection, float coneAngle, float exponent);
	void draw(GLuint shaderProgram, glm::mat4 modelview);
	void rotate(glm::vec3 aix,float deg);
};