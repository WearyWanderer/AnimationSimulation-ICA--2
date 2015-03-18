#include "MyParticleSystem.hpp"

MyParticleSystem::Particle::Particle(){
	//Ensure the particle data is empty
	position = tyga::Vector3(0, 200.f, 0);
	velocity = tyga::Vector3(0, 0, 0);
	emit_direction = tyga::Vector3(0, 0, 0);
	living = false;
}

MyParticleSystem::MyParticleSystem()
{
	particles.resize(100000);
}

std::string MyParticleSystem::graphicsSpriteTexture() const
{
	return "tex1.png";
}

int MyParticleSystem::graphicsSpriteVertexCount() const
{
	// NB: you may need to adjust this count if you keep dead particles
	return currentLivingParticles;
}

void MyParticleSystem::graphicsSpriteGenerate(tyga::GraphicsSpriteVertex vertex_array[]) const
{
	// NB: you may need to adjust this if you want to control the sprite look
	for (unsigned int i = 0; i<currentLivingParticles; ++i) 
	{
		vertex_array[i].position = particles[i].position;
		vertex_array[i].size = 0.5f;
		vertex_array[i].colour = tyga::Vector3(1, 1, 1);
		vertex_array[i].alpha = 1.f;
		vertex_array[i].rotation = 0.f; // NB: has no effect in basic renderer
	}
}

void MyParticleSystem::AddParticleToPool(tyga::Vector3 emitter_position, tyga::Vector3 emit_direction, tyga::Vector3 force, float lifetime, float timeSpawned)
{
	particles[currentLivingParticles] = Particle(emitter_position, emit_direction, force, lifetime, timeSpawned);

	currentLivingParticles++;
}

void MyParticleSystem::ReapParticle(int particlePos)
{
	std::iter_swap(particles.begin() + particlePos, particles.begin() + currentLivingParticles - 1); //swap this particle to be the last living particle


	
	currentLivingParticles--; //reduce the amount of living particles by one
}

int MyParticleSystem::GetNextFreePosition()
{
	int position = 0;

	for each (Particle thisPar in particles)
	{
		if (thisPar.living)
		{
			position++; //increment how far in the cold storage begins
		}
	}

	return position; //return the position we can access cold storage
}
