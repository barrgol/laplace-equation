#pragma once
#include <SFML/Graphics.hpp>

namespace Gradient
{
	float v_min = 32;
	float v_max = 212;

	const sf::Color COLOR_MIN{ 0,0,255 };
	const sf::Color COLOR_MAX{ 255,0,0};

	sf::Color RED_BLUE(float val) {

		// Special case for space inside the inner pipe (we show it as gray)
		if (val == 0) {
			return {
				100,
				100,
				100
			};
		} else {
			// Gradient value
			double t = (double)(val - v_min) / (v_max - v_min);

			// Blue at v_min, Red at v_max, purple gradient in between
			return {
				(sf::Uint8)(COLOR_MIN.r + t * (COLOR_MAX.r - COLOR_MIN.r)),
				(sf::Uint8)(COLOR_MIN.g + t * (COLOR_MAX.g - COLOR_MIN.g)),
				(sf::Uint8)(COLOR_MIN.b + t * (COLOR_MAX.b - COLOR_MIN.b)),
			};
		}
	}
}