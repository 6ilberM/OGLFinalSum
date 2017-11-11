#ifndef RPARTICLE_H
#define RPARTICLE_H
#include <iostream>
#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"
#include <random>

using namespace std;

class Camera;
static float randFloat(float min, float max)
{
	float r = (float)rand() / (double)RAND_MAX; return min + r * (max - min);
}

class RParticle
{
public:
	Camera* camera;
	// can the particle move or not ?
	bool m_bmovable;

	float mass;
	glm::vec3 m_Origin;

	glm::vec3 _pos;
	glm::vec3  old_pos;
	glm::vec3  velocity;
	glm::vec3  acceleration;
	glm::vec3  accumulated_normal;

	//Scalar
	float f_speed;
	//how long
	float f_elapsedTime;

	RParticle(glm::vec3 pos);
	~RParticle();
	void update(float dt);

	//void addForce(glm::vec3 f);

	//glm::vec3& getPos();

	//void resetAcceleration();

	//void offsetPos(const glm::vec3 v);

	//void makeUnmovable();
	//void makeMovabble();
	//void addToNormal(glm::vec3 normal);

	//// notice, the normal is not unit length
	//glm::vec3& getNormal();
	//void reset();
	//void resetNormal();
	//void limitnegy();
	//void ShiftStatic(float _scale);
};
#endif // !RPARTICLE_H
