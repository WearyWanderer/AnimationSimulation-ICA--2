#include "MyParticleSystem.hpp"

std::string MyParticleSystem::graphicsSpriteTexture() const
{
	return "tex1.png";
}

int MyParticleSystem::graphicsSpriteVertexCount() const
{
	// NB: you may need to adjust this count if you keep dead particles
	return particles.size();
}

void MyParticleSystem::graphicsSpriteGenerate(tyga::GraphicsSpriteVertex vertex_array[]) const
{
	// NB: you may need to adjust this if you want to control the sprite look
	for (unsigned int i = 0; i<particles.size(); ++i) 
	{
		vertex_array[i].position = particles[i].position;
		vertex_array[i].size = 0.5f;
		vertex_array[i].colour = tyga::Vector3(1, 1, 1);
		vertex_array[i].alpha = 1.f;
		vertex_array[i].rotation = 0.f; // NB: has no effect in basic renderer
	}
}