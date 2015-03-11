#include "ParticleSystem.hpp"


ParticleSystem::ParticleSystem()
{
	for (int i = 0; i <= particleQuantity; i++) //push 4000 particles into pool
	{
		Particle newParticle(i);
		particlePool.push_back(i);
	}
}


ParticleSystem::~ParticleSystem()
{

}

