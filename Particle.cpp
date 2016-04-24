/*
 * Particle.cpp
 *
 *  Created on: 15/04/2016
 *      Author: josh
 */

#include "Particle.h"
#include "constants.h"
#include <cmath>
#include <iostream>

Particle::Particle(Eigen::VectorXd _location, Eigen::VectorXd _velocity, double _mass, double _density):
	location(_location),
	velocity(_velocity),
	mass(_mass),
	density(_density)
{
	radius = 1;
	active = true;
}

Particle::~Particle() {
}

bool
Particle::interact(Particle *p, double delta_time) {
	Eigen::VectorXd forceVector = p->location - location;
	double distance = forceVector.norm();
	if (distance > (radius+p->radius)) {
		// Do gravity
		forceVector.normalize();
		double force = G * (mass*p->mass) / std::pow(distance,location.rows()-1) * delta_time;
		forceVector *= force;
		forceVector /= mass;
		velocity += forceVector;
		return false;
	} else {
		// Do collision
		double CR = 0.2;
		Eigen::VectorXd v1 = ((p->velocity-velocity)*CR*p->mass + velocity*mass + p->velocity*p->mass) / (mass+p->mass);
		Eigen::VectorXd v2 = ((velocity-p->velocity)*CR*mass + velocity*mass + p->velocity*p->mass) / (mass+p->mass);
		velocity = v1;
		p->velocity = v2;
		std::cout << velocity << std::endl;
		std::cout << "    -     " << velocity.dot(p->velocity) / velocity.norm() << "    " << (std::abs(velocity.dot(p->velocity) / velocity.norm() )-1) << std::endl;
		if ((std::abs(velocity.dot(p->velocity) / velocity.norm()) -1)  < 0.05) {
			velocity = (velocity*mass + p->velocity*p->mass) / (mass + p->mass);
			location = (location*mass + p->location*p->mass) / (mass + p->mass);
			mass += p->mass;
			p->active = false;
			std::cout << "Particles merged\n";
			return true;
		} else {
			return false;
		}
	}
}

void
Particle::step(float delta_time) {
	double volume = mass / density;
	int dimensions = location.rows();
	if (dimensions == 2) {
		radius = sqrt(volume / PI);
	}
	else if (dimensions == 3) {
		radius = cbrt(volume / (PI*PI/3.0f));
	}
	location += velocity*delta_time;
}
