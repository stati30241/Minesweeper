#pragma once

#include "Grid.h"


class Application {
private:
	sf::RenderWindow* m_window;
	sf::Clock m_clock;

	Grid m_grid;

private:
	void initialize();

	void handleInputs();
	void update();
	void render();

public:
	Application();
	~Application();

	void run();
};
