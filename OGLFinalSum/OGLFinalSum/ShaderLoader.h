#pragma once

#include "glew.h"
#include "freeglut.h"
#include "shaderloader.h"
#include <iostream>


class shaderloader
{
private:

	std::string readshader(char *filename);
	GLuint createshader(GLenum shadertype,std::string source,char* shadername);

public:

	shaderloader(void);
	~shaderloader(void);
	GLuint createprogram(char* vertexshaderfilename,char* fragmentshaderfilename);
	GLuint createprogram(char* vertexshaderfilename, char* fragmentshaderfilename,char* geometryshaderfilename);
	GLuint createProgram(char* vertexShaderFilename, char* fragmentShaderFilename, char* TessControlShaderFilename, char* TessEvalShaderFilename);
};
#pragma once
