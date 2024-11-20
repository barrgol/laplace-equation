#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <format>
#include <random>
#include "configuration.hpp"
#include "arraygrid.hpp"
#include "gradient.hpp"
#include "simulation.hpp"

int main()
{
    // Main SFML window
    auto window = sf::RenderWindow{ { conf::WINDOW_SIZE.x, conf::WINDOW_SIZE.y }, "Laplace" };
    window.setFramerateLimit(conf::MAX_FRAMERATE);

    // Structure with grid of values and visual updates
    ArrayGrid grid{ {conf::N, conf::N}, {0.0f, 0.0f}, conf::WINDOW_SIZE_F, Gradient::RED_BLUE };

    // Boolean matrix to indicate which values stay constant
    std::vector<std::vector<bool>> isConst{ conf::N, std::vector<bool>(conf::N, false) };

    // Initialize the grid values
    Laplace::init(grid.values, isConst);

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

    // Text that display the number of steps
    int nsteps = 0;
    bool converged = false;
    sf::Text stepsText(std::format("Simulation steps: {}", nsteps), font);

    stepsText.setCharacterSize(24);
    stepsText.setColor(sf::Color::Yellow);
    stepsText.setStyle(sf::Text::Bold);
    stepsText.setPosition(sf::Vector2f{ 20.0f, conf::WINDOW_SIZE_F.y - 40.0f });

    // Create a clock to track FPS
    sf::Clock clock;

    while (window.isOpen())
    {
        // Check if the "X" was clicked, close the window if it was
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        } 

        // Continue updating untill convergence
        if (!converged) {
            // Store the maximal update in the current iteration
            float maxUpdate = Laplace::update(grid.values, isConst);

            // Check if the simulation converged
            if (maxUpdate < conf::EPS) {
                converged = true;
            }

            // Update the counters
            nframes += 1;
            nsteps += 1;

            // Check elapsed time
            if (clock.getElapsedTime().asSeconds() >= 1.0) {
                fpsText.setString(std::format("FPS: {}", nframes));
                nframes = 0;
                clock.restart();
            }

            // Update step counter
            if (!converged) {
                stepsText.setString(std::format("Simulation steps: {}", nsteps));
            } else {
                stepsText.setString(std::format("Simulation steps: {} (conv.)", nsteps));
                fpsText.setString("FPS: 0");
            }

            // Draw all the elements
            grid.render(window);
            window.draw(fpsText);
            window.draw(stepsText);

            // Display everything to the screen
            window.display();
        }
    }
}