#include "PS_Graphics.hpp"
#include <iostream>

namespace PistonSlap{
    Graphics::~Graphics(){
        std::cout << "PS Graphics API shutting down\n";
    };

    
}