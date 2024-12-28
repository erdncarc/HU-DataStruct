#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Sector.h"

using namespace std;

class SpaceSectorBST {
public:
	SpaceSectorBST();

	~SpaceSectorBST();

	void readSectorsFromFile(const string &File);

	void insertSectorByCoordinates(int x, int y, int z);

	Sector *insertion(Sector *sector, int x, int y, int z);

	void displaySectorsInOrder();

	void inorder(Sector *sector);

	void displaySectorsPreOrder();

	void preorder(Sector *sector);

	void displaySectorsPostOrder();

	void postorder(Sector *sector);

	vector<Sector *> getStellarPath(const string &code);

	Sector *find(const string &code);

	static void printStellarPath(const vector<Sector *> &path);

	void deleteSector(const string &code);

	Sector *rightLess(Sector *sector);

	Sector *root;
	vector<Sector *> nodes;
};

#endif
