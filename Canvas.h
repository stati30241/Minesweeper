#pragma once

#include "GUI.h"


// Manages all the drawing 
class Canvas {
private:
	sf::RenderWindow* m_window;
	sf::Image m_canvas;

	bool m_drawing = true;
	bool m_erasing = false;
	size_t m_size = 5;

private:
	void draw(const sf::Vector2f& position);

public:
	Canvas() : m_window{ nullptr } { };
	Canvas(sf::RenderWindow* m_window);

	void setErasing(bool state) { m_erasing = state; }
	void setSize(size_t size) { m_size = size; }

	void undo();
	void redo();

	void handleInputs(const sf::Event& sfmlEvent);
	void render();
};

