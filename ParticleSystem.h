#pragma once

#include <tyga/ActorDelegate.hpp>
#include <vector>

class Particle : public tyga::ActorDelegate
{


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

private:
	ParticleSystem();

	// technique of deleting the methods we don't want.
	ParticleSystem(ParticleSystem const&) = delete;
	void operator=(ParticleSystem const&) = delete;


};