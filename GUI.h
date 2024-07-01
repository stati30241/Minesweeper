#pragma once

#include <SFML/Graphics.hpp>

#include <ctime>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <unordered_set>


int randInt(int a, int b);


template <>
struct std::hash<sf::Vector2i> {
	std::size_t operator() (const sf::Vector2i& vec) const {
		return std::hash<int>()(vec.x) ^ std::hash<int>()(vec.y);
	}
};


class TextureManager {
public:
	sf::Texture texture;

private:
	TextureManager(std::string_view filename) {
		texture.loadFromFile(filename.data());
		texture.setSmooth(true);
	}

	TextureManager(const TextureManager& copy);
	TextureManager operator= (const TextureManager& copy);

public:
	static TextureManager& get(std::string_view filename = "") {
		static TextureManager instance{ filename };
		return instance;
	}
};
