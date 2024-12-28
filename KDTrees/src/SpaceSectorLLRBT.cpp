#include "SpaceSectorLLRBT.h"

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

SpaceSectorLLRBT::~SpaceSectorLLRBT() {
	for (Sector *s: nodes) {
		delete s;
	}
}

void SpaceSectorLLRBT::readSectorsFromFile(const string &File) {
	ifstream file(File);

	if (file.is_open()) {
		string line;

		getline(file, line);
		while (getline(file, line)) {
			replace(line.begin(), line.end(), ',', ' ');
			stringstream ss(line);
			int x, y, z;

			if (ss >> x >> y >> z) {
				insertSectorByCoordinates(x, y, z);
				root = balance(nodes.back());
			}
		}
		file.close();
	}
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
	root = insertion(root, x, y, z);
}

Sector *SpaceSectorLLRBT::insertion(Sector *sector, int x, int y, int z) {
	if (sector == nullptr) {
		sector = new Sector(x, y, z);
		nodes.push_back(sector);

	} else {
		bool control;
		if (x != sector->x) control = x > sector->x;
		else if (y != sector->y) control = y > sector->y;
		else if (z != sector->z) control = z > sector->z;
		else return sector;

		if (control) {
			sector->right = insertion(sector->right, x, y, z);
			sector->right->parent = sector;
		} else {
			sector->left = insertion(sector->left, x, y, z);
			sector->left->parent = sector;
		}
	}
	return sector;
}

Sector *SpaceSectorLLRBT::balance(Sector *child) {
	if (child->parent == nullptr) {
		child->color = BLACK;
		return child;
	}

	Sector *parent = child->parent;
	bool isRightP = parent->right != nullptr && child->sector_code == parent->right->sector_code;

	if (parent->right != nullptr && parent->left != nullptr) {
		colorFlips(isRightP, child, parent);
	}

	if (isRightP && child->color) {
		rotateL(child, parent);

	} else if (child->color && parent->color && parent->parent != nullptr) {
		rotateR(parent, parent->parent);
		return balance(child);
	}
	return balance(parent);
}

void SpaceSectorLLRBT::colorFlips(bool isRight, Sector *child, Sector *parent) {
	Sector *bro = isRight ? parent->left : parent->right;

	if (child->color && bro->color) {
		child->color = BLACK;
		bro->color = BLACK;
		parent->color = RED;
	}
}

void SpaceSectorLLRBT::rotateR(Sector *parent, Sector *grandparent) {
	swap(grandparent->left, parent->right);
	swap(parent->color, grandparent->color);
	parent->right = grandparent;
	parent->parent = grandparent->parent;
	grandparent->parent = parent;

	if (grandparent->left != nullptr) grandparent->left->parent = grandparent;

	if (parent->parent != nullptr && parent->parent->right != nullptr && grandparent->sector_code == parent->parent->right->sector_code) parent->parent->right = parent;
	else if (parent->parent != nullptr) parent->parent->left = parent;
}

void SpaceSectorLLRBT::rotateL(Sector *child, Sector *parent) {
	swap(parent->right, parent->parent);
	swap(parent->right, child->left);
	swap(child->left, child->parent);
	swap(child->color, parent->color);

	if (parent->right != nullptr) parent->right->parent = parent;

	if (child->parent != nullptr && child->parent->right != nullptr && parent->sector_code == child->parent->right->sector_code) child->parent->right = child;
	else if (child->parent != nullptr) child->parent->left = child;
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
	cout << "Space sectors inorder traversal:" << endl;
	inorder(root);
	cout << endl;
}

void SpaceSectorLLRBT::inorder(Sector *sector) {
	if (sector == nullptr) return;
	inorder(sector->left);
	cout << (sector->color ? "RED" : "BLACK") << " sector: " << sector->sector_code << endl;
	inorder(sector->right);
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
	cout << "Space sectors preorder traversal:" << endl;
	preorder(root);
	cout << endl;
}

void SpaceSectorLLRBT::preorder(Sector *sector) {
	if (sector == nullptr) return;
	cout << (sector->color ? "RED" : "BLACK") << " sector: " << sector->sector_code << endl;
	preorder(sector->left);
	preorder(sector->right);
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
	cout << "Space sectors postorder traversal:" << endl;
	postorder(root);
	cout << endl;
}

void SpaceSectorLLRBT::postorder(Sector *sector) {
	if (sector == nullptr) return;
	postorder(sector->left);
	postorder(sector->right);
	cout << (sector->color ? "RED" : "BLACK") << " sector: " << sector->sector_code << endl;
}

vector<Sector *> SpaceSectorLLRBT::getStellarPath(const string &code) {
	vector<Sector *> path;
	vector<Sector *> pathEarth = findPath(nodes.front());
	vector<Sector *> pathWanted = findPath(find(code));
	if (pathWanted.empty() || pathEarth.empty()) return path;
	reverse(pathWanted.begin(), pathWanted.end());

	for (int i = 0; i < pathEarth.size(); i++) {
		for (int j = 0; j < pathWanted.size(); j++) {
			if (pathEarth[i]->sector_code == pathWanted[j]->sector_code) {
				path = vector<Sector *>(pathEarth.begin(), pathEarth.begin() + i);
				vector<Sector *> temp(pathWanted.begin() + j, pathWanted.end());
				path.insert(path.end(), temp.begin(), temp.end());
				return path;
			}
		}
	}
	return path;
}

Sector *SpaceSectorLLRBT::find(const string &code) {
	for (Sector *s: nodes) {
		if (s->sector_code == code && s->isExist) {
			return s;
		}
	}
	return nullptr;
}

vector<Sector *> SpaceSectorLLRBT::findPath(Sector *sector) {
	vector<Sector *> path;

	while (sector != nullptr) {
		path.push_back(sector);
		sector = sector->parent;
	}
	return path;
}

void SpaceSectorLLRBT::printStellarPath(const vector<Sector *> &path) {
	if (path.empty()) {
		cout << "A path to Dr. Elara could not be found." << endl;
	} else {
		cout << "The stellar path to Dr. Elara: ";
		for (int i = 0; i < path.size(); i++) {
			cout << path[i]->sector_code << ((i == path.size() - 1) ? "\n" : "->");
		}
	}
}
