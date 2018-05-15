#include "error.h"

Error::Error()
{

}

std::string Error::GetStringForSource(GLenum source)
{
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "Window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "Shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "Third party";
	case GL_DEBUG_SOURCE_APPLICATION:
		return "Application";
	case GL_DEBUG_SOURCE_OTHER:
		return "Other";
	default:
		assert(false);
		return "";
	}
}

//debug callback function
//Inspired by blog.nobel-jorgensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
//
void APIENTRY Error::OpenGLCallbackFunction(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam
)
{
	switch (type) 
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR" << std::endl;
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR" << std::endl;
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR" << std::endl;
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY" << std::endl;
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE" << std::endl;
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "OTHER" << std::endl;
		break;
	}
	//void pointer needs to be cast to its oriiginal type
	//if you don't know the original type, this can't be done
	std::cout << "*********************************" << std::endl;
	std::cout << "ERROR CALLBACK at location = " << *(int*)userParam << std::endl;
	std::cout << "ERROR CALLBACK: " << GetStringForSource(source).c_str() << std::endl;
	std::cout << "ERROR CALLBACK: " << message << std::endl;
	if (severity == GL_DEBUG_SEVERITY_HIGH) {
		std::cout << "Aborting..." << std::endl;
		abort();
	}
}

void CheckGLerror()
{
	GLenum error(glGetError());
	while (error != GL_NO_ERROR) {

		std::string err;

		switch (error) {
		case GL_INVALID_OPERATION:      err = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           err = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          err = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          err = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  err = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		std::cout << err.c_str() << std::endl;
		return;
		//err = glGetError();
	}
}