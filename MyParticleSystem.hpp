#pragma once

#include <tyga/ActorDelegate.hpp>
#include <tyga\ActorWorld.hpp>
#include <tyga\GraphicsCentre.hpp>
#include <vector>

class MyParticleSystem : public tyga::GraphicsSpriteDelegate,
						 public std::enable_shared_from_this<MyParticleSystem>
{
public:

	struct Particle 
	{
		tyga::Vector3 position;
		tyga::Vector3 velocity;

	};

	std::string graphicsSpriteTexture() const override;

	inline std::shared_ptr<tyga::GraphicsSpriteDelegate> getSprite(){ return spriteImage; }

	int graphicsSpriteVertexCount() const override;

	void graphicsSpriteGenerate(tyga::GraphicsSpriteVertex vertex_array[]) const override;

	std::vector<Particle> particles;
	int particleQuantity = 10000;

private:

	std::shared_ptr<tyga::GraphicsSpriteDelegate> spriteImage;

};