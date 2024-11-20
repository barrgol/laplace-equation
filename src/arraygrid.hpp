#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "gradient.hpp"

class ArrayGrid
{
public:
	std::vector<std::vector<float>> values;		// Grid with cell values
	sf::Vector2i dim;							// Grid dimension

	sf::Vector2f size;							// Visualization scale of the grid
	const float CELL_WIDTH;						// Visual width of each cell
	const float CELL_HEIGHT;					// Visual height of each cell

private:
	sf::VertexArray vertices;					// SFML's VertexArray for rendering cells
	sf::VertexArray lines;						// SFML's VertexArray for rendering grid lines
	std::function<sf::Color(float)> cmap;		// Color mapping function (gradient)

public:
	ArrayGrid(sf::Vector2i dim, sf::Vector2f pos, sf::Vector2f size,
		std::function<sf::Color(float)> cmap = Gradient::RED_BLUE)
		: dim(dim)
		, size(size)
		, cmap(cmap)
		, vertices(sf::Triangles, dim.x* dim.y * 6)
		, lines(sf::Lines, 2 * (dim.x - 1) + 2 * (dim.y - 1))
		, values(dim.y, std::vector<float>(dim.x, 0.0f))
		, CELL_WIDTH(std::round((size.x / dim.x) * 100) / 100)
		, CELL_HEIGHT(std::round((size.y / dim.y) * 100) / 100)
	{
		for (int i = 0; i < dim.y; i++) {
			for (int j = 0; j < dim.x; j++) {
				// Get a pointer to the triangles' vertices of the current tile
				sf::Vertex* triangles = &vertices[(i * dim.x + j) * 6];

				// Define the 6 corners of the two triangles
				triangles[0].position = sf::Vector2f(j * CELL_WIDTH, i * CELL_HEIGHT);
				triangles[1].position = sf::Vector2f((j + 1) * CELL_WIDTH, i * CELL_HEIGHT);
				triangles[2].position = sf::Vector2f((j + 1) * CELL_WIDTH, (i + 1) * CELL_HEIGHT);
				triangles[3].position = sf::Vector2f(j * CELL_WIDTH, i * CELL_HEIGHT);
				triangles[4].position = sf::Vector2f(j * CELL_WIDTH, (i + 1) * CELL_HEIGHT);
				triangles[5].position = sf::Vector2f((j + 1) * CELL_WIDTH, (i + 1) * CELL_HEIGHT);
			}
		}

		for (int i = 0; i < dim.y - 1; i++) {
			// Get a pointer to the corresponding horizontal line
			sf::Vertex* line = &lines[2 * i];

			// Define the endpoints of a line
			line[0].position = sf::Vector2f(0, (i + 1) * CELL_HEIGHT);
			line[1].position = sf::Vector2f(size.x, (i + 1) * CELL_HEIGHT);

			line[0].color = sf::Color::Black;
			line[1].color = sf::Color::Black;
		}

		for (int j = 0; j < dim.x - 1; j++) {
			// Get a pointer to the corresponding vertical line
			sf::Vertex* line = &lines[2 * (dim.y - 1) + 2 * j];

			// Define the endpoints of a line
			line[0].position = sf::Vector2f((j + 1) * CELL_WIDTH, 0);
			line[1].position = sf::Vector2f((j + 1) * CELL_HEIGHT, size.y);
			
			line[0].color = sf::Color::Black;
			line[1].color = sf::Color::Black;
		}
	}

	void render(sf::RenderWindow& context) {
		for (int i = 0; i < dim.y; i++) {
			for (int j = 0; j < dim.x; j++) {
				// Get a pointer to the triangles' vertices of the current tile
				sf::Vertex* triangles = &vertices[(i * dim.x + j) * 6];

				// Update the colours of the two triangles
				triangles[0].color = cmap(values[i][j]);
				triangles[1].color = cmap(values[i][j]);
				triangles[2].color = cmap(values[i][j]);
				triangles[3].color = cmap(values[i][j]);
				triangles[4].color = cmap(values[i][j]);
				triangles[5].color = cmap(values[i][j]);
			}
		}

		// Render the grid cells and lines
		context.draw(vertices);
		context.draw(lines);
	}
};