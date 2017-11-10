
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"
#include "glm\glm/glm.hpp"
#include "glm\glm/gtc/matrix_transform.hpp"
#include "glm\glm/gtc/type_ptr.hpp"
#include <iostream>

#include "ShaderLoader.h"
#include "Utils.h"


class Light;
class Camera;


#pragma once
class Skybox
{
public:

	Skybox(ModelType modelType, Camera* _camera, std::vector<std::string>cubeMapTexture, GLuint prog);

	~Skybox();

	void update(GLfloat time);
	void render();
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setRotation(glm::vec3 angle);
	void setRotationAxis(glm::vec3 rotationAxis);

	void setColor(glm::vec3 _color);
	void setCamera(Camera* camera);
	void setSpeed(float _speed);
	void setProgram(GLuint program);


	GLuint loadCubeMap(std::vector<const GLchar*> faces);


	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getRotation();
	glm::vec3 getRotationAxis();

	glm::vec3 getColor();

	Camera* camera;
	Light* light;

	bool bIsTextureSet = false;
	float speed;

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 angle;
	glm::vec3 rotationAxis;
	glm::vec3 color;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;
};


