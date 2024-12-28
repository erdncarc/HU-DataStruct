#include "SpaceSectorBST.h"

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
	for (Sector *s: nodes) {
		delete s;
	}
}

void SpaceSectorBST::readSectorsFromFile(const string &File) {
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
			}
		}
		file.close();
	}
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
	root = insertion(root, x, y, z);
}

Sector *SpaceSectorBST::insertion(Sector *sector, int x, int y, int z) {
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

void SpaceSectorBST::displaySectorsInOrder() {
	cout << "Space sectors inorder traversal:" << endl;
	inorder(root);
	cout << endl;
}

void SpaceSectorBST::inorder(Sector *sector) {
	if (sector == nullptr) return;
	inorder(sector->left);
	cout << sector->sector_code << endl;
	inorder(sector->right);
}

void SpaceSectorBST::displaySectorsPreOrder() {
	cout << "Space sectors preorder traversal:" << endl;
	preorder(root);
	cout << endl;
}

void SpaceSectorBST::preorder(Sector *sector) {
	if (sector == nullptr) return;
	cout << sector->sector_code << endl;
	preorder(sector->left);
	preorder(sector->right);
}

void SpaceSectorBST::displaySectorsPostOrder() {
	cout << "Space sectors postorder traversal:" << endl;
	postorder(root);
	cout << endl;
}

void SpaceSectorBST::postorder(Sector *sector) {
	if (sector == nullptr) return;
	postorder(sector->left);
	postorder(sector->right);
	cout << sector->sector_code << endl;
}

vector<Sector *> SpaceSectorBST::getStellarPath(const string &code) {
	Sector *wanted = find(code);
	vector < Sector * > path;

	while (wanted != nullptr) {
		path.push_back(wanted);
		wanted = wanted->parent;
	}
	reverse(path.begin(), path.end());

	return path;
}

Sector *SpaceSectorBST::find(const string &code) {
	for (Sector *s: nodes) {
		if (s->sector_code == code && s->isExist) {
			return s;
		}
	}
	return nullptr;
}

void SpaceSectorBST::printStellarPath(const vector<Sector *> &path) {
	if (path.empty()) {
		cout << "A path to Dr. Elara could not be found." << endl;
	} else {
		cout << "The stellar path to Dr. Elara: ";
		for (int i = 0; i < path.size(); i++) {
			cout << path[i]->sector_code << ((i == path.size() - 1) ? "\n" : "->");
		}
	}
}

void SpaceSectorBST::deleteSector(const string &code) {
	Sector *wanted = find(code);
	if (wanted == nullptr) return;

	bool isRoot = wanted->sector_code == root->sector_code;
	bool isRight = !isRoot && wanted->parent->right != nullptr && wanted->sector_code == wanted->parent->right->sector_code;

	Sector *temp;
	if (wanted->left == nullptr && wanted->right == nullptr) {
		temp = nullptr;

	} else if (wanted->left == nullptr) {
		temp = wanted->right;
		wanted->right->parent = wanted->parent;

	} else if (wanted->right == nullptr) {
		temp = wanted->left;
		wanted->left->parent = wanted->parent;

	} else {
		Sector *rl = rightLess(wanted->right);
		swap(wanted->distance_from_earth, rl->distance_from_earth);
		swap(wanted->x, rl->x);
		swap(wanted->y, rl->y);
		swap(wanted->z, rl->z);
		swap(wanted->sector_code, rl->sector_code);

		deleteSector(rl->sector_code);
		return;
	}

	if (isRoot) root = temp;
	else if (isRight) wanted->parent->right = temp;
	else wanted->parent->left = temp;
	wanted->isExist = false;
}

Sector *SpaceSectorBST::rightLess(Sector *sector) {
	if (sector->left == nullptr) return sector;
	else return rightLess(sector->left);
}
