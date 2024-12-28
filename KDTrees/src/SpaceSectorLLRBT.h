#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Sector.h"

using namespace std;

class SpaceSectorLLRBT {
public:
	SpaceSectorLLRBT();

	~SpaceSectorLLRBT();

	void readSectorsFromFile(const string &File);

	void insertSectorByCoordinates(int x, int y, int z);

	Sector *insertion(Sector *sector, int x, int y, int z);

	Sector *balance(Sector *child);

	static void colorFlips(bool isRight, Sector *child, Sector *parent);

	static void rotateR(Sector *parent, Sector *grandparent);

	static void rotateL(Sector *child, Sector *parent);

	void displaySectorsInOrder();

	void inorder(Sector *sector);

	void displaySectorsPreOrder();

	void preorder(Sector *sector);

	void displaySectorsPostOrder();

	void postorder(Sector *sector);

	vector<Sector *> getStellarPath(const string &code);

	Sector *find(const string &code);

	vector<Sector *> findPath(Sector *sector);

	static void printStellarPath(const vector<Sector *> &path);

	Sector *root;
	vector<Sector *> nodes;
};

#endif
