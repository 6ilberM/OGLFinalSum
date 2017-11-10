#ifndef PARTICLE_H
#define PARTICLE_H
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name    :Particle.h
// Description    : ParticleCreation .h
// Author        :Gilberto Moreno & Gibson Hickey
// Mail            :gilberto.mor6945@mediadesign.school.nz

#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"
using namespace std;

class Camera;
class Particle
{
public:
	Camera* camera;
	// can the particle move or not ?
	bool m_bmovable;
	 glm::vec3 m_Origin;

	float mass; // the mass of the particle (is always 1 in this example)
	glm::vec3 _pos; //the current position of the particle in 3D space
	glm::vec3  old_pos; // the position of the particle in the previous time step, used as part of the verlet numerical integration scheme
	glm::vec3  acceleration; // a vector representing the current acceleration of the particle
	glm::vec3  accumulated_normal; // an accumulated normal (i.e. non normalized), used for OpenGL soft shading

public:
	Particle(glm::vec3 pos) : _pos(pos), old_pos(pos), acceleration(glm::vec3(0, 0, 0)), mass(1), m_bmovable(true), accumulated_normal(glm::vec3(0, 0, 0)) 
	{ 
		m_Origin = _pos; 
	}
	Particle() {}

	void addForce(glm::vec3 f);

	/* This is one of the important methods, where the time is progressed a single step size (TIME_STEPSIZE)
	The method is called by Cloth.time_step()
	Given the equation "force = mass * acceleration" the next position is found through verlet integration*/
	
	void timeStep();

	glm::vec3& getPos();

	void resetAcceleration();

	void offsetPos(const glm::vec3 v);

	void makeUnmovable();
	void makeMovabble();
	void addToNormal(glm::vec3 normal);

	// notice, the normal is not unit length
	glm::vec3& getNormal();
	void reset();
	void resetNormal();
	void limitnegy();
	void ShiftStatic(float _scale);
};
#endif // !PARTICLE_H
