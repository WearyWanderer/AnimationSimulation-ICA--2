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
	particles.resize(10000); //upon startup, we create a pool of 10,000 particles max
}

std::string MyParticleSystem::graphicsSpriteTexture() const
{
	return "tex1.png";
}

int MyParticleSystem::graphicsSpriteVertexCount() const
{
	// NB: you may need to adjust this count if you keep dead particles
	return currentLivingParticles; //return only the living particles count
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

#ifdef NDEBUG //when in release I used this simple position logic check to stop particles from exploding below the floor. Basic but just an additional bit of detail
		if (particles[i].position.y < -0.1f)
			{
				particles[i].position.y = 0.0f;
				if (particles[i].velocity.y < -0.1f)
				{
					particles[i].velocity.y = 0.f;
				}
				else
				{
					particles[i].velocity.y = abs(particles[i].velocity.y);
				}
		}
#endif

		//Pure acceleration should ge got similar to the physics manner, but without using gravity
		tyga::Vector3 acceleration = particles[i].force / 0.2f;

		//euler vec to get current tick position
		tyga::Vector3 tempEulerMove = utilAyre::EulerVec(particles[i].position, deltaTime, particles[i].velocity);
		particles[i].position = tempEulerMove;

		//velocity using similar principle but  with acceleration as derivative
		particles[i].velocity = utilAyre::EulerVec(particles[i].velocity, deltaTime, acceleration); //this was my original attemp, results are still the same [personal note, ignore]

		particles[i].force = tyga::Vector3(0, 0, 0);

		particles[i].particleSize = utilAyre::LinStep(particles[i].timeSpawned + particles[i].totalLife, particles[i].timeSpawned, time); //rather than both using one function or being linked together, I used linear step for size and lerp for colour to create an explosion-esque sparkly effect for particles
		particles[i].particleCol = utilAyre::Lerp(particles[i].particleCol, tyga::Vector3(1.0f, 1.0f, 0.0f), time); //this col change no longer really is used, as we just use a texture now

		if (particles[i].particleSize <= 0.0f) //if the particle's size is no longer visible, reap back into cold storage
			ReapParticle(i);
	}
}

void MyParticleSystem::AddParticleToPool(tyga::Vector3 emitter_position, tyga::Vector3 emit_direction, tyga::Vector3 force, float lifetime, float timeSpawned)
{
	if (currentLivingParticles < particles.size() - 1) //only if there is cold stored particles available, create particles
	{
		particles[currentLivingParticles] = Particle(emitter_position, emit_direction, force, lifetime, timeSpawned);
		currentLivingParticles++;
	}
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

