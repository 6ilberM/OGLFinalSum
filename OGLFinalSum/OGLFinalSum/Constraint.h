#ifndef CONSTRAINT_H
#define CONSTRAINT_H
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name    :constraint.h
// Description    : constraint delcaration and Creation
// Author        :Gilberto Moreno & Gibson Hickey
// Mail            :gilberto.mor6945@mediadesign.school.nz
#include "ShaderLoader.h"
#include "Utils.h"
#include <vector>
#include <time.h>  
#include "camera.h"
#include "Particle.h"

class Constraint
{
private:
	float rest_distance; // the length between particle p1 and p2 in rest configuration

public:
	Particle *p1, *p2; // the two particles that are connected through this constraint

	//constructor
	Constraint(Particle *p1, Particle *p2) : p1(p1), p2(p2)
	{
		glm::vec3 vec = p1->getPos() - p2->getPos();
		rest_distance = glm::length(vec);
	}


	/* This is one of the important methods, where a single constraint between two particles p1 and p2 is solved
	the method is called by Cloth.time_step() many times per frame*/
	void satisfyConstraint();

};
#endif // !CONSTRAINT_H
