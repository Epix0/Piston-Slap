#pragma once
#include "PS_ErrorList.hpp"
#include <string_view>

namespace PistonSlap {
    void logStatus(const PistonSlap::StatusCode status, std::string_view desc);
}