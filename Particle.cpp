/*
 * Particle.cpp
 *
 *  Created on: 15/04/2016
 *      Author: josh
 */

#include "Particle.h"
#include "constants.h"

Particle::Particle(Eigen::Vector3f _location, Eigen::Vector3f _velocity, float _mass):
	location(_location),
	velocity(_velocity),
	mass(_mass)
{
}

Particle::~Particle() {
}

void
Particle::interact(Particle *p) {
	Eigen::Vector3f forceVector = p->location - location;
	float distance = forceVector.norm();
	forceVector.normalize();
	float force = G * (mass*p->mass) / (distance * distance);
	forceVector *= force;
	forceVector /= mass;
	velocity += forceVector;
}

void
Particle::step() {
	location += velocity;
}
