#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class OBJObject
{
private:
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4 toWorld;
	float angle;
	float scaleParam = 1;
	float orbitAngle = 0;
public:
	OBJObject(const char* filepath);
	void update();

	void spin(float);
	void parse(const char* filepath);
	void draw();
	void translate(float x, float y, float z);
	void scale(float x);
	void orbitLeft();
	void orbitRight();
	void resetPosition();
	void resetScale();
	glm::mat4 getMat();
	std::vector<glm::vec3> getVert();
	std::vector <glm::vec3> getNormals();
};

#endif