#ifndef TESSEL_H
#define TESSEL_H
#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"

class Camera;

class CTessel
{
public:
	Camera* m_camera;

	CTessel(GLuint prog, Camera* _camera);
	~CTessel();

	GLuint program;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	void render();

	float GetTessLevel(glm::vec3 Distance0, glm::vec3 Distance1);

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;

	glm::vec3 position;

	glm::vec3 scale;
	glm::vec3 angle;
	glm::vec3 rotationAxis;
	glm::vec3 color;

	//getter
	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getRotation();
	glm::vec3 getRotationAxis();

	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	glm::vec3 setAngle(glm::vec3& _Vec, float _Angle);
	void setRotation(glm::vec3 angle);
	void setRotationAxis(glm::vec3 rotationAxis);

	//setter

private:

};


#endif // !TESSEL_H
