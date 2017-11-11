#include "Rparticle.h"

RParticle::RParticle(glm::vec3 origin, glm::vec3 pos, Camera * _camera) : acceleration(glm::vec3(0, 0, 0)), mass(1), m_bmovable(true), accumulated_normal(glm::vec3(0, 0, 0))
{
	m_Origin = pos;
	_pos = pos;
	old_pos = pos;

	camera = _camera;
	f_elapsedTime = randFloat(0.015f, 0.025f);
}

RParticle::~RParticle()
{

}

void RParticle::update(float dt) {

	//glm::vec3 temp = _pos;
	//_pos = _pos + (_pos - old_pos)*(1.0f - DAMPING) + velocity;
	// acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
	this->velocity.y += -0.2 * .0167f;
	this->_pos += velocity;
	this->f_elapsedTime -= .000167;
	if (this->f_elapsedTime <= 0.0f)
	{
		this->_pos = this->m_Origin;
		this->velocity = glm::vec3(0.0f, randFloat(0.05f, 0.1f)*-1.0f, 0.0f);
		this->f_elapsedTime = randFloat(0.015f, 0.025f);
		this->cameraDist = glm::distance(this->camera->getPosition(), this->_pos);
	}
}

glm::vec3 & RParticle::getPos()
{
	return this->_pos;
}

float RParticle::getDistance()
{
	return this->cameraDist;
}



