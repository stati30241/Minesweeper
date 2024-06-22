#include "Grid.h"


int randInt(int a, int b) {
	static std::mt19937 twister{ static_cast<unsigned int>(std::time(nullptr)) };
	std::uniform_int_distribution<int> randDist{ a, b };
	return randDist(twister);
}


Grid::Grid(sf::RenderWindow* window, const sf::Vector2i& size, size_t numMines)
	: m_window{ window }, m_size { size } {
	initialize(numMines);
}


void Grid::initialize(size_t numMines) {
	m_cellSize = { 600.0f / m_size.y, 600.0f / m_size.y };
	m_grid = std::vector<char>(m_size.x * m_size.y, '\0');

	for (size_t i = 0; i < numMines;) {
		const sf::Vector2i minePos = { randInt(0, m_size.x - 1), randInt(0, m_size.y - 1) };
		if (m_grid.at(minePos.x + minePos.y * m_size.x) & 0b00010000) continue;
		m_grid.at(minePos.x + minePos.y * m_size.x) |= 0b00010000;
		i++;
	}

	for (size_t i = 0; i < m_size.x; ++i) {
		for (size_t j = 0; j < m_size.y; ++j) {
			if (!(m_grid.at(i + j * m_size.x) & 0b00010000)) continue;
			for (const auto& [dx, dy] : std::vector<std::pair<int, int>>
				{ {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} }) {
				if (i + dx < 0 || i + dx >= m_size.x || j + dy < 0 || j + dy >= m_size.y) continue;
				m_grid.at((i + dx) + (j + dy) * m_size.x)++;
			}
		}
	}

	const sf::Vector2f topLeft = (sf::Vector2f{ m_window->getSize() } - sf::Vector2f{ m_cellSize.x * m_size.x, m_cellSize.y * m_size.y }) / 2.0f;
	const sf::Vector2f bottomRight = (sf::Vector2f{ m_window->getSize() } + sf::Vector2f{ m_cellSize.x * m_size.x, m_cellSize.y * m_size.y }) / 2.0f;
	for (float i = topLeft.x; i <= bottomRight.x; i += m_cellSize.x) {
		m_vertecies.append({ { i, topLeft.y } });
		m_vertecies.append({ { i, bottomRight.y } });
	}
	for (float j = topLeft.y; j <= bottomRight.y; j += m_cellSize.y) {
		m_vertecies.append({ { topLeft.x, j } });
		m_vertecies.append({ { bottomRight.x, j } });
	}

	for (size_t i = 0; i < m_size.x; ++i) {
		for (size_t j = 0; j < m_size.y; ++j) {
			const sf::Vector2f pos = { i * m_cellSize.x + topLeft.x, j * m_cellSize.y + topLeft.y };
			std::string str = "";
			if (m_grid.at(i + j * m_size.x) & 0b10000) str = ">";
			else {
				int num = m_grid.at(i + j * m_size.x) & 0b1111;
				str = std::to_string(num);
			}

			sf::Text text{ str, FontManager::getFont("cour.ttf").font };
			text.setPosition(pos + m_cellSize / 4.0f);
			m_numbersText.push_back(text);
		}
	}
}


void Grid::handleInputs(const sf::Event& sfmlEvent) {

}


void Grid::render() {
	m_window->draw(m_vertecies);
	for (const auto& text : m_numbersText) m_window->draw(text);
}
