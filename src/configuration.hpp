#pragma once

namespace conf
{
	// ********************************
	// *** Window configuration ***
	// ********************************

	// Visual size of the window (in two formats for convenience)
	sf::Vector2u const WINDOW_SIZE = { 800u, 800u };
	sf::Vector2f const WINDOW_SIZE_F = static_cast<sf::Vector2f>(WINDOW_SIZE);

	// Maximal framerate
	uint32_t const MAX_FRAMERATE = 60;

	// ********************************
	// *** Simulation configuration ***
	// ********************************
	
	// Grid dimension
	const int N = 200;

	// Wall boundaries where temperature rules will change, keep constant to avoid recalculation
	const int INNER_TOP = 3.0f / 9 * N;
	const int INNER_BOTTOM = 6.0f / 9 * N;
	const int INNER_LEFT = 3.0f / 9 * N;
	const int ICEBATH_TOP = 5.0f / 9 * N;

	// If no cell changed by more than this value we stop the simulation
	const float EPS = 0.05f;
}