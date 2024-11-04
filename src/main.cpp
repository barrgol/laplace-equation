#include <SFML/Graphics.hpp>
#include "configuration.hpp"
#include "shapegrid.hpp"
#include "arraygrid.hpp"
#include "colors.hpp"

const sf::Vector2i dim{ 500, 500 };

const float k = 0.5;
const float w = 0.5;
const sf::Vector2i center{ dim.x / 2, dim.y / 2 };

float r(sf::Vector2i pos) {    
    int i = pos.x - center.x;
    int j = pos.y - center.y;
    return std::sqrt(i * i + j * j);
}

float u(float r, float t) {
    return std::cos(k * r - w * t);
}

int main()
{
    auto window = sf::RenderWindow{ { conf::WINDOW_SIZE.x, conf::WINDOW_SIZE.y }, "Application" };
    window.setFramerateLimit(conf::MAX_FRAMERATE);

    //ShapeGrid grid{ dim, {0.0f, 0.0f}, conf::WINDOW_SIZE_F, Gradient::BLACK_WHITE };
    ArrayGrid grid{ dim, {0.0f, 0.0f}, conf::WINDOW_SIZE_F, Gradient::RED_BLUE };

    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray triangle(sf::Triangles, 6);

    int t = 0;
    while (window.isOpen())
    {
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        for (int i = 0; i < grid.dim.y; i++) {
            for (int j = 0; j < grid.dim.x; j++) {
                grid.values[i][j] = u(r({ i, j }), t);
            }
        }

        //sg.render(window);
        grid.render(window);

        window.draw(triangle);

        window.display();

        t += 1;
    }
}