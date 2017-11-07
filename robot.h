#define _USE_MATH_DEFINES
#ifndef ROBOT_H
#define ROBOT_H
#include "Node.h"
#include <vector>
#include "Transform.h"
#include "Geometry.h"
class robot : public Node {
public:
	glm::mat4 toParent;
	Geometry* antenna;
	Geometry* body;
	Geometry* eyeball;
	Geometry* head;
	Geometry* limb;
	Transform *antennaleftTorobot;
	Transform *antennarightTorobot;
	Transform *headTorobot;
	Transform* bodyTorobot;
	Transform* eyeballTorobot;
	Transform* leftarmTorobot;
	Transform* rightarmTorobot;
	Transform* leftlegTorobot;
	Transform* rightlegTorobot;
	std::vector<Node*> children;
	bool state;
	int count;
	bool ball = false;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	void parse(const char* filepath);
	float xmax;
	float xmin;
	float ymax;
	float ymin;
	float zmax;
	float zmin;
	GLuint VBO, VAO, EBO, NBO;
	robot(glm::mat4 m,Geometry* antenna,Geometry* body,	Geometry* eyeball,	Geometry* head,	Geometry* limb);
	~robot();
	void draw(GLuint shaderProgram, glm::mat4 m);
	void update();
};
#endif