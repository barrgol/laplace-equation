#pragma once
#include "configuration.hpp"

namespace Laplace
{
	static void init(std::vector<std::vector<float>>& values, std::vector<std::vector<bool>>& isConst) {
        // Random number generator to assign random values to intermediate cells
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> unif(32, 212);

        // Initialize the grid values
        for (size_t i = 0; i < conf::N; i++) {
            // Make use of the symmetry around the vertical axis
            for (size_t j = 0; j <= conf::N / 2; j++) {
                // Top surface is kept constant at 100°F
                if (i == 0) {
                    values[i][j] = 100;
                    values[i][conf::N - j - 1] = 100;

                    isConst[i][j] = true;
                    isConst[i][conf::N - j - 1] = true;
                } else if ((i > conf::ICEBATH_TOP && j == 0) || i == conf::N - 1) {
                    // Lower part is kept in ice bath at 32°F
                    values[i][j] = 32;
                    values[i][conf::N - j - 1] = 32;

                    isConst[i][j] = true;
                    isConst[i][conf::N - j - 1] = true;
                } else if (i <= conf::ICEBATH_TOP && j == 0) {
                    // Temperature at the pipe walls above the ice bath increases
                    // linearly from 32°F to 100°F, 100°F - 32°F = 68°F
                    float t = ((float)i / conf::ICEBATH_TOP);
                    float val = 100 - t * 68;

                    values[i][j] = val;
                    values[i][conf::N - j - 1] = val;

                    isConst[i][j] = true;
                    isConst[i][conf::N - j - 1] = true;
                } else if (i >= conf::INNER_TOP && i <= conf::INNER_BOTTOM && j >= conf::INNER_LEFT) {
                    // Inner pipe filled with steam with walls kept at 212°F
                    values[i][j] = 212;
                    values[i][conf::N - j - 1] = 212;

                    isConst[i][j] = true;
                    isConst[i][conf::N - j - 1] = true;
                } else {
                    // In all remaining cases we arbitrarily set temperature to
                    // a 'reasonable' value between 32°F and 212°F - here we do Unif(32,212)
                    values[i][j] = unif(rng);
                    values[i][conf::N - j - 1] = unif(rng);

                    // These are the only values that will not be constant
                    isConst[i][j] = false;
                    isConst[i][conf::N - j - 1] = false;
                }
            }
        }
	}

    static float update(std::vector<std::vector<float>>& values, std::vector<std::vector<bool>>& isConst) {
        // Store the maximal update in the current iteration
        float maxUpdate = FLT_MIN;

        // Update the grid with the magic formula
        for (size_t i = 0; i < conf::N; i++) {
            // Make use of the symmetry around the vertical axis
            for (size_t j = 0; j <= conf::N / 2; j++) {
                // Only apply the formula if the grid cell is not constant
                if (!isConst[i][j]) {
                    // Get neighbouring cells and apply the formula
                    float top = values[i - 1][j];
                    float bottom = values[i + 1][j];
                    float left = values[i][j - 1];
                    float right = values[i][j + 1];

                    float val = 0.25f * (top + bottom + left + right);

                    // Check if value update exceeded current maximum
                    float update = std::abs(values[i][j] - val);
                    maxUpdate = std::max(maxUpdate, update);

                    // Update the values utilizing domain symmetry
                    values[i][j] = val;
                    values[i][conf::N - j - 1] = val;
                }
            }
        }

        return maxUpdate;
    }
}