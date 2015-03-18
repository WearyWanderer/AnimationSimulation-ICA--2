#include "MyParticleSystem.hpp"

MyParticleSystem::Particle::Particle(){
	
	//hidden particles off screen just in case
	position = tyga::Vector3(0, -400, 0);
	velocity = tyga::Vector3(0, 0, 0);
	emit_direction = tyga::Vector3(0, 0, 0);
	living = false;
}

MyParticleSystem::MyParticleSystem()
{
	particles.resize(10000);
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
	for (unsigned int i = 0; i < currentLivingParticles; ++i)
	{
		vertex_array[i].position = particles[i].position;
		vertex_array[i].size = 0.5f;
		vertex_array[i].colour = tyga::Vector3(1, 1, 1);
		vertex_array[i].alpha = 1.f;
		vertex_array[i].rotation = 0.f; // NB: has no effect in basic renderer
	}
}

void MyParticleSystem::SimulateLivingParticles()
{
	float deltaTime = tyga::BasicWorldClock::CurrentTickInterval();
	for (unsigned int i = 0; i < currentLivingParticles; ++i)
	{
		//Pure acceleration should ge got similar to the physics manner, but without using gravity
		tyga::Vector3 acceleration = (particles[i].emit_direction) / 0.2f;

		//euler vec to get current tick position
		particles[i].position = utilAyre::EulerVec(particles[i].position, deltaTime, particles[i].velocity);
		//velocity using similar principle but  with acceleration as derivative
		particles[i].velocity = utilAyre::EulerVec(particles[i].velocity, deltaTime, acceleration);
	}
}

void MyParticleSystem::AddParticleToPool(tyga::Vector3 emitter_position, tyga::Vector3 emit_direction, tyga::Vector3 force, float lifetime, float timeSpawned)
{
	particles[currentLivingParticles] = Particle(emitter_position, emit_direction, force, lifetime, timeSpawned);

	currentLivingParticles++;
}

void MyParticleSystem::ReapParticle(int particlePos)
{
	std::iter_swap(particles.begin() + particlePos, particles.begin() + currentLivingParticles); //swap this particle to be the last living particle

	particles[currentLivingParticles] = Particle();

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
