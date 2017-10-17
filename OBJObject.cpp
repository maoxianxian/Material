#include "OBJObject.h"
#include <iostream>
#include <math.h>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
OBJObject::OBJObject(const char *filepath)
{
	toWorld = glm::mat4(1.0f);
	parse(filepath);
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
		c2 = fgetc(fp);
		if ((c1 == 'v') && (c2 == ' '))
		{
			int a = fscanf(fp, "%f %f %f", &x, &y, &z);
			glm::vec3 temp = glm::vec3(x, y, z);
			vertices.push_back(temp);
		}
		else
		{
			if ((c1 == 'f') && (c2 == ' '))
			{
				fscanf(fp, "%d %s %d %s %d", &i1, &temps, &i2, &temps, &i3);
				indices.push_back(i1);
				indices.push_back(i2);
				indices.push_back(i3);
				cout << i3 << endl;
			}
			else if ((c1 == 'v') && (c2 == 'n') && (fgetc(fp) == ' '))
			{
				fscanf(fp, "%f %f %f", &x, &y, &z);
				glm::vec3 temp = glm::vec3(x, y, z);
				normals.push_back(temp);
			}
		}
		char buffer[128];
		fgets(buffer, 128, fp);
	}
	fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::draw()
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	glBegin(GL_POINTS);
	// Loop through all the vertices of this OBJ Object and render them
	//cout << normals.size() - vertices.size()<<endl;
	for (unsigned int i = 0; i < vertices.size(); ++i)
	{
		//cout << vertices.size() - normals.size() << endl;
		GLfloat length = normals[i].x*normals[i].x + normals[i].y*normals[i].y + normals[i].z*normals[i].z;
		length = sqrt(length);
		GLfloat x = (normals[i].x / length + 1) / 2;
		GLfloat y = (normals[i].y / length + 1) / 2;
		GLfloat z = (normals[i].z / length + 1) / 2;
		glColor3f(x, y, z);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void OBJObject::update()
{
	spin(1.0f);
}

void OBJObject::spin(float deg)
{
	// This creates the matrix to rotate the cube
	this->toWorld = toWorld*glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::translate(float x, float y, float z)
{
	this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z))*this->toWorld;
}

void OBJObject::scale(float x)
{
	this->toWorld = this->toWorld*glm::scale(glm::mat4(1.0f), glm::vec3(x, x, x));
}
void OBJObject::orbitLeft()
{
	this->toWorld = glm::rotate(glm::mat4(1.0f), 3 / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))*this->toWorld;
}
void OBJObject::orbitRight()
{
	this->toWorld = glm::rotate(glm::mat4(1.0f), -3 / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f))*this->toWorld;
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