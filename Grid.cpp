#include "Grid.h"


Cell::Cell() {
	m_sprite.setTexture(TextureManager::get("textureatlas.png").texture);
}


void Cell::setTexture(size_t index) {
	const sf::Vector2i texCoords{ 32 * (static_cast<int>(index) % 5), 32 * (static_cast<int>(index) / 5) };
	m_sprite.setTextureRect({ { texCoords.x, texCoords.y },{ 32, 32 } });
}


void Cell::render(sf::RenderTarget* target, const sf::Vector2f& position, const sf::Vector2f& cellSize) {
	sf::Transform transform;
	transform.translate(position);
	transform.scale({ cellSize.x / 32.0f, cellSize.y / 32.0f });
	sf::RenderStates states{ transform };

	if (isCovered && !isFlagged) setTexture(12);
	else if (isFlagged) setTexture(11);
	else if (isMine) setTexture(10);
	else {
		setTexture(numMines);
		if (numMines > numFlags) {
			m_sprite.setColor({ 255, 255, 255 });
		} else if (numMines == numFlags) {
			m_sprite.setColor({ 150, 150, 150 });
		} else if (numMines < numFlags) {
			m_sprite.setColor({ 255, 0, 0 });
		}
	}
	target->draw(m_sprite, states);
}


Grid::Grid(sf::RenderWindow* window, const sf::Vector2i& size, size_t numMines)
	: m_window{ window }, m_size { size } {
	initialize(numMines);
}


void Grid::initialize(size_t numMines) {
	m_cellSize = { 600.0f / m_size.y, 600.0f / m_size.y };
	m_cells = std::vector<Cell>(m_size.x * m_size.y, Cell{});

	// Finds random positions for the mines
	for (size_t i = 0; i < numMines;) {
		const sf::Vector2i minePos = { randInt(0, m_size.x - 1), randInt(0, m_size.y - 1) };
		if (m_cells.at(minePos.x + minePos.y * m_size.x).isMine) continue;
		m_cells.at(minePos.x + minePos.y * m_size.x).isMine = true;
		i++;
	}

	// Updates the cells adjacent to the mines to have the correct numbers
	for (size_t i = 0; i < m_size.x; ++i) {
		for (size_t j = 0; j < m_size.y; ++j) {
			if (!m_cells.at(i + j * m_size.x).isMine) continue;
			for (const auto& [dx, dy] : std::vector<std::pair<int, int>>
				{ {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} }) {
				if (i + dx < 0 || i + dx >= m_size.x || j + dy < 0 || j + dy >= m_size.y) continue;
				m_cells.at((i + dx) + (j + dy) * m_size.x).numMines++;
			}
		}
	}

	// Initializes grid vertecies
	m_topLeft = (sf::Vector2f{ m_window->getSize() } - sf::Vector2f{ m_cellSize.x * m_size.x, m_cellSize.y * m_size.y }) / 2.0f;
	m_bottomRight = (sf::Vector2f{ m_window->getSize() } + sf::Vector2f{ m_cellSize.x * m_size.x, m_cellSize.y * m_size.y }) / 2.0f;
	for (float i = m_topLeft.x; i <= m_bottomRight.x; i += m_cellSize.x) {
		m_grid.append({ { i, m_topLeft.y } });
		m_grid.append({ { i, m_bottomRight.y } });
	}
	for (float j = m_topLeft.y; j <= m_bottomRight.y; j += m_cellSize.y) {
		m_grid.append({ { m_topLeft.x, j } });
		m_grid.append({ { m_bottomRight.x, j } });
	}
}


void Grid::updateHighlighedCells(const sf::Vector2i& cellIndex) {
	m_highlightedCells.clear();
	sf::Color highlightColor = { 255, 255, 255, 50 };
	for (const auto& [dx, dy] : std::vector<std::pair<int, int>>
		{ {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0},  {0, 1}, {1, -1}, {1, 0}, {1, 1} }) {
		if (cellIndex.x + dx < 0 || cellIndex.x + dx >= m_size.x || cellIndex.y + dy < 0 || cellIndex.y + dy >= m_size.y) continue;
		if (!m_cells.at((cellIndex.x + dx) + (cellIndex.y + dy) * m_size.x).isCovered) continue;
		sf::Vector2f cellPos = sf::Vector2f{ cellIndex + sf::Vector2i{dx, dy} };
		cellPos = sf::Vector2f{ cellPos.x * m_cellSize.x, cellPos.y * m_cellSize.y } + m_topLeft;

		m_highlightedCells.append({ {cellPos.x, cellPos.y}, highlightColor });
		m_highlightedCells.append({ {cellPos.x + m_cellSize.x, cellPos.y}, highlightColor });
		m_highlightedCells.append({ {cellPos.x + m_cellSize.x, cellPos.y + m_cellSize.y}, highlightColor });
		m_highlightedCells.append({ {cellPos.x, cellPos.y + m_cellSize.y}, highlightColor });
	}
}


void Grid::floodFill(const sf::Vector2i& cellPos) {
	std::unordered_set<sf::Vector2i> visited{ cellPos };
	std::queue<sf::Vector2i> queue;
	queue.push(cellPos);
	
	while (!queue.empty()) {
		sf::Vector2i current = queue.front();
		queue.pop();

		if (m_cells.at(current.x + current.y * m_size.x).numMines != 0) continue;

		for (const auto& [dx, dy] : std::vector<std::pair<int, int>>
			{ {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} }) {
			if (current.x + dx < 0 || current.x + dx >= m_size.x || current.y + dy < 0 || current.y + dy >= m_size.y) continue;
			if (m_cells.at((current.x + dx) + (current.y + dy) * m_size.x).isMine) continue;
			if (visited.contains({ current.x + dx, current.y + dy })) continue;
			queue.push({ current.x + dx, current.y + dy });
			visited.insert({ current.x + dx, current.y + dy });
		}
	}

	for (const auto& pos : visited) {
		m_cells.at(pos.x + pos.y * m_size.x).isCovered = false;
	}
}


void Grid::gameOver() {
	for (Cell& cell : m_cells) {
		cell.isFlagged = false;
		cell.isCovered = false;
		cell.numFlags = 0;
	}
}


void Grid::handleInputs(const sf::Event& sfmlEvent) {
	const sf::Vector2f mousePos{ sf::Vector2f{ sf::Mouse::getPosition(*m_window) } - m_topLeft };
	const sf::Vector2i cellPos = { static_cast<int>(std::floorf(mousePos.x / m_cellSize.x)),
		static_cast<int>(std::floorf(mousePos.y / m_cellSize.y)) };
	
	if (cellPos.x < 0 || cellPos.x >= m_size.x || cellPos.y < 0 || cellPos.y >= m_size.y) return;
	updateHighlighedCells(cellPos);

	switch (sfmlEvent.type) {
	case sf::Event::MouseButtonPressed:
		if (sfmlEvent.mouseButton.button == sf::Mouse::Left) {
			if (m_cells.at(cellPos.x + cellPos.y * m_size.x).isFlagged) break;
			else if (m_cells.at(cellPos.x + cellPos.y * m_size.x).isMine) gameOver();
			else  floodFill(cellPos);
		} else if (sfmlEvent.mouseButton.button == sf::Mouse::Right) {
			if (!m_cells.at(cellPos.x + cellPos.y * m_size.x).isCovered) break;
			m_cells.at(cellPos.x + cellPos.y * m_size.x).isFlagged = !m_cells.at(cellPos.x + cellPos.y * m_size.x).isFlagged;

			for (const auto& [dx, dy] : std::vector<std::pair<int, int>>
				{ {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} }) {
				if (cellPos.x + dx < 0 || cellPos.x + dx >= m_size.x || cellPos.y + dy < 0 || cellPos.y + dy >= m_size.y) continue;
				if (m_cells.at((cellPos.x + dx) + (cellPos.y + dy) * m_size.x).isMine) continue;
				m_cells.at((cellPos.x + dx) + (cellPos.y + dy) * m_size.x).numFlags +=
					m_cells.at(cellPos.x + cellPos.y * m_size.x).isFlagged ? 1 : -1;
			}
		}
		break;
	}
}


void Grid::render() {
	for (size_t i = 0; i < m_size.x; ++i) {
		for (size_t j = 0; j < m_size.y; ++j) {
			const sf::Vector2f pos = m_topLeft + sf::Vector2f{ i * m_cellSize.x, j * m_cellSize.y };
			m_cells.at(i + j * m_size.x).render(m_window, pos, m_cellSize);
		}
	}

	m_window->draw(m_highlightedCells);

	m_window->draw(m_grid);
}
