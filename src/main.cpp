#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <format>
#include "configuration.hpp"
#include "arraygrid.hpp"
#include "gradient.hpp"

// Grid dimension
const int N = 300;

// Wall boundaries where temperature rules will change, keep constant to avoid recalculation
const int INNER_TOP = 3.0f / 9 * N;
const int INNER_BOTTOM = 6.0f / 9 * N;
const int INNER_LEFT = 3.0f / 9 * N;
const int ICEBATH_TOP = 5.0f / 9 * N;

// If no cell changed by more than this value we stop the simulation
const float EPS = 0.05f;

int main()
{
    auto window = sf::RenderWindow{ { conf::WINDOW_SIZE.x, conf::WINDOW_SIZE.y }, "Application" };
    window.setFramerateLimit(conf::MAX_FRAMERATE);

    // Structure with grid of values and visual updates
    ArrayGrid grid{ {N, N}, {0.0f, 0.0f}, conf::WINDOW_SIZE_F, Gradient::RED_BLUE };

    // Boolean matrix to indicate which values stay constant
    std::array<std::array<bool, N>, N> isConst;

    // Initialize the grid values
    for (size_t i = 0; i < N; i++) {
        // Make use of the symmetry around the vertical axis
        for (size_t j = 0; j <= N / 2; j++) {
            // Top surface is kept constant at 100°F
            if (i == 0) {
                grid.values[i][j] = 100;
                grid.values[i][N - j - 1] = 100;

                isConst[i][j] = true;
                isConst[i][N - j - 1] = true;
            } else if ((i > ICEBATH_TOP && j == 0) || i == N - 1) {
                // Lower part is kept in ice bath at 32°F
                grid.values[i][j] = 32;
                grid.values[i][N - j - 1] = 32;

                isConst[i][j] = true;
                isConst[i][N - j - 1] = true;
            } else if (i <= ICEBATH_TOP && j == 0) {
                // Temperature at the pipe walls above the ice bath increases
                // linearly from 32°F to 100°F, 100°F - 32°F = 68°F
                float t = ((float) i / ICEBATH_TOP);
                float val = 100 - t * 68;

                grid.values[i][j] = val;
                grid.values[i][N - j - 1] = val;

                isConst[i][j] = true;
                isConst[i][N - j - 1] = true;
            } else if (i >= INNER_TOP && i <= INNER_BOTTOM && j >= INNER_LEFT) {
                // Inner pipe
                if (i == INNER_TOP || i == INNER_BOTTOM || j == INNER_LEFT) {
                    // Inner pipe filled with steam with walls kept at 212°F
                    grid.values[i][j] = 212;
                    grid.values[i][N - j - 1] = 212;
                } else {
                    // We treat insides as special case to indicate we're not interested in these values
                    grid.values[i][j] = 0;
                    grid.values[i][N - j - 1] = 0;
                }
                isConst[i][j] = true;
                isConst[i][N - j - 1] = true;
            } else {
                // In all remaining cases we arbitrarily set temperature to
                // a 'reasonable' value between 32°F and 212°F - here we do 90°F
                grid.values[i][j] = 90;
                grid.values[i][N - j - 1] = 90;

                // These are the only values that will not be constant
                isConst[i][j] = false;
                isConst[i][N - j - 1] = false;
            }
        }
    }

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
            float maxUpdate = FLT_MIN;

            // Update the grid with the magic formula
            for (size_t i = 0; i < grid.dim.y; i++) {
                for (size_t j = 0; j < grid.dim.x; j++) {
                    // Only apply the formula if the grid cell is not constant
                    if (!isConst[i][j]) {
                        // Get neighbouring cells and apply the formula
                        float top = grid.values[i - 1][j];
                        float bottom = grid.values[i + 1][j];
                        float left = grid.values[i][j - 1];
                        float right = grid.values[i][j + 1];

                        float val = 0.25f * (top + bottom + left + right);

                        // Check if value update exceeded current maximum
                        float update = std::abs(grid.values[i][j] - val);
                        maxUpdate = std::max(maxUpdate, update);

                        // Update the values utilizing domain symmetry
                        grid.values[i][j] = val;
                        grid.values[i][N - j - 1] = val;
                    }
                }
            }

            // Check if the simulation converged
            if (maxUpdate < EPS) {
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