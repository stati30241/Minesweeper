#include "GUI.h"


int randInt(int a, int b) {
	static std::mt19937 twister{ static_cast<unsigned int>(std::time(nullptr)) };
	std::uniform_int_distribution<int> randDist{ a, b };
	return randDist(twister);
}


Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::IntRect& textureRect) {
	m_sprite.setTexture(TextureManager::get("textureAtlas").texture);
	m_sprite.setTextureRect(textureRect);
	m_sprite.setPosition(position);
	m_sprite.setScale(size.x / textureRect.width, size.y / textureRect.height);
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(m_sprite, states);
}


void Button::handleInputs(const sf::Event& sfmlEvent) {
	switch (sfmlEvent.type) {
	case sf::Event::MouseButtonPressed:
		if (m_sprite.getGlobalBounds().contains({ (float)sfmlEvent.mouseButton.x, (float)sfmlEvent.mouseButton.y })) {
			m_enabled = !m_enabled;
			m_sprite.setColor({ 255, 255, 0 });
		} break;

	case sf::Event::MouseMoved:
		if (m_sprite.getGlobalBounds().contains({ (float)sfmlEvent.mouseMove.x, (float)sfmlEvent.mouseMove.y })) {
			m_sprite.setColor(m_enabled ? sf::Color{ 150, 150, 0 } : sf::Color{ 150, 150, 150 });
		} else {
			m_sprite.setColor(m_enabled ? sf::Color{ 255, 255, 0 } : sf::Color{ 255, 255, 255 });
		} break;
	}
}
