#include "PS_Util.hpp"
#include <iostream>
#include <string_view>

static constexpr std::string_view LOG_MSG{"PS Status: "};

namespace PistonSlap {
    void logStatus(const PistonSlap::StatusCode status, std::string_view desc){
        std::cerr << LOG_MSG << status << desc << "\n";
    }
}