#ifndef LIGHT_H
#define LIGHT_H
#pragma once
#include <iostream>
#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"
#include <iostream>

class CLight
{
public:
	float speed = 0.5f;
	glm::vec3 &getPosition();
	glm::vec3 &getColor();

	CLight(Camera* camera, GLuint program);

	void render();
	void update(unsigned char keystate[]);
	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void moveup();
	glm::vec3 m_origin;
	void movedown();
	~CLight();

private:

	glm::vec3 LightPos;
	glm::vec3 LightColor;

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;

	Camera* camera;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;

};






#endif // !LIGHT_H
