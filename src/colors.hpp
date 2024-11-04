#pragma once
#include <SFML/Graphics.hpp>

namespace Gradient
{
	sf::Color RED_BLUE(float val) {
		return {
			(sf::Uint8)(-(val - 1) / 2.0f * 255),
			0,
			(sf::Uint8)((val + 1) / 2.0f * 255)
		};
	}

	sf::Color GREEN_BLUE(float val) {
		return {
			0,
			(sf::Uint8)(-(val - 1) / 2.0f * 255),
			(sf::Uint8)((val + 1) / 2.0f * 255)
		};
	}

	sf::Color BLACK_WHITE(float val) {
		return {
			(sf::Uint8)((val + 1) / 2.0f * 255),
			(sf::Uint8)((val + 1) / 2.0f * 255),
			(sf::Uint8)((val + 1) / 2.0f * 255)
		};
	}
}