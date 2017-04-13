#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include "TexturedParticleSystem.h"
#include <iostream>
#include <string>

TexturedParticleSystem::TexturedParticleSystem(unsigned int count, sf::PrimitiveType shape, std::string texture_filename, sf::Vector2f texture_size) :
	m_particles(count),
	m_lifetime(sf::seconds(3)),
	m_emitter(0, 0),
	m_count(count),
	m_shape(shape),
	m_texture_size(texture_size)
{
	if (!m_texture.loadFromFile(texture_filename))
	{
		std::cout << "failed to load from file" << std::endl;
	}
	
}

void TexturedParticleSystem::setEmitter(sf::Vector2f position)
{
	m_emitter = position;
}

void TexturedParticleSystem::update(sf::Time elapsed)
{
	for (std::size_t i = 0; i < m_particles.size(); ++i)
	{
		Particle& p = m_particles[i];
		// update the particle lifetime
		p.lifetime -= elapsed;

		// if the particle is dead, respawn it
		if (p.lifetime <= sf::Time::Zero)
		{
			resetParticle(i);
		}

		// adding gravity
//		p.velocity += sf::Vector2f(0, 0.98f);

		// update the position and alpha (transparency relative to its lifetime) of each vertex
		float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
		for (std::size_t j = 0; j < p.vertices.getVertexCount(); ++j)
		{
			p.vertices[j].position += p.velocity * elapsed.asSeconds();
//			p.vertices[j].texCoords += p.velocity * elapsed.asSeconds();
			p.vertices[j].color.a = static_cast<sf::Uint8>(ratio * 255);

			if (m_shape == sf::Quads)
			{
				
			}
		}
	}
}

void TexturedParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// our particles don't use a texture
	states.texture = &m_texture;

	// draw the vertex array
	for (std::size_t i = 0; i < m_particles.size(); ++i)
	{
//		target.draw(m_particles[i].vertices, states);
		target.draw(m_particles[i].vertices, &m_texture);
	}
}

void TexturedParticleSystem::resetParticle(std::size_t index)
{
	// give a random velocity and lifetime to the particle
	float angle = (std::rand() % 360) * 3.14f / 180.f;
	float speed = (std::rand() % 50) + 50.f;
	m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);

	m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

	// reset the position of the corresponding vertex
	if (m_shape == sf::Quads)
	{
		m_particles[index].vertices = sf::VertexArray(sf::Quads, 4);
		m_particles[index].vertices[0].position = m_emitter + sf::Vector2f(-10, -10);	// top left
		m_particles[index].vertices[0].texCoords = sf::Vector2f(0, 0);
		m_particles[index].vertices[1].position = m_emitter + sf::Vector2f(10, -10);	// top right
		m_particles[index].vertices[1].texCoords = sf::Vector2f(m_texture_size.x, 0);
		m_particles[index].vertices[2].position = m_emitter + sf::Vector2f(10, 10);		// bot right
		m_particles[index].vertices[2].texCoords = m_texture_size;
		m_particles[index].vertices[3].position = m_emitter + sf::Vector2f(-10, 10);	// bot left
		m_particles[index].vertices[3].texCoords = sf::Vector2f(0, m_texture_size.y);
	}
	else if (m_shape == sf::Triangles)
	{
		m_particles[index].vertices = sf::VertexArray(sf::Triangles, 3);
		m_particles[index].vertices[0].position = m_emitter + sf::Vector2f(0, -10);							// top
		m_particles[index].vertices[0].texCoords = sf::Vector2f(m_texture_size.x / 2, 0);
		m_particles[index].vertices[1].position = m_emitter + sf::Vector2f(-10 * sqrt(2), 10 * sqrt(2));	// bot right
		m_particles[index].vertices[1].texCoords = sf::Vector2f(0, m_texture_size.y);
		m_particles[index].vertices[2].position = m_emitter + sf::Vector2f(10 * sqrt(2), 10 * sqrt(2));		// bot left
		m_particles[index].vertices[2].texCoords = m_texture_size;
	}
}

void TexturedParticleSystem::increaseParticleCount()
{
	m_count = m_count * 2;
	//	cout << m_count << endl;
	m_particles.resize(m_count);
}

void TexturedParticleSystem::decreaseParticleCount()
{
	if (m_count == 1)
	{
		return;
	}
	m_count = m_count / 2;
	//	cout << m_count << endl;
	m_particles.resize(m_count);
}