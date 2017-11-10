#ifndef STAR_H
#define STAR_H
#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"

class Camera;

class GeoStar
{
public:
	Camera* m_camera;

	GeoStar(GLuint prog, Camera* _camera);
	~GeoStar();

	GLuint program;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	void render();

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
	void update();
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	glm::vec3 setAngle(glm::vec3& _Vec, float _Angle);
	void setRotation(glm::vec3 angle);
	void setRotationAxis(glm::vec3 rotationAxis);

	//setter

private:

};


#endif // !STAR_H
