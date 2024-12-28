#include "Sector.h"

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
	distance_from_earth = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	string first = x > 0 ? "R" : "L";
	string second = y > 0 ? "U" : "D";
	string third = z > 0 ? "F" : "B";
	if (x == 0) first = "S";
	if (y == 0) second = "S";
	if (z == 0) third = "S";
	sector_code = to_string((int) distance_from_earth) + first + second + third;
}

Sector::~Sector() = default;
