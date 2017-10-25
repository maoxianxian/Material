#include "light.h"
#include <iostream>
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
void light::draw(GLuint shaderProgram, glm::mat4 modelview) {
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
		//glm::vec3 pos = glm::vec3(1.0f);
		//pos.x= (modelview*glm::vec4(position, 1.0f))[0];
		//pos.y = (modelview*glm::vec4(position, 1.0f))[1];
		//pos.z = (modelview*glm::vec4(position, 1.0f))[2];

		glUniform3fv(temp, 1,&position[0]);
		temp = glGetUniformLocation(shaderProgram, "Light.att");
		glUniform1f(temp, attenuation);
	}
	if (type == 2)
	{
		temp = glGetUniformLocation(shaderProgram, "Light.cutoff");
		glUniform1f(temp, coneAngle);
		temp = glGetUniformLocation(shaderProgram, "Light.exponent");
		glUniform1f(temp, exponent);
	}
}