#ifndef SPHERE_H
#define SPHERE_H

#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h> 
#include "Light.h"
using namespace std;

class CLight;
class Camera;

class CSphere
{
public:
	Camera* camera;
	CLight* light;

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;


	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 scale;
	glm::vec3 angle;
	glm::vec3 rotationAxis;
	glm::vec3 color;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;

	float speed;
	float Radius;

	float ambientStrength;
	float specularStrength;

	CSphere(Camera* _camera, GLuint prog, float _Radius, CLight *_light, float _ambientStrength, float _specularStrength);
	~CSphere();

	void update(unsigned char keyState[255], glm::vec3 v_Ballpos);

	void render();
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	glm::vec3 setAngle(glm::vec3& _Vec, float _Angle);
	void setRotation(glm::vec3 angle);
	void setRotationAxis(glm::vec3 rotationAxis);
	void setColor(glm::vec3 _color);
	void setCamera(Camera* camera);
	void setSpeed(float _speed);
	void setProgram(GLuint program);

	void setTexture(std::string texFileName);

	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getRotation();
	glm::vec3 getRotationAxis();
	glm::vec3 getColor();
	//secondary edition

	void moveUp();
	void moveLeft();
	void moveRight();
	void moveDown();

	void moveForward();

	void moveBackward();

	void rotate(glm::vec3 axis);

};
#endif // !SPHERE_H