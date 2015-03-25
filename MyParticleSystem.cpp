#include "MyParticleSystem.hpp"

MyParticleSystem::Particle::Particle(){
	
	//hidden particles off screen just in case
	position = tyga::Vector3(0, -400, 0);
	velocity = tyga::Vector3(0, 0, 0);
	force = tyga::Vector3(0, 0, 0);
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
		vertex_array[i].size = particles[i].particleSize;
		vertex_array[i].colour = particles[i].particleCol;
		vertex_array[i].alpha = 1.f;
		vertex_array[i].rotation = 0.f; // NB: has no effect in basic renderer

	}

}

void MyParticleSystem::SimulateLivingParticles()
{
	const float time = tyga::BasicWorldClock::CurrentTime();
	const float deltaTime = tyga::BasicWorldClock::CurrentTickInterval();

	for (unsigned int i = 0; i < currentLivingParticles; ++i)
	{
		//Pure acceleration should ge got similar to the physics manner, but without using gravity
		tyga::Vector3 acceleration = particles[i].force / 0.2f;

		//std::cout << "this particle force is " << std::to_string(particles[i].force.x) + " " + std::to_string(particles[i].force.y) + " " + std::to_string(particles[i].force.z) << std::endl;

		tyga::Vector3 tempEulerMove = utilAyre::EulerVec(particles[i].position, deltaTime, particles[i].velocity);

		//std::cout << "velocity is " << std::to_string(tempEulerMove.x) + " " + std::to_string(tempEulerMove.y) + " " + std::to_string(tempEulerMove.z) << std::endl;

		//euler vec to get current tick position
		particles[i].position = tempEulerMove;

		particles[i].velocity = utilAyre::EulerVec(particles[i].velocity, deltaTime, acceleration); //this was my original attemp, results are still the same

		//velocity using similar principle but  with acceleration as derivative
		particles[i].force = tyga::Vector3(0, 0, 0);

		particles[i].particleSize = utilAyre::LinStep(particles[i].timeSpawned + particles[i].totalLife, particles[i].timeSpawned, time);
		particles[i].particleCol = utilAyre::Lerp(particles[i].particleCol, tyga::Vector3(1.0f, 1.0f, 0.0f), time);

		if (particles[i].particleSize <= 0.0f)
			ReapParticle(i);
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

ParticlePoolInstance::ParticlePoolInstance()
{

}

void ParticlePoolInstance::actorDidEnterWorld(std::shared_ptr<tyga::Actor> actor)
{
	pool = std::make_shared<MyParticleSystem>();
	auto graphics = tyga::GraphicsCentre::defaultCentre();
	graphics_sprite = graphics->newSpriteWithDelegate(pool);
}

void ParticlePoolInstance::actorWillLeaveWorld(std::shared_ptr<tyga::Actor> actor)
{

}

void ParticlePoolInstance::actorClockTick(std::shared_ptr<tyga::Actor> actor)
{
	pool->SimulateLivingParticles();
}

