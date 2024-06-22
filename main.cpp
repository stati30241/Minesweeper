#include <SFML/Graphics.hpp>


int main() {
	sf::RenderWindow window{ sf::VideoMode{1200, 675}, "Test", sf::Style::Close };
	sf::Clock clock;

	while (window.isOpen()) {
		sf::Event sfmlEvent{};
		while (window.pollEvent(sfmlEvent)) {
			if (sfmlEvent.type == sf::Event::Closed) window.close();
		}

		float deltaTime = clock.restart().asSeconds();

		window.clear({ 50, 40, 80 });
		window.display();
	}

	return 0;
}
