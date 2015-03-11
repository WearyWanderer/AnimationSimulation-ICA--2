#pragma once

#include <tyga/ActorDelegate.hpp>
#include <tyga\GraphicsCentre.hpp>
#include <vector>

class Particle : public tyga::ActorDelegate
{
public:
	Particle(int id);

	inline void ReturnToPool(){ activeParticle = false; }
private:

	int id;
	bool activeParticle;
	
};

class ParticleSystem
{
public:
	~ParticleSystem();
	static ParticleSystem& getInstance()
	{
		static ParticleSystem instance;
		// Instantiated on first use.
		return instance;
	}

	Particle* GetAvailableSprite();
	inline std::vector<Particle>* GetParticlePool(){ return &particlePool; }


private:
	ParticleSystem();

	// technique of deleting the methods we don't want.
	ParticleSystem(ParticleSystem const&) = delete;
	void operator=(ParticleSystem const&) = delete;

	std::vector<Particle> particlePool;
	int particleQuantity = 10000;
};