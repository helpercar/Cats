#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <filesystem>

namespace fs = std::filesystem;

class FileReader {
public:
    FileReader();
    std::vector<std::vector<std::string>> read_file(std::string file_path);

};