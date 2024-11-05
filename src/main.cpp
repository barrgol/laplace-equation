#include <SFML/Graphics.hpp>
#include <iostream>
#include <format>
#include "configuration.hpp"
#include "shapegrid.hpp"
#include "arraygrid.hpp"
#include "colors.hpp"

const sf::Vector2i dim{ 600, 600 };

const float k = 0.5;
const float w = 1.0;
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

    //ShapeGrid grid{ dim, {0.0f, 0.0f}, conf::WINDOW_SIZE_F, Gradient::GREEN_BLUE };
    ArrayGrid grid{ dim, {0.0f, 0.0f}, conf::WINDOW_SIZE_F, Gradient::GREEN_BLUE };

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("res/arialbd.ttf"))
    {
        std::cerr << "Error: Could not open 'arialbd.ttf'" << std::endl;
        return EXIT_FAILURE;
    }

    // Text that displays the FPS
    int nframes = 0;
    sf::Text fpsText(std::format("FPS: {}", nframes), font);

    fpsText.setCharacterSize(36);
    fpsText.setColor(sf::Color::Yellow);
    fpsText.setStyle(sf::Text::Bold);
    fpsText.setPosition(sf::Vector2f{ 20.0f, 20.0f });

    // Discrete time
    int t = 0;

    // Create a clock to track FPS
    sf::Clock clock;

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

        grid.render(window);
        window.draw(fpsText);

        window.display();

        nframes += 1;

        // Check elapsed time
        if (clock.getElapsedTime().asSeconds() >= 1.0) {
            fpsText.setString(std::format("FPS: {}", nframes));
            nframes = 0;
            clock.restart();
        }

        t += 1;
    }
}