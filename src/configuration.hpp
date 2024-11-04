#pragma once

namespace conf
{
	// Window configuration
	sf::Vector2u const WINDOW_SIZE = { 800u, 800u };
	sf::Vector2f const WINDOW_SIZE_F = static_cast<sf::Vector2f>(WINDOW_SIZE);
	uint32_t const MAX_FRAMERATE = 60;
	float const DT = 1.0f / static_cast<float>(MAX_FRAMERATE);
}