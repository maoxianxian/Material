#include "robot.h"
#include <iostream>
#include "Window.h"
robot::robot(glm::mat4 m,Geometry* antenna,Geometry* body,Geometry* eyeball,Geometry* head,Geometry* limb)
{
	parse("C:\\Users\\c7ye\\Desktop\\CSE167StarterCode2-master\\sphere.obj");
	//std::cout << vertices.size() << std::endl;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &NBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	state = true;
	count = 0;
	toParent = m*glm::scale(glm::mat4(1.0f),glm::vec3(0.5,0.5,0.5));
	this->antenna = antenna;
	this->body = body;
	this->eyeball = eyeball;
	this->head = head;
	this->limb = limb;
	antennaleftTorobot = new Transform(glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.08, 0.3)));
	antennaleftTorobot->rotate(glm::vec3(1, 0, 0), -(float)M_PI / 1.6);
	antennaleftTorobot->rotate(glm::vec3(0, 0, 1), (float)M_PI / 2);
	antennaleftTorobot->addChild(antenna);
	children.push_back(antennaleftTorobot);

	antennarightTorobot = new Transform(glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.08, 0.3)));
	antennarightTorobot->rotate(glm::vec3(1, 0, 0), -(float)M_PI / 1.6);
	antennarightTorobot->rotate(glm::vec3(0, 0, 1), (float)M_PI / 2);	antennarightTorobot->addChild(antenna);
	antennarightTorobot->translate(3, 0, 0);
	children.push_back(antennarightTorobot);
	
	headTorobot = new Transform(glm::mat4(1.0f));
	headTorobot->scale(0.1, 0.1, 0.1);
	headTorobot->rotate(glm::vec3(1, 0, 0), 3.0f*M_PI / 2);
	headTorobot->translate(1.5f, -5.5f, 0);
	headTorobot->addChild(head);
	children.push_back(headTorobot);
	
	bodyTorobot = new Transform(glm::mat4(1.0f));
	bodyTorobot->scale(0.1, 0.1, 0.1);
	bodyTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	bodyTorobot->translate(1.5f, -3, 0);
	bodyTorobot->addChild(body);
	children.push_back(bodyTorobot);
	
	eyeballTorobot = new Transform(glm::mat4(1.0f));
	eyeballTorobot->scale(0.1, 0.1, 0.1);
	eyeballTorobot->translate(1.5, -1, 2);
	eyeballTorobot->addChild(eyeball);
	children.push_back(eyeballTorobot);
	
	leftarmTorobot = new Transform(glm::mat4(1.0f));
	leftarmTorobot->scale(0.1, 0.09, 0.1);
	leftarmTorobot->rotate(glm::vec3(0, 1, 0), -M_PI / 18);
	leftarmTorobot->translate(0, -3, 0);
	leftarmTorobot->addChild(limb);
	children.push_back(leftarmTorobot);
	
	rightarmTorobot = new Transform(glm::mat4(1.0f));
	rightarmTorobot->scale(0.1, 0.09, 0.1);
	rightarmTorobot->rotate(glm::vec3(0, 0, 1), M_PI / 18);
	rightarmTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	rightarmTorobot->translate(5.1, -3, 0);
	rightarmTorobot->addChild(limb);
	children.push_back(rightarmTorobot);

	
	leftlegTorobot = new Transform(glm::mat4(1.0f));
	leftlegTorobot->scale(0.1, 0.07, 0.1);
	leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 5.0f);
	leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 2);
	leftlegTorobot->translate(1.5, -6, 0.5);
	leftlegTorobot->addChild(limb);
	children.push_back(leftlegTorobot);
	
	rightlegTorobot = new Transform(glm::mat4(1.0f));
	rightlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 5.0f);
	rightlegTorobot->scale(0.1, 0.07, 0.1);
	rightlegTorobot->translate(3.5, -6, 0.5);
	rightlegTorobot->addChild(limb);
	children.push_back(rightlegTorobot);
}

robot::~robot()
{
	delete headTorobot;
	delete bodyTorobot;
	delete eyeballTorobot;
	delete leftarmTorobot;
	delete rightarmTorobot;
	delete leftlegTorobot;
	delete rightlegTorobot;
}
void robot::parse(const char* filepath)
{
	FILE* fp;     // file pointer
	GLfloat x, y, z;  // vertex coordinates
	unsigned int i1, i2, i3; // indices
	std::string temps;
	GLfloat r, g, b;  // vertex color
	GLint c1, c2;    // characters read from file
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt
																				   //int i = 0;
	while (!feof(fp))
	{
		c1 = fgetc(fp);

		if (c1 != '#'&&c1 != 13 && c1 != 10)
		{
			c2 = fgetc(fp);

			if ((c1 == 'v') && (c2 == ' '))
			{
				int a = fscanf(fp, "%f %f %f", &x, &y, &z);
				glm::vec3 temp = glm::vec3(x, y, z);
				vertices.push_back(temp);
				if (x > xmax)
				{
					xmax = x;
				}
				if (x < xmin)
				{
					xmin = x;
				}if (y > ymax)
				{
					ymax = y;
				}if (y < ymin)
				{
					ymin = y;
				}if (z > zmax)
				{
					zmax = z;
				}if (z < zmin)
				{
					zmin = z;
				}
			}
			else
			{
				if ((c1 == 'f') && (c2 == ' '))
				{
					fscanf(fp, "%d %s %d %s %d", &i1, &temps, &i2, &temps, &i3);
					indices.push_back(i1 - 1);
					indices.push_back(i2 - 1);
					indices.push_back(i3 - 1);
				}
				else if ((c1 == 'v') && (c2 == 'n') && (fgetc(fp) == ' '))
				{
					fscanf(fp, "%f %f %f", &x, &y, &z);
					glm::vec3 temp = glm::vec3(x, y, z);
					normals.push_back(temp);
				}
			}
		}
		char buffer[128];
		fgets(buffer, 128, fp);
	}
	//center();
	fclose(fp);   // make sure you don't forget to close the file when done
}
void robot::draw(GLuint shaderProgram, glm::mat4 m)
{
	if (ball)
	{
		glm::mat4 modelview = Window::V * m*toParent*glm::scale(glm::mat4(1.0f), glm::vec3(6, 6, 6))*glm::translate(glm::mat4(1.0f), glm::vec3(0.25, -0.7, 0));
		GLuint uProjection = glGetUniformLocation(shaderProgram, "projection");
		GLuint uModelview = glGetUniformLocation(shaderProgram, "modelview");
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &(m*toParent)[0][0]);

		glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
		glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
		glBindVertexArray(VAO);

		glDrawArrays(GL_LINES, 0, vertices.size());

		glBindVertexArray(0);
	}
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->draw(shaderProgram, m*toParent);
	}
}

void robot::update()
{
	if (state)
	{
		leftarmTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 180.0f);
		rightarmTorobot->rotate(glm::vec3(1, 0, 0), -M_PI / 180.0f);
		leftlegTorobot->rotate(glm::vec3(1, 0, 0), -M_PI / 180.0f);
		rightlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 180.0f);

	}
	else
	{
		leftarmTorobot->rotate(glm::vec3(1, 0, 0), -M_PI / 180.0f);
		rightarmTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 180.0f);
		leftlegTorobot->rotate(glm::vec3(1, 0, 0), M_PI / 180.0f);
		rightlegTorobot->rotate(glm::vec3(1, 0, 0), -M_PI / 180.0f);
	}
	this->toParent = this->toParent*glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.03f));
	count++;
	if (count == 90)
	{
		state = !state;
		count = 0;
	}
}