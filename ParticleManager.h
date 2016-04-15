/*
 * ParticleManager.h
 *
 *  Created on: 15/04/2016
 *      Author: josh
 */

#ifndef PARTICLEMANAGER_H_
#define PARTICLEMANAGER_H_

#include <vector>
#include "Particle.h"

class ParticleManager {
public:
	ParticleManager();
	virtual ~ParticleManager();
	void addParticle(Particle *p);
	std::vector<Particle*> getParticles(Particle *p);
	std::vector<Particle*> particles;
};

#endif /* PARTICLEMANAGER_H_ */
