#pragma once

#include <SFML/Graphics.hpp>

#include <ctime>
#include <random>
#include <string>
#include <vector>


int randInt(int a, int b);


class FontManager {
public:
	sf::Font font;

private:
	FontManager(std::string_view filename) {
		font.loadFromFile(filename.data());
	}

	FontManager(const FontManager& copy);
	FontManager operator= (const FontManager& copy);

public:
	static FontManager& getFont(std::string_view filename = "") {
		static FontManager instance{ filename };
		return instance;
	}
};


// Manages the minesweeper game
class Grid {
private:
	sf::RenderWindow* m_window;
	sf::Vector2i m_size;
	sf::Vector2f m_cellSize;
	std::vector<char> m_grid;

	sf::VertexArray m_vertecies{ sf::Lines };
	std::vector<sf::Text> m_numbersText;

private:
	void initialize(size_t numMines);

public:
	Grid() { };
	Grid(sf::RenderWindow* window, const sf::Vector2i& size, size_t numMines);

	void handleInputs(const sf::Event& sfmlEvent);
	void render();
};
