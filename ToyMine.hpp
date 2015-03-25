#pragma once

#include <tyga/ActorDelegate.hpp>
#include "MyPhysicsCentre.hpp"
#include "MyParticleSystem.hpp"

class ToyMine : public tyga::ActorDelegate
{
public:

    ToyMine();

    void
    reset(tyga::Vector3 position, float mass);

    void
    applyForce(tyga::Vector3 force);

    void
    trigger();

	inline void SetParticlePtr(std::weak_ptr<ParticlePoolInstance> w_ptr){ particle_system = w_ptr; }
private:

    virtual void
    actorDidEnterWorld(std::shared_ptr<tyga::Actor> actor) override;

    virtual void
    actorWillLeaveWorld(std::shared_ptr<tyga::Actor> actor) override;

    virtual void
    actorClockTick(std::shared_ptr<tyga::Actor> actor) override;

    std::shared_ptr<PhysicsSphere> physics_model_;
	std::weak_ptr<ParticlePoolInstance> particle_system;

	bool isDetontated = false;
	bool waitingToDie = false;
	float triggerStart;
	std::minstd_rand rand;
};
