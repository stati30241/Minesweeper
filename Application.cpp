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

	m_grid = Grid{ m_window, { 12, 12 }, 16 };
	m_canvas = Canvas{ m_window };

	m_buttons.insert({ "New Game", Button{ { 22, 75 }, { 256, 64 }, { { 0, 129 }, { 128, 32 } } } });
	m_buttons.insert({ "Draw", Button{ { 936, 80 }, { 96, 96 }, { { 128, 129 }, { 32, 32 } } } });
}


void Application::handleInputs() {
	sf::Event sfmlEvent{ };
	while (m_window->pollEvent(sfmlEvent)) {
		for (auto& [str, button] : m_buttons) {
			button.handleInputs(sfmlEvent);
		}

		if (m_buttons.at("Draw").isEnabled()) m_canvas.handleInputs(sfmlEvent);
		else m_grid.handleInputs(sfmlEvent);

		switch (sfmlEvent.type) {
		case sf::Event::Closed:
			m_window->close();
			break;
		}
	}
}


void Application::update() {
	float deltaTime = m_clock.restart().asSeconds();

	if (m_buttons.at("New Game").isEnabled()) {
		m_buttons.insert({ "Easy", Button{ { 54, 175 }, { 192, 64 }, { { 0, 161 }, { 96, 32 } } } });
		m_buttons.insert({ "Medium", Button{ { 54, 275 }, { 192, 64 }, { { 0, 193 }, { 96, 32 } } } });
		m_buttons.insert({ "Hard", Button{ { 54, 375 }, { 192, 64 }, { { 0, 225 }, { 96, 32 } } } });
	} else {
		m_buttons.erase("Easy");
		m_buttons.erase("Medium");
		m_buttons.erase("Hard");
	}

	if (m_buttons.at("Draw").isEnabled()) {
		m_buttons.insert({ "Erase", Button{ { 1068, 80 }, { 96, 96 }, { { 0, 97 }, { 32, 32 } } } });
		m_buttons.insert({ "Undo", Button{ { 936, 212 }, { 96, 96 }, { { 96, 65 }, { 32, 32 } } } });
		m_buttons.insert({ "Redo", Button{ { 1068, 212 }, { 96, 96 }, { { 128, 65 }, { 32, 32 } } } });
		m_buttons.insert({ "Size Small", Button{ { 936, 344 }, { 96, 96 }, { { 32, 97 }, { 32, 32 } } } });
		m_buttons.insert({ "Size Medium", Button{ { 1068, 344 }, { 96, 96 }, { { 64, 97 }, { 32, 32 } } } });
		m_buttons.insert({ "Size Large", Button{ { 936, 476 }, { 96, 96 }, { { 96, 97 }, { 32, 32 } } } });
		m_buttons.insert({ "Clear", Button{ { 1068, 476 }, { 96, 96 }, { { 128, 97 }, { 32, 32 } } } });
	} else {
		m_buttons.erase("Erase");
		m_buttons.erase("Undo");
		m_buttons.erase("Redo");
		m_buttons.erase("Size Small");
		m_buttons.erase("Size Medium");
		m_buttons.erase("Size Large");
		m_buttons.erase("Clear");
	}


	if (m_buttons.at("New Game").isEnabled()) {
		if (m_buttons.at("Easy").isEnabled()) {
			m_grid = Grid(m_window, { 10, 10 }, 8);
			m_buttons.at("Easy").disable();
			m_buttons.at("New Game").disable();
		} if (m_buttons.at("Medium").isEnabled()) {
			m_grid = Grid(m_window, { 12, 12 }, 16);
			m_buttons.at("Medium").disable();
			m_buttons.at("New Game").disable();
		} if (m_buttons.at("Hard").isEnabled()) {
			m_grid = Grid(m_window, { 18, 18 }, 48);
			m_buttons.at("Hard").disable();
			m_buttons.at("New Game").disable();
		}
	}
}


void Application::render() {
	m_window->clear({ 50, 40, 80 });

	m_grid.render();
	m_canvas.render();


	for (auto& [str, button] : m_buttons) {
		m_window->draw(button);
	}

	m_window->display();
}


void Application::run() {
	while (m_window->isOpen()) {
		handleInputs();
		update();
		render();
	}
}
