#pragma once
#include <cmath>
#include <functional>
#include <SFML/Graphics.hpp>
#include "colors.hpp"

class ShapeGrid
{
public:
	std::vector<std::vector<float>> values;
	sf::Vector2i dim;
	sf::Vector2f pos;
	sf::Vector2f size;

	const float CELL_WIDTH;
	const float CELL_HEIGHT;

private:
	std::vector<std::vector<sf::RectangleShape>> cells;
	std::function<sf::Color(float)> cmap;

	const float CELL_HOFF;
	const float CELL_VOFF;

public:
	ShapeGrid(sf::Vector2i dim, sf::Vector2f pos, sf::Vector2f size,
		      std::function<sf::Color(float)> cmap= Gradient::RED_BLUE)
		: dim(dim)
		, size(size)
		, pos(pos)
		, cmap(cmap)
		, cells(dim.y, std::vector<sf::RectangleShape>(dim.x))
		, values(dim.y, std::vector<float>(dim.x, 0.0f))
		, CELL_WIDTH(std::ceil((size.x / dim.x) * 100) / 100)
		, CELL_HEIGHT(std::ceil((size.y / dim.y) * 100) / 100)
		, CELL_HOFF(std::round((size.x / dim.x) * 100) / 100)
		, CELL_VOFF(std::round((size.y / dim.y) * 100) / 100)
	{
		for (int i = 0; i < dim.y; i++) {
			for (int j = 0; j < dim.x; j++) {
				cells[i][j].setSize({ CELL_WIDTH, CELL_HEIGHT });
				cells[i][j].setPosition({ pos.x + j * CELL_HOFF, pos.y + i * CELL_VOFF });
			}
		}
	}

	void render(sf::RenderWindow& context) {
		for (int i = 0; i < dim.y; i++) {
			for (int j = 0; j < dim.x; j++) {
				cells[i][j].setFillColor(cmap(values[i][j]));
				context.draw(cells[i][j]);

			}
		}
	}
};