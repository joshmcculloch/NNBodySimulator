/*
 * ParticleManager.cpp
 *
 *  Created on: 15/04/2016
 *      Author: josh
 */

#include "ParticleManager.h"

ParticleManager::ParticleManager() {
}

ParticleManager::~ParticleManager() {
}

void
ParticleManager::addParticle(Particle *p) {
	particles.push_back(p);
}

std::vector<Particle*>
ParticleManager::getParticles(Particle *p) {
	return particles;
}
