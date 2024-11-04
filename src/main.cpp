#include <SFML/Graphics.hpp>
#include "configuration.hpp"
#include "shapegrid.hpp"

const sf::Vector2i dim{ 100, 100 };

const float k = 0.9;
const float w = 0.5;
const sf::Vector2i center{ 50, 50 };

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

    ShapeGrid sg{ dim, {0.0f, 0.0f}, conf::WINDOW_SIZE_F, ShapeGrid::BLACK_WHITE };

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

        for (int i = 0; i < sg.dim.y; i++) {
            for (int j = 0; j < sg.dim.x; j++) {
                sg.values[i][j] = u(r({ i, j }), t);
            }
        }

        sg.render(window);
        
        window.display();

        t += 1;
    }
}