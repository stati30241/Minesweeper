#pragma once

#include <SFML/Graphics.hpp>

#include <ctime>
#include <random>
#include <string>
#include <vector>


int randInt(int a, int b);


class TextureManager {
public:
	sf::Texture texture;

private:
	TextureManager(std::string_view filename) {
		texture.loadFromFile(filename.data());
	}

	TextureManager(const TextureManager& copy);
	TextureManager operator= (const TextureManager& copy);

public:
	static TextureManager& get(std::string_view filename = "") {
		static TextureManager instance{ filename };
		return instance;
	}
};
