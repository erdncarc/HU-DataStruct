#ifndef SECTOR_H
#define SECTOR_H

#include <iostream>
#include <cmath>

const bool RED = true;
const bool BLACK = false;

using namespace std;

class Sector {
public:
	Sector(int x, int y, int z);

	~Sector();

	int x, y, z;
	double distance_from_earth;
	string sector_code;
	Sector *left, *right, *parent;
	bool color;
	bool isExist = true;
};

#endif
