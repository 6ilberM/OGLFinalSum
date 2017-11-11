#include "camera.h"
#include<iostream>
Camera::~Camera() {}

void Camera::init()
{
	Speed = 0.5f*2; //Keyboard input moveSpeed

	Pos = glm::vec3(0.0f, 2.0f, 28.0f);
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::lookAt(Pos, Pos + Front, Up);
	projectionMatrix = glm::perspective(45.0f, 800.0f / 800.0f, 1.0f, 2000.0f); //Window Width / Window Height
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::keyMoveCamera(unsigned char _keyState[255], float _deltaTime)
{
	//Up
	if (_keyState[(unsigned char)'w'] == BUTTON_DOWN)
	{
		Pos += Speed * Front * 1.0f;
	}
	//Down
	if (_keyState[(unsigned char)'s'] == BUTTON_DOWN)
	{
		Pos -= Speed * Front * 1.0f;
	}
	//Left
	if (_keyState[(unsigned char)'a'] == BUTTON_DOWN)
	{
		Pos -= Speed * glm::normalize(glm::cross(Front, Up)) *1.0f;
	}
	//Right
	if (_keyState[(unsigned char)'d'] == BUTTON_DOWN)
	{
		Pos += Speed * glm::normalize(glm::cross(Front, Up)) * 1.0f;
	}
	//Jump/Raise
	if (_keyState[(unsigned char)' '] == BUTTON_DOWN)
	{
		Pos += Speed * Up *1.0f;
	}
	//Crouch/Lower
	if (_keyState[(unsigned char)'c'] == BUTTON_DOWN)
	{
		Pos -= Speed * Up *1.0f;
	}
	//if (_keyState[(unsigned char)'r'] == BUTTON_DOWN)
	//{
	//	Pos = glm::vec3(0.0f, 2.0f, 28.0f);
	//	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	//	Up = glm::vec3(0.0f, 1.0f, 0.0f);

	//}
	viewMatrix = glm::lookAt(Pos, Pos + Front, Up);
}

void Camera::setCameraFront(glm::vec3 _front)
{
	Front = _front;

	viewMatrix = glm::lookAt(Pos, Pos + Front, Up);
}

glm::vec3 &Camera::getPosition()
{
	return Pos;
}

glm::vec3 &Camera::getFront()
{
	return Front;
}

glm::vec3 &Camera::getUp()
{
	return Up;
}