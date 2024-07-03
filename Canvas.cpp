#include "Canvas.h"


Canvas::Canvas(sf::RenderWindow* window)
	: m_window{ window } {
	m_canvas.create(m_window->getSize().x, m_window->getSize().y, sf::Color::Transparent);
}


void Canvas::draw(const sf::Vector2f& pos) {
	for (size_t i = pos.x - m_size; i < pos.x + m_size; ++i) {
		for (size_t j = pos.y - m_size; j < pos.y + m_size; ++j) {
			if ((i - pos.x) * (i - pos.x) + (j - pos.y) * (j - pos.y) > m_size * m_size) continue;
			if (m_erasing) m_canvas.setPixel(i, j, sf::Color::Transparent);
			else m_canvas.setPixel(i, j, sf::Color::Green);
		}
	}
}


void Canvas::undo() {

}


void Canvas::redo() {

}


void Canvas::handleInputs(const sf::Event& sfmlEvent) {
	const sf::Vector2f mousePos = sf::Vector2f{ sf::Mouse::getPosition(*m_window) };
	if (mousePos.x < 0.0f || mousePos.x > m_window->getSize().x ||
		mousePos.y < 0.0f || mousePos.y > m_window->getSize().y) return;

	switch (sfmlEvent.type) {
	case sf::Event::MouseButtonPressed:
		if (sfmlEvent.mouseButton.button == sf::Mouse::Left) m_erasing = false;
		else if (sfmlEvent.mouseButton.button == sf::Mouse::Right) m_erasing = true;
		m_drawing = true;
		draw(mousePos);
		break;

	case sf::Event::MouseButtonReleased:
		m_drawing = false;
		break;

	case sf::Event::MouseMoved:
		if (sfmlEvent.mouseButton.button == sf::Mouse::Left) m_erasing = false;
		else if (sfmlEvent.mouseButton.button == sf::Mouse::Right) m_erasing = true;
		if (!m_drawing) break;
		draw(mousePos);
		break;
	}
}


void Canvas::render() {
	sf::Texture texture;
	texture.loadFromImage(m_canvas);
	sf::Sprite s{ texture };
	m_window->draw(s);
}
