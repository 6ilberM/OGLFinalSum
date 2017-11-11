#ifndef RAIN_H
#define RAIN_H
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name    :rain.h
// Description  : 
// Author       :Gilberto Moreno 
// Mail         :gilberto.mor6945@mediadesign.school.nz
#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"
#include "Constraint.h"


#include "glm\glm/glm.hpp"
#include "glm\glm/gtc/matrix_transform.hpp"
#include "glm\glm/gtc/type_ptr.hpp"

class Camera;
class CRain
{
public:
	Camera* camera;
	CRain(int num_particles_width, int num_particles_height, Camera* _camera, GLuint prog);
	~CRain();
	//Vector holders
	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;
	//Pos variables
	glm::vec3 position;
	glm::vec3 velocity;

	int num_particles_width;
	int num_particles_height;

#pragma region Variables
	glm::vec3 scale;
	glm::vec3 angle;
	glm::vec3 rotationAxis;
	glm::vec3 color;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;

	int m_txtWind;//possibly
	float f_numparticles;
	float speed;
#pragma endregion

	void update();

	void render();

	std::vector<Particle> particles;
	std::vector<Constraint> constraints; // alle constraints between particles as part of this cloth
	std::vector<glm::vec3> vPositions;
#pragma region Setters&GEtters
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
	glm::vec3 getColor();
	glm::vec3 getScale();
	glm::vec3 getRotation();
	glm::vec3 getRotationAxis();

	void rotate(glm::vec3 axis);
#pragma endregion

	//Optional func
	Particle* getParticle(int x, int y) { return &particles[y*num_particles_width + x]; }
	void windForce(const glm::vec3 direction);


	void addWindForcesForTriangle(Particle * p1, Particle * p2, Particle * p3, const glm::vec3 direction);

	glm::vec3 calcTriangleNormal(Particle * p1, Particle * p2, Particle * p3);

private:

};


#endif // !RAIN_H
