#pragma once

#include <tyga/ActorDelegate.hpp>
#include <tyga\ActorWorld.hpp>
#include <tyga\GraphicsCentre.hpp>
#include <tyga/BasicWorldClock.hpp>
#include <vector>
#include <iostream>
#include "MyUtils.hpp"

class MyParticleSystem : public tyga::GraphicsSpriteDelegate,
						 public std::enable_shared_from_this<MyParticleSystem>
{
public:

	struct Particle 
	{
		tyga::Vector3 position;
		tyga::Vector3 velocity;
		tyga::Vector3 force;
		float totalLife;
		float timeSpawned;
		bool living = false;

		//additional visual effects
		float particleSize = 0.25f;
		tyga::Vector3 particleCol = tyga::Vector3(1.0f, 0.0f, 0.0f);

		inline Particle();
		inline Particle(tyga::Vector3 pos, tyga::Vector3 vel, tyga::Vector3 emit_dir, float life, float initTime) : position(pos), velocity(vel), force(emit_dir), totalLife(life), timeSpawned(initTime){ living = true; };
	};

	MyParticleSystem();

	std::string graphicsSpriteTexture() const override;

	inline std::shared_ptr<tyga::GraphicsSpriteDelegate> getSprite(){ return spriteImage; }

	int graphicsSpriteVertexCount() const override;

	void graphicsSpriteGenerate(tyga::GraphicsSpriteVertex vertex_array[]) const override;

	void SimulateLivingParticles();

	void AddParticleToPool(tyga::Vector3 emitter_position, tyga::Vector3 emit_direction, tyga::Vector3 force, float lifetime, float timeSpawned);

	void AddParticleToPool(tyga::Vector3 emitter_position, tyga::Vector3 emit_direction, tyga::Vector3 force, float lifetime, float timeSpawned, tyga::Vector3 colour); //overloaded version that gives a new particle colour, other than generic explosion

	void ReapParticle(int particlePos);

	int GetNextFreePosition();

	std::vector<Particle> particles;

	int currentLivingParticles = 0;

private:

	std::shared_ptr<tyga::GraphicsSpriteDelegate> spriteImage;

};

/*I've decided to use the ActorDelegate system to utilise tyrones actorClockTick calls and similar things, 
to create one singular contiguous block of memory to manage all the particles.
A instance of this is held within the MyDemo, and is linked to by weak_ptr within mines
*/
class ParticlePoolInstance : public tyga::ActorDelegate 
{
public:

	ParticlePoolInstance();

	inline std::shared_ptr<MyParticleSystem> GetPoolPtr(){ return pool; };

private:

	virtual void
		actorDidEnterWorld(std::shared_ptr<tyga::Actor> actor) override;

	virtual void
		actorWillLeaveWorld(std::shared_ptr<tyga::Actor> actor) override;

	virtual void
		actorClockTick(std::shared_ptr<tyga::Actor> actor) override;

	std::shared_ptr<MyParticleSystem> pool;
	std::shared_ptr<tyga::GraphicsSprite> graphics_sprite;

};