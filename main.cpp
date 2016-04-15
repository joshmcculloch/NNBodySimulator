/*
 * main.cpp
 *
 *  Created on: 15/04/2016
 *      Author: josh
 */
#include <iostream>
#include <eigen3/Eigen/Eigen>
#include "Particle.h"
#include "ParticleManager.h"



int main(void)
{
	ParticleManager pManager = ParticleManager();
	for(unsigned int i=0; i<100; i++) {
		pManager.addParticle(new Particle(Eigen::Vector3f(0,0,0), Eigen::Vector3f(0,0,0)));
	}

	for(unsigned int i=0; i<pManager.particles.size(); i++) {
		std::vector<Particle*> neigbours = pManager.getParticles(pManager.particles[i]);
		for(unsigned int j=0; j<neigbours.size(); j++) {

		}
	}
    return 0;
}



