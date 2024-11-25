#pragma once
#include <SFML/Window.hpp>
#include <vector>
#include "simulation.hpp"

struct AppState {
    std::vector<std::vector<float>>& values;
    std::vector<std::vector<bool>>& isConst;
    int& nsteps;
    bool& paused;
    bool& converged;
};

void processEvents(sf::Window& window, AppState& state) {
    for (auto event = sf::Event(); window.pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
                window.close();
                break;
            case sf::Keyboard::R:
                Laplace::init(state.values, state.isConst);
                state.paused = true;
                state.converged = false;
                state.nsteps = 0;
                break;
            case sf::Keyboard::S:
                state.paused = !state.paused;
                break;
            default:
                break;
            }
        }
    }
}