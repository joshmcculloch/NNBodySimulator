/*
 * Particle.cpp
 *
 *  Created on: 15/04/2016
 *      Author: josh
 */

#include "Particle.h"

Particle::Particle(Eigen::Vector3f _location, Eigen::Vector3f _velocity):
	location(_location),
	velocity(_velocity)
{
}

Particle::~Particle() {
}

