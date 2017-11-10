#ifndef CLOTH_H
#define CLOTH_H
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name    :cloth.h
// Description    : class declaration of manager for cloth
// Author        :Gilberto Moreno & Gibson Hickey
// Mail            :gilberto.mor6945@mediadesign.school.nz
#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"
#include "Constraint.h"


#include "glm\glm/glm.hpp"
#include "glm\glm/gtc/matrix_transform.hpp"
#include "glm\glm/gtc/type_ptr.hpp"

using namespace std;

class Camera;
class Ccloth
{
public:
	Camera* camera;
	// can the particle move or not ?

	std::vector<VertexFormat>vertices;
	std::vector<GLuint>indices;

	glm::vec3 position;

	glm::vec3 velocity;

	//Legacy

	glm::vec3 scale;
	glm::vec3 angle;
	glm::vec3 rotationAxis;
	glm::vec3 color;
	int m_hooks;
	int m_txtHooks;
	int m_txtWind;
	int m_txtSize;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint texture;
	GLuint program;

	float speed;
	//old constructor
	//Ccloth(Camera* _camera, GLuint prog);
	~Ccloth();

	void update(unsigned char keyState[255]);

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

	int SetTextState(int _txtVar, int _txtState);
	int GetTextState(int _txtVar);

	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getRotation();
	glm::vec3 getRotationAxis();
	void HoldaParticle(float x, float y, float lastx, float lasty);
	glm::vec3 getColor();
	//secondary edition

	void moveUp();
	void moveLeft();
	void moveRight();
	void moveDown();

	void moveForward();

	void moveBackward();

	void rotate(glm::vec3 axis);

#pragma region tuto.h
	int num_particles_width; // number of particles in "width" direction
	int num_particles_height; // number of particles in "height" direction
							  // total number of particles is num_particles_width*num_particles_height

	std::vector<Particle> particles; // all particles that are part of this cloth
	std::vector<Constraint> constraints; // alle constraints between particles as part of this cloth
	//new Gilbertostuff
	std::vector<glm::vec3> vPositions;
	float f_numparticles;
	Particle* getParticle(int x, int y) { return &particles[y*num_particles_width + x]; }
	void makeConstraint(Particle *p1, Particle *p2);

	void shredConstraint(float _epx, float _epy);


	glm::vec3 calcTriangleNormal(Particle *p1, Particle *p2, Particle *p3);


	void addWindForcesForTriangle(Particle *p1, Particle *p2, Particle *p3, const 	glm::vec3 direction);


	void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const glm::vec3 color);


	//Our Constructor
	Ccloth(float width, float height, int num_particles_width, int num_particles_height, Camera* _camera, GLuint prog,int hooks);


	void drawShaded();

	void drawShadedtwo();

	/* this is an important methods where the time is progressed one time step for the entire cloth.
	This includes calling satisfyConstraint() for every constraint, and calling timeStep() for all particles
	*/
	void timeStep();


	/* used to add gravity (or any other arbitrary vector) to all particles*/
	void addForce(const glm::vec3 direction);


	/* used to add wind forces to all particles, is added for each triangle since the final force is proportional to the triangle area as seen from the wind direction*/
	void windForce(const glm::vec3 direction);


	/* used to detect and resolve the collision of the cloth with the ball.
	This is based on a very simples scheme where the position of each particle is simply compared to the sphere and corrected.
	This also means that the sphere can "slip through" if the ball is small enough compared to the distance in the grid bewteen particles
	*/
	void ballCollision(const glm::vec3 center, const float radius);

	void ClothCollision();

	//NULL
	void doFrame()
	{

	}




#pragma endregion

};


#endif // !CLOTH_H
