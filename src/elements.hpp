#pragma once
#include <SFML/Graphics.hpp>

namespace elements {
    void loadFont(sf::Font& font) {
        if (!font.loadFromFile("res/arialbd.ttf"))
        {
            std::cerr << "Error: Could not open 'arialbd.ttf'" << std::endl;
        }
    }

    void fpsTextStyle(sf::Text& fpsText) {
        fpsText.setCharacterSize(36);
        fpsText.setColor(sf::Color::Yellow);
        fpsText.setStyle(sf::Text::Bold);
        fpsText.setPosition(sf::Vector2f{ 20.0f, 20.0f });
    }

    void stepsTextStyle(sf::Text& stepsText) {
        stepsText.setCharacterSize(24);
        stepsText.setColor(sf::Color::Yellow);
        stepsText.setStyle(sf::Text::Bold);
        stepsText.setPosition(sf::Vector2f{ 20.0f, conf::WINDOW_SIZE_F.y - 40.0f });
    }

    void instructionTextStyle(sf::Text& instructionsText) {
        instructionsText.setCharacterSize(18);
        instructionsText.setColor(sf::Color::Yellow);
        instructionsText.setPosition(sf::Vector2f{ conf::WINDOW_SIZE_F.x - 200.0f, conf::WINDOW_SIZE_F.y - 80.0f });
    }
}