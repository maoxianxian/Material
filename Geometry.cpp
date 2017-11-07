#include "Geometry.h""
#include "Window.h"
Geometry::Geometry(const char* filepath)
{
	parse(filepath);
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
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
Geometry::~Geometry(){}
void Geometry::draw(GLuint shaderProgram, glm::mat4 m)
{
	m = m*centermat;
	glm::mat4 modelview = Window::V * m;
	GLuint uProjection = glGetUniformLocation(shaderProgram, "projection");
	GLuint uModelview = glGetUniformLocation(shaderProgram, "modelview");
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &m[0][0]);

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Geometry::update()
{

}
void Geometry::center()
{
	this->centermat = this->centermat*glm::translate(glm::mat4(1.0f), glm::vec3(-(xmin + xmax) / 2, -(ymin + ymax) / 2, -(zmin + zmax) / 2));
}
void Geometry::parse(const char* filepath)
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
	center();
	fclose(fp);   // make sure you don't forget to close the file when done
}