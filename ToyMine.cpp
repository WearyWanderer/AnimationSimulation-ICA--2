#include "ToyMine.hpp"
#include "MyUtils.hpp"
#include <tyga/Actor.hpp>
#include <tyga/BasicWorldClock.hpp>
#include <tyga/Math.hpp>
#include <tyga/ActorWorld.hpp>
#include <tyga/GraphicsCentre.hpp>
#include <tyga/Log.hpp>

ToyMine::
ToyMine()
{
}

void ToyMine::
reset(tyga::Vector3 position, float mass)
{
    // NB: this method should not need changing

    const auto& p = position;
    auto xform = tyga::Matrix4x4(    1,   0,   0,  0,
                                     0,   1,   0,  0,
                                     0,   0,   1,  0,
                                   p.x, p.y, p.z,  1);
    Actor()->setTransformation(xform);
    physics_model_->mass = mass;
}

void ToyMine::
applyForce(tyga::Vector3 force)
{
    // NB: this method should not need changing

    physics_model_->force += force;
    physics_model_->velocity = tyga::Vector3(0,0,0);
}

void ToyMine::
trigger()
{
	GenerateExplosion(this->particleType);

	isDetontated = true;
}

void ToyMine::
GenerateExplosion(int explosionType)
{
	switch (explosionType) //based on the type of mine instantiated, lets have some fun and create two different kinds of particle effect
	{
	case 1:
		ParticleAllocExplosion();
		break;
	case 2:
		ParticleAllocSmoke();
		break;
	default:
		break;
	}
	
}

void ToyMine::
ParticleAllocExplosion()
{
	if (!isDetontated)
	{
		tyga::Vector3 source_position = utilAyre::GetPos(this->Actor()->Transformation()); //get initial position
		triggerStart = tyga::BasicWorldClock::CurrentTime(); //time the explosion begins

		int particlesNeeded = utilAyre::RandomScalar(100, 175); //random engine generates number of particles in this explosion

		float lifespanLimit = utilAyre::RandomScalar(0.8f, 1.5f); //lifespan of the particles for this mine
		float forceLimit = (float)utilAyre::RandomScalar(40, 50); //force outward from the explosion

		for (int i = 0; i < particlesNeeded; i++) //for each particle, loop through and generate animation
		{
			tyga::Vector3 source_direction = utilAyre::RandomDirVecSphere(); //randomised direction vector using schochastic properties

			//Generate random force under forceLimit
			tyga::Vector3 thisForce = forceLimit * source_direction;

#ifdef _DEBUG
			//std::cout << "this particle force is " << std::to_string(thisForce.x) + " " + std::to_string(thisForce.y) + " " + std::to_string(thisForce.z) << std::endl;
#endif
			auto tempP = particle_system.lock(); //access the particle pool

			tempP->GetPoolPtr()->AddParticleToPool(source_position, source_direction, thisForce, lifespanLimit, triggerStart); //add this particle to the living pool from cold storage
		}
	}
}

void ToyMine::
ParticleAllocSmoke()
{
	if (!isDetontated)
	{
		tyga::Vector3 source_position = utilAyre::GetPos(this->Actor()->Transformation()); //get initial position
		triggerStart = tyga::BasicWorldClock::CurrentTime(); //time the explosion begins

		int particlesNeeded = utilAyre::RandomScalar(60, 90); //random engine generates number of particles in this explosion

		for (int i = 0; i < particlesNeeded; i++) //for each particle, loop through and generate animation
		{
			float lifespanLimit = utilAyre::RandomScalar(0.4f, 2.4f); //lifespan of the particles for this mine
			float forceLimit = (float)utilAyre::RandomScalar(10, 80); //force outward from the explosion
			tyga::Vector3 source_direction = utilAyre::RandomDirVecSphere(); //randomised direction vector using schochastic properties
			//Generate random force under forceLimit
			tyga::Vector3 thisForce = forceLimit * source_direction;

#ifdef _DEBUG
			//std::cout << "this particle force is " << std::to_string(thisForce.x) + " " + std::to_string(thisForce.y) + " " + std::to_string(thisForce.z) << std::endl;
#endif
			auto tempP = particle_system.lock(); //access the particle pool

			tempP->GetPoolPtr()->AddParticleToPool(source_position, source_direction, thisForce, lifespanLimit, triggerStart, tyga::Vector3(0.1f,0.1f,0.5f)); //add this particle to the living pool from cold storage
		}
	}
}

void ToyMine::
actorDidEnterWorld(std::shared_ptr<tyga::Actor> actor)
{
    auto world = tyga::ActorWorld::defaultWorld();
    auto graphics = tyga::GraphicsCentre::defaultCentre();
    auto physics = MyPhysicsCentre::defaultCentre();

    auto graphics_model = graphics->newModel();
    graphics_model->material = graphics->newMaterial();
	switch (particleType)
	{
	case 1:
		graphics_model->material->colour = tyga::Vector3(1, 0.33f, 0);
		break;
	case 2:
		graphics_model->material->colour = tyga::Vector3(0.5f, 0.73f, 0);
		break;
	default:
		break;
	}
    graphics_model->mesh = graphics->newMeshWithIdentifier("sphere");
    graphics_model->xform = tyga::Matrix4x4( 0.5f,    0,    0,   0,
                                                0, 0.5f,    0,   0,
                                                0,    0, 0.5f,   0,
                                                0,    0,    0,   1);

    auto physics_model = physics->newSphere();
    physics_model->radius = 0.25f;
    physics_model->mass = 1.f;
    physics_model_ = physics_model;

    actor->attachComponent(graphics_model);
    actor->attachComponent(physics_model);
    world->addActor(actor);
}

void ToyMine::
actorWillLeaveWorld(std::shared_ptr<tyga::Actor> actor)
{
}

void ToyMine::
actorClockTick(std::shared_ptr<tyga::Actor> actor)
{
	float current_time = tyga::BasicWorldClock::CurrentTime();

	if (physics_model_->collisionOccured)
	{
		
		std::uniform_real_distribution<float> x_rand(-0.2f, 0.2f);
		std::uniform_real_distribution<float> z_rand(-0.2f, 0.2f);
		std::uniform_real_distribution<float> t_rand(1, 3);
		auto dir = tyga::unit(tyga::Vector3(x_rand(rand), 1, z_rand(rand)));
		auto force = 600 * dir;
		triggerStart = current_time + t_rand(rand);
		applyForce(force);
		physics_model_->collisionOccured = false;
		waitingToDie = true;
	}

	if (triggerStart < current_time && waitingToDie) //if the mine has been collided with and the countdown to explosion has ended
		trigger();
    // HINT: once the toy has exploded and there is no visible traces left
    //       then call this->removeFromWorld() to free the memory
	if (isDetontated)
		this->removeFromWorld();
}
