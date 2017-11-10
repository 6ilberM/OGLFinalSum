#ifndef CAMERA_H
#define CAMERA_H

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "glew.h"
#include "freeglut.h"

class Camera
{
public:
#define BUTTON_UP 0
#define BUTTON_DOWN 1
	unsigned char keyState[255];
	float Speed;

	~Camera();

	void init();
	void keyMoveCamera(unsigned char _keyState[255], float _deltaTime); //New test for multi-input
	void setCameraFront(glm::vec3 _front);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	glm::vec3 &getPosition();
	glm::vec3 &getFront();
	glm::vec3 &getUp();

private:

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;

};

#endif // !CAMERA_H

