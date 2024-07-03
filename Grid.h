#pragma once

#include "GUI.h"


// Manages each individual cell
class Cell {
private:
	sf::Sprite m_sprite;

private:
	void setTexture(size_t index);

public:
	size_t numMines = 0;
	size_t numFlags = 0;
	bool isMine = false;
	bool isCovered = true;
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
	sf::VertexArray m_highlightedCells{ sf::Quads };
	sf::Vector2f m_topLeft;
	sf::Vector2f m_bottomRight;

	bool m_initialized = false;
	bool m_gameOver = false;
	float timer = 0.0f;

	size_t m_numMines;
	size_t m_numFlags = 0;
	
	sf::Text m_flagText{ "Flags: ", FontManager::get("cour.ttf").font, 40 };
	sf::Text m_timerText{ "Time: ", FontManager::get().font, 40 };
	sf::Text m_gameOverText{ "", FontManager::get().font, 40 };

private:
	void initialize();
	void createGame(const sf::Vector2i& cellPos);

	void updateHighlighedCells(const sf::Vector2i& cellPos);

	void floodFill(const sf::Vector2i& cellPos);
	void gameOver();
	void gameWin();

public:
	Grid() : m_window{ nullptr } { };
	Grid(sf::RenderWindow* window, const sf::Vector2i& size, size_t numMines);

	void handleInputs(const sf::Event& sfmlEvent);
	void update(float deltaTime);
	void render();
};
