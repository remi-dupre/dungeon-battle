#include <cassert>

#include "../src/config.hpp"


int main()
{
    Configuration config;

    parse_configuration(config, "config.ini");

    assert(config.width == 800);
    std::cout << "config.width: Ok" << std::endl;

    assert(config.height == 600);
    std::cout << "config.height: Ok" << std::endl;

    assert(config.fullscreen == false);
    std::cout << "config.fullscreen: Ok" << std::endl;

    assert(config.vsync == true);
    std::cout << "config.vsync: Ok" << std::endl;

    return 0;
}