#pragma once
#include <SFML/Graphics.hpp>

namespace Gradient
{
	// Minimal and maximal temperature
	float v_min = 32;
	float v_max = 212;

	const sf::Color COLOR_MIN{ 0,0,255 };	// Pure blue at coldest temperature
	const sf::Color COLOR_MAX{ 255,0,0 };	// Pure red at hottest temperature



	sf::Color RED_BLUE(float val) {
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