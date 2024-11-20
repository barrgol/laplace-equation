#pragma once
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include "configuration.hpp"

namespace CSV {

    // Function to generate a .csv filename for a specific timestep
    std::string generateFilename(int timestep) {
        std::ostringstream filename;
        filename << "../data/simulation_timestep_"
            << std::setw(5) << std::setfill('0')  // Set width to 5 and fill with zeros
            << timestep << ".csv";
        return filename.str();
    }

    // Function to write a single CSV file for one timestep
    void write(const std::string& filename, const std::vector<std::vector<float>>& data, const std::string& variableName) {
        // Create and open the file
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing!" << std::endl;
            return;
        }

        // Write the header
        file << "x, y, " << variableName << "\n";

        // Write the data
        for (size_t i = 0; i < conf::N; i++) {
            for (size_t j = 0; j <= conf::N / 2; j++) {
                file << i << ", " << j << ", " << data[i][j] << "\n";
            }
        }

        file.close();
    }
}