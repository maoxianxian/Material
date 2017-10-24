#include "light.h"
light::light(glm::vec3 Direction, glm::vec3 color)
{
	type = 0;
	this->Direction = Direction;
	this->color = color;
}

light::light(glm::vec3 position, glm::vec3 color, float attenuation)
{
	type = 1;
	this->position = position;
	this->color = color;
	this->attenuation = attenuation;
}
light::light(glm::vec3 position, glm::vec3 color, float attenuation, glm::vec3 coneDirection, float coneAngle, float exponent)
{
	type = 2;
	this->position = position;
	this->color = color;
	this->attenuation = attenuation;
	this->Direction = coneDirection;
	this->coneAngle = coneAngle;
	this->exponent = exponent;
}
void light::draw(GLuint shaderProgram) {
	int temp = glGetUniformLocation(shaderProgram, "Light.light_mode");
	glUniform1i(temp, type);
	temp = glGetUniformLocation(shaderProgram, "Light.light_color");
	glUniform3fv(temp,1,&color[0]);
	if (type == 0 || type == 2)
	{
		temp = glGetUniformLocation(shaderProgram, "Light.light_dir");
		glUniform3fv(temp, 1, &Direction[0]);
	}
	if (type == 1 || type == 2)
	{
		temp = glGetUniformLocation(shaderProgram, "Light.light_pos");
		glUniform3fv(temp, 1, &position[0]);
		temp = glGetUniformLocation(shaderProgram, "Light.att");
		glUniform1i(temp, attenuation);
	}
	if (type == 2)
	{
		temp = glGetUniformLocation(shaderProgram, "Light.cutoff");
		glUniform1i(temp, coneAngle);
		temp = glGetUniformLocation(shaderProgram, "Light.exponent");
		glUniform1i(temp, exponent);
	}
}