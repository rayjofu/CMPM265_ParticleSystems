#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include "TrailParticleSystem.h"
#include <iostream>

TrailParticleSystem::TrailParticleSystem(unsigned int count) :
	m_particles(count),
	m_vertices(sf::Points, count),
	m_lifetime(sf::seconds(3)),
	m_emitter(0, 0),
	m_count(count)
{
}

void TrailParticleSystem::setEmitter(sf::Vector2f position)
{
	m_emitter = position;
}

void TrailParticleSystem::update(sf::Time elapsed)
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

		// update the position of the corresponding vertex
		m_vertices[i].position += p.velocity * elapsed.asSeconds();

		// update the alpha (transparency) of the particle according to its lifetime
		float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
		m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);

		float distance = sqrt((m_vertices[i].position.x - m_emitter.x) * (m_vertices[i].position.x - m_emitter.x) + (m_vertices[i].position.y - m_emitter.y));
		if (distance >= 5.f) {
			p.trail[0].position = (m_vertices[i].position - m_emitter) * 0.9f + m_emitter;
			p.trail[0].color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
			p.trail[0].color.a = static_cast<sf::Uint8>(ratio * 255);
			p.trail[1].position = (m_vertices[i].position - m_emitter) * 0.6f + m_emitter;
			p.trail[1].color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
			p.trail[1].color.a = static_cast<sf::Uint8>(ratio * 255);
		}
	}
}

void TrailParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// our particles don't use a texture
	states.texture = NULL;

	// draw the vertex array
	target.draw(m_vertices, states);

	for (std::size_t i = 0; i < m_particles.size(); ++i)
	{
		target.draw(m_particles[i].trail, states);
	}
}

void TrailParticleSystem::resetParticle(std::size_t index)
{
	// give a random velocity and lifetime to the particle
	float angle = (std::rand() % 360) * 3.14f / 180.f;
	float speed = (std::rand() % 50) + 50.f;
	m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
	m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);
	m_particles[index].trail = sf::VertexArray(sf::Lines, 2);

	// reset the position of the corresponding vertex
	m_vertices[index].position = m_emitter;
}

void TrailParticleSystem::increaseParticleCount()
{
	m_count = m_count * 2;
	//	cout << m_count << endl;
	m_particles.resize(m_count);
	m_vertices.resize(m_count);
}

void TrailParticleSystem::decreaseParticleCount()
{
	if (m_count == 1)
	{
		return;
	}
	m_count = m_count / 2;
	//	cout << m_count << endl;
	m_particles.resize(m_count);
	m_vertices.resize(m_count);
}