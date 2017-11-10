#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "Utils.h"
#include <iostream>
class CfrBuff
{
public:
	CfrBuff(GLuint prog);
	~CfrBuff();

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;
	

	
	void init();
		void render(bool _CanRun);
		//for render
		void bindFbo();
		void unbindFrameBf();

		GLuint program;
		GLuint frameBuffer;
		GLuint renderTexture;
		GLuint rbo;

		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint texture;
		GLuint m_program;
private:


};




#endif // !FRAMEBUFFER_H
