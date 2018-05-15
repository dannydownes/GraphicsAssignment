#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "windows.h"

class Error
{
public:
	Error();
	std::string GetStringForSource(GLenum source);
	void APIENTRY OpenGLCallbackFunction(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam);
private:

};