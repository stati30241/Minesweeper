#pragma once

#include "GUI.h"


// Manages each individual cell
class Cell {
private:
	sf::VertexArray m_vertecies{ sf::Quads };

private:
	void setTexture(size_t index);

public:
	size_t numMines = 0;
	bool isMine = false;
	bool isCovered = false;
	bool isSatisfied = false;
	bool isFlagged = false;

public:
	Cell();
	void render(sf::RenderTarget* target, const sf::Vector2f& position, const sf::Vector2f& cellSize);
};


// Manages the minesweeper game
class Grid {
private:
	sf::RenderWindow* m_window;
	sf::Vector2i m_size;
	sf::Vector2f m_cellSize;
	std::vector<Cell> m_cells;

	sf::VertexArray m_grid{ sf::Lines };
	sf::Vector2f m_topLeft;
	sf::Vector2f m_bottomRight;

private:
	void initialize(size_t numMines);

public:
	Grid() : m_window{ nullptr } { };
	Grid(sf::RenderWindow* window, const sf::Vector2i& size, size_t numMines);

	void handleInputs(const sf::Event& sfmlEvent);
	void render();
};
