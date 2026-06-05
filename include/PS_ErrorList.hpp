#pragma once
// HEADER ONLY
#include <cstdint>
#include <iosfwd>
#include <string>

namespace PistonSlap {
    enum class StatusCode : std::uint16_t {
        OK = 0,

        // GLFW
        GLFW_Window_Init_Failed = 1,
        GLFW_MAIN_INIT_FAILED = 2,
        GLFW_RESERVED = 199,

        // Vulkan
        VULKAN_ = 200,

        VULKAN_RESERVED = 299,


        // Assimp
        ASSIMP_MODEL_IMPORT_FAILED = 300,

        ASSIMP_RESERVED = 399,

    };
    
    inline std::ostream& operator<<(std::ostream& os, const StatusCode& status){
        os << std::to_string(static_cast<std::uint16_t>(status));
        return os;
    };
};