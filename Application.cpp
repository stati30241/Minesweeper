#include "Application.h"


Application::Application() {
	initialize();
}


Application::~Application() {
	delete m_window;
}


void Application::initialize() {
	sf::VideoMode windowSize{ 1200u, 675u };
	sf::String windowTitle = "Minesweeper";
	sf::Uint32 windowStyle = sf::Style::Close;
	m_window = new sf::RenderWindow{ windowSize, windowTitle, windowStyle };

	m_grid = Grid{ m_window, { 8, 8 }, 10 };
}


void Application::handleInputs() {
	sf::Event sfmlEvent{ };
	while (m_window->pollEvent(sfmlEvent)) {
		m_grid.handleInputs(sfmlEvent);

		switch (sfmlEvent.type) {
		case sf::Event::Closed:
			m_window->close();
			break;
		}
	}
}


void Application::update() {
	float deltaTime = m_clock.restart().asSeconds();
}


void Application::render() {
	m_window->clear({ 50, 40, 80 });

	m_grid.render();

	m_window->display();
}


void Application::run() {
	while (m_window->isOpen()) {
		handleInputs();
		update();
		render();
	}
}
