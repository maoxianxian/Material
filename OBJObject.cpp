#include "OBJObject.h"
#include <iostream>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"

using namespace std;
OBJObject::OBJObject(const char *filepath, int type)
{
	if (type == 0)
	{
		mater.ambient = glm::vec3(0.1,0.1,0.1);
		mater.diffuse = glm::vec3(0, 0, 0);
		mater.specular = glm::vec3(0.9, 0.0, 0.0);
		mater.shiness = 0.3;
	}
	if (type == 1)
	{
		mater.ambient = glm::vec3(0.1, 0.1, 0.1);
		mater.specular = glm::vec3(0, 0, 0);
		mater.diffuse = glm::vec3(0.0, 0.9, 0.0);
		mater.shiness = 1;
	}
	if (type == 2)
	{

		mater.ambient = glm::vec3(0.1, 0.1, 0.1);
		mater.specular = glm::vec3(0.0, 0.0, 0.8);
		mater.diffuse = glm::vec3(0.0, 0.0, 0.8);
		mater.shiness = 0.8;
	}
	if (type == 3)
	{
		mater.ambient = glm::vec3(0);
		mater.specular = glm::vec3(0);
		mater.diffuse = glm::vec3(0);
		mater.shiness = 0.1;
	}
	toWorld = glm::mat4(1.0f);
	parse(filepath);
	if (type != 3)
	{
		center();
		scale(2.0f / (xmax - xmin), 2.0f / (ymax - ymin), 2.0f / (zmax - zmin));
	}
	else
	{
		scalebefore(0.4f / (xmax - xmin), 0.4f / (ymax - ymin), 0.4f / (zmax - zmin));
	}
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
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
	//glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

OBJObject::~OBJObject()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char *filepath)
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	GLfloat x, y, z;  // vertex coordinates
	unsigned int i1, i2, i3; // indices
	string temps;
	GLfloat r, g, b;  // vertex color
	GLint c1, c2;    // characters read from file
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { cerr << "error loading file" << endl; exit(-1); }  // just in case the file can't be found or is corrupt
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
	fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::draw(GLuint shaderProgram)
{
	glm::mat4 modelview = Window::V * toWorld;
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),1,GL_FALSE,&toWorld[0][0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "ambient"), 1, &mater.ambient[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "diffuse") , 1, &mater.diffuse[0]);
	//cout << mater.ambient[0] << endl;
	glUniform3fv(glGetUniformLocation(shaderProgram, "specular") , 1, &mater.specular[0]);
	glUniform1f(glGetUniformLocation(shaderProgram, "shiness"), mater.shiness);
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
 	glBindVertexArray(0);
}

void OBJObject::center()
{
	translate(-(xmin + xmax) / 2, -(ymin + ymax) / 2, -(zmin + zmax) / 2);
}
void OBJObject::update()
{
}



void OBJObject::translate(float x, float y, float z)
{
	this->toWorld = this->toWorld*glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}
void OBJObject::translateAfter(float x, float y, float z)
{
	this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))*this->toWorld;
}

void OBJObject::scale(float x, float y, float z)
{
	this->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(x, x, x))*this->toWorld;
}
void OBJObject::scalebefore(float x, float y, float z)
{
	this->toWorld =this->toWorld*glm::scale(glm::mat4(1.0f), glm::vec3(x, x, x));
}
void OBJObject::resetPosition()
{
	this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-1 * this->toWorld[3][0], -1 * this->toWorld[3][1], -1 * this->toWorld[3][2]))*this->toWorld;
}
void OBJObject::resetScale()
{
	auto translateMat = glm::vec3(this->toWorld[3][0], this->toWorld[3][1], this->toWorld[3][2]);
	this->toWorld = glm::translate(glm::mat4(1.0f), translateMat);
}
glm::mat4 OBJObject::getMat()
{
	return this->toWorld;
}
std::vector<glm::vec3> OBJObject::getVert()
{
	return this->vertices;
}
std::vector <glm::vec3> OBJObject::getNormals()
{
	return this->normals;
}
void OBJObject::rotate(glm::vec3 axies, float deg)
{
	this->toWorld = glm::rotate(glm::mat4(1.0f) , deg, axies)*this->toWorld;
}