/**
* \file lighting.hpp
* \brief Says if a cell can be seen.
*/

#pragma once

#include <cmath>

#include "map.hpp"

float from_x_to_y(float a,float b,float c,float d,float x);

float from_y_to_x(float a,float b,float c,float d,float y);

bool can_be_seen(sf::Vector2i pos1, sf::Vector2i pos2, const Map& map);
