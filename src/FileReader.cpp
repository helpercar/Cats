#include "../include/FileReader.hpp"

FileReader::FileReader() {

}

std::vector<std::vector<std::string>> FileReader::read_file(std::string file_path) {
    std::ifstream input_file(file_path);

    if (!input_file.is_open()) {
        std::cout << "No Items Found at: " << file_path << std::endl;
        return {};
    }

    std::vector<std::vector<std::string>> csvData;
    std::string line;

    while (std::getline(input_file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            if (cell.size() >= 2) {
                row.push_back(cell.substr(1, cell.size() - 2));
            } else {
                
                row.push_back(cell);
            }
        }

        csvData.push_back(row);
    }

    input_file.close();

    return csvData;
}