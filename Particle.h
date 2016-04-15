/*
 * Particle.h
 *
 *  Created on: 15/04/2016
 *      Author: josh
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <eigen3/Eigen/Eigen>

class Particle {
	Eigen::Vector3f location;
	Eigen::Vector3f velocity;
public:
	Particle(Eigen::Vector3f _location, Eigen::Vector3f _velocity);
	virtual ~Particle();
	void interact(Particle &particle);
};

#endif /* PARTICLE_H_ */
