#include "PS_Window.hpp"
#include "PS_Util.hpp"
#include <memory>
#include <vector>
#include <iostream>

int main(int argCount, char *arrArgValue[])
{
    auto pWindowInstance = std::make_unique<PistonSlap::Window>();

    auto status = pWindowInstance->initWrangler();

	pWindowInstance->initWindow(800, 800, "piston-slap-dbg");

    std::vector<char> buffer{0};
    bool passed{false};
    passed = PistonSlap::readFile("shaders/default_shader.frag", buffer);

    if (passed){
        std::cout << std::string_view{buffer.data(), buffer.size()};
    } else {
        std::cout << "YO this shit failed\n";
    }

    while (!pWindowInstance->getShouldWindowClose()) {
		
        pWindowInstance->pollEvents();
    }

    return 0;
}