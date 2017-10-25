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
#include "Window.h"

struct material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shiness;
};
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
	GLuint VBO, VAO, EBO, NBO;
	GLuint uProjection, uModelview;
	float xmax;
	float xmin;
	float ymax;
	float ymin;
	float zmax;
	float zmin;
public:
	
	material mater;
	/*directionalLight * direction;
	pointLight* point;
	spotLight* spot;*/
	OBJObject(const char* filepath, int type);
	~OBJObject();
	void update();
	void center();
	void spin(float deg);
	void parse(const char* filepath);
	void draw(GLuint shaderProgram);
	void translate(float x, float y, float z);
	void translateAfter(float x, float y, float z);
	void scale(float x, float y, float z);
	void resetPosition();
	void resetScale();
	void rotate(glm::vec3 axies, float deg);
	glm::mat4 getMat();
	std::vector<glm::vec3> getVert();
	std::vector <glm::vec3> getNormals();
};

#endif