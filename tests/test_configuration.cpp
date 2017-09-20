#include <cassert>
#include <iostream>

#include "../src/config.hpp"
#include "../src/config.cpp"


int main()
{
    Configuration config;

    config.read("config.ini");

    assert(config.width == 800);
    assert(config.height == 600);
    assert(config.scalefactor == 1);
    assert(config.fullscreen == false);
    assert(config.vsync == true);

    config.read("config2.ini");

    assert(config.width == 685);
    assert(config.height == 534);
    assert(config.scalefactor == 2);
    assert(config.fullscreen == true);
    assert(config.vsync == true);

    std::cout << "Configuration module OK" << std::endl;

    return 0;
}
