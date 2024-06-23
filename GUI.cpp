#include "GUI.h"

int randInt(int a, int b) {
	static std::mt19937 twister{ static_cast<unsigned int>(std::time(nullptr)) };
	std::uniform_int_distribution<int> randDist{ a, b };
	return randDist(twister);
}
