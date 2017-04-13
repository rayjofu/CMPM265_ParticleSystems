#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <string>

class RainParticleSystem : public sf::Drawable, public sf::Transformable
{
public:

	RainParticleSystem(unsigned int count, sf::PrimitiveType shape, std::string texture_filename, sf::Vector2f texture_size);

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
		sf::VertexArray vertices;
	};

	void resetParticle(std::size_t index);

	std::vector<Particle> m_particles;
	sf::Time m_lifetime;
	sf::Vector2f m_emitter;
	unsigned int m_count;
	sf::PrimitiveType m_shape;
	sf::Texture m_texture;
	sf::Vector2f m_texture_size;
};