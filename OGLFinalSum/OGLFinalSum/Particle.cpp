#include "Particle.h"
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name    :Particle.cpp
// Description    : cpp creator for Particle 
// Author        :Gilberto Moreno & Gibson Hickey
// Mail            :gilberto.mor6945@mediadesign.school.nz
void Particle::addForce(glm::vec3 f)
{
	acceleration += f / mass;
}

void Particle::timeStep()
{
	if (m_bmovable)
	{
		glm::vec3 temp = _pos;
		_pos = _pos + (_pos - old_pos)*(1.0f - DAMPING) + acceleration*TSTEPSIZE;
		old_pos = temp;
		acceleration = glm::vec3(0, 0, 0); // acceleration is reset since it HAS been translated into a change in position (and implicitely into velocity)	
		limitnegy();
	}
}

glm::vec3 & Particle::getPos()
{
	return _pos;
}

void Particle::resetAcceleration()
{
	acceleration = glm::vec3(0, 0, 0);
}

void Particle::offsetPos(const glm::vec3 v)
{
	if (m_bmovable) _pos += v;
}

void Particle::makeUnmovable()
{
	m_bmovable = false;
}

void Particle::makeMovabble()
{
	m_bmovable = true;
}

void Particle::addToNormal(glm::vec3 normal)
{
	accumulated_normal += glm::normalize(normal);
}

glm::vec3 & Particle::getNormal()
{
	return accumulated_normal;
}

void Particle::reset()
{
	_pos = m_Origin;
}

void Particle::resetNormal()
{
	accumulated_normal = glm::vec3(0, 0, 0);
}
//fake real ground haha...
void Particle::limitnegy()
{
	if (_pos.y<-.2f)
	{
		_pos.y = -.2f;
	}
}

void Particle::ShiftStatic(float _scale)
{
	if (m_bmovable== false)
	{
		_pos.x = _pos.x-_pos.x*.04f;
	}
}
