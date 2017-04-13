#include <SFML/Graphics.hpp>
#include "ParticleSystem.h"
#include "TexturedParticleSystem.h"
#include "RainParticleSystem.h"
#include "TrailParticleSystem.h"
#include <iostream>

using namespace std;

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 256), "Particles");

	// create the particle system
	TrailParticleSystem mouseParticles(10);
	TexturedParticleSystem staticParticles1(10, sf::Triangles, "cooltexture.png", sf::Vector2f(10, 10));
	ParticleSystem staticParticles2(100);
	RainParticleSystem staticParticles3(100, sf::Lines, "", sf::Vector2f(0, 0));
	TexturedParticleSystem staticParticles4(10, sf::Quads, "snowflake.png", sf::Vector2f(225, 225));

	// set emitter
	staticParticles1.setEmitter(sf::Vector2f(100.f, 100.f));
	staticParticles2.setEmitter(sf::Vector2f(200.f, 100.f));
	staticParticles3.setEmitter(sf::Vector2f(300.f, 50.f));
	staticParticles4.setEmitter(sf::Vector2f(100.f, 100.f));

	// create a clock to track the elapsed time
	sf::Clock clock;

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			// handle keyboard events
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				mouseParticles.increaseParticleCount();
				staticParticles1.increaseParticleCount();
				staticParticles2.increaseParticleCount();
				staticParticles3.increaseParticleCount();
				staticParticles4.increaseParticleCount();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				mouseParticles.decreaseParticleCount();
				staticParticles1.decreaseParticleCount();
				staticParticles2.decreaseParticleCount();
				staticParticles3.decreaseParticleCount();
				staticParticles4.decreaseParticleCount();
			}
		}

		// make the particle system emitter follow the mouse
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		mouseParticles.setEmitter(window.mapPixelToCoords(mouse));

		// update it
		sf::Time elapsed = clock.restart();
		mouseParticles.update(elapsed);
		staticParticles1.update(elapsed);
		staticParticles2.update(elapsed);
		staticParticles3.update(elapsed);
		staticParticles4.update(elapsed);

		// draw it
		window.clear();
		window.draw(mouseParticles);
		window.draw(staticParticles1);
		window.draw(staticParticles2);
		window.draw(staticParticles3);
		window.draw(staticParticles4);
		window.display();
	}

	return 0;
}