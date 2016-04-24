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
public:
	Particle(Eigen::VectorXd _location, Eigen::VectorXd _velocity, double _mass, double _density);
	virtual ~Particle();
	bool interact(Particle *p, double delta_time);
	void step(float delta_time);
	Eigen::VectorXd location;
	Eigen::VectorXd velocity;
	bool active;
	double density;
	double mass;
	double radius;
};

#endif /* PARTICLE_H_ */
