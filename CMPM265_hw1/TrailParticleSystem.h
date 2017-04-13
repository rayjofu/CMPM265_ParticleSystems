#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

class TrailParticleSystem : public sf::Drawable, public sf::Transformable
{
public:

	TrailParticleSystem(unsigned int count);

	void setEmitter(sf::Vector2f position);
	void increaseParticleCount();
	void decreaseParticleCount();
	void update(sf::Time elapsed);
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time lifetime;
		sf::VertexArray trail;
	};

	void resetParticle(std::size_t index);

	std::vector<Particle> m_particles;
	sf::VertexArray m_vertices;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
	unsigned int m_count;
};