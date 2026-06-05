#pragma once
#include "PS_ErrorList.hpp"
#include <string_view>
#include <vector>

namespace PistonSlap {
    // Prefixes a "PS" tag for ease of output filtering.
    // Automatically applies a newlines char
    void logStatus(StatusCode status, std::string_view desc);

    // Ensure to prefix with the directory (ex: "models/cube.gltf")
    // returns bool if the file was located
   bool readFile(const std::string &fileName, std::vector<char> &buffer);
}