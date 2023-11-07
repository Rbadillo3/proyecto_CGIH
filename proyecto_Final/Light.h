#pragma once

#include <glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);
	void setAmbienIntensity(GLfloat aIntensity) { ambientIntensity = aIntensity; }
	void setDiffuseIntensity(GLfloat dIntensity) { diffuseIntensity = dIntensity; }
	~Light();
	//Si una variable es superprotected, significa que se trata de una super clase
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

