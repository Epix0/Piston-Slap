#include "PS_Util.hpp"
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

static constexpr std::string_view LOG_MSG{"PS Status: "};

namespace PistonSlap {
    void logStatus(PistonSlap::StatusCode status, std::string_view desc){
        std::cout << LOG_MSG << status << " " << desc << "\n";
    }

    // fstream doesn't accept str view since the type doesn't ensure a null char
    bool readFile(const std::string &fileName, std::vector<char> &buffer){
        std::fstream file{fileName, std::ios::in | std::ios::ate | std::ios::binary};

        if (!file.is_open()){
            return false;
        };

        long fileSize = static_cast<long>(file.tellg());
        buffer.resize(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        
        return true;
    }
}