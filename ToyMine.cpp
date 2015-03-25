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
	if (!isDetontated)
	{
		// TODO: code to begin the explosion animation/simulation
		tyga::debugLog("ToyMine::trigger: toy should explode now");

		tyga::Vector3 source_position = utilAyre::GetPos(this->Actor()->Transformation());
		triggerStart = tyga::BasicWorldClock::CurrentTime();

		int particlesNeeded = utilAyre::RandomScalar(100, 250);

		float lifespanLimit = utilAyre::RandomScalar(0.8f, 1.5f);
		float forceLimit = (float)utilAyre::RandomScalar(40, 50);

		for (int i = 0; i < particlesNeeded; i++)
		{
			tyga::Vector3 source_direction = utilAyre::RandomDirVecSphere(); //randomised direction vector using schochastic properties
			//Generate random force under forceLimit
			tyga::Vector3 thisForce = forceLimit * source_direction;

#ifdef _DEBUG
			//std::cout << "this particle force is " << std::to_string(thisForce.x) + " " + std::to_string(thisForce.y) + " " + std::to_string(thisForce.z) << std::endl;
#endif
			auto tempP = particle_system.lock();

			tempP->GetPoolPtr()->AddParticleToPool(source_position, source_direction, thisForce, lifespanLimit, triggerStart); //add this particle to the living pool
		}
	}

	isDetontated = true;
}

void ToyMine::
actorDidEnterWorld(std::shared_ptr<tyga::Actor> actor)
{
    auto world = tyga::ActorWorld::defaultWorld();
    auto graphics = tyga::GraphicsCentre::defaultCentre();
    auto physics = MyPhysicsCentre::defaultCentre();

    auto graphics_model = graphics->newModel();
    graphics_model->material = graphics->newMaterial();
    graphics_model->material->colour = tyga::Vector3(1, 0.33f, 0);
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

	if (triggerStart < current_time && waitingToDie)
		trigger();
    // HINT: once the toy has exploded and there is no visible traces left
    //       then call this->removeFromWorld() to free the memory
	if (isDetontated)
		this->removeFromWorld();
}
