#include "Rparticle.h"

RParticle::RParticle(glm::vec3 pos) : _pos(pos), old_pos(pos), acceleration(glm::vec3(0, 0, 0)), mass(1), m_bmovable(true), accumulated_normal(glm::vec3(0, 0, 0))
{
	m_Origin = pos;
	f_elapsedTime = 0.0f;
}

RParticle::~RParticle()
{

}

void RParticle::update(float dt) {
	this->velocity.y += -0.2 * .0167f; this->_pos += velocity; this->f_elapsedTime -= .000167;
	if (this->f_elapsedTime <= 0.0f) { this->_pos = this->m_Origin; this->velocity = glm::vec3(randFloat(-0.015, 0.015), randFloat(0.05f, 0.1f), randFloat(-0.015, 0.015)); this->f_elapsedTime = randFloat(0.01f, 0.02f); }
}

