#include "KD_Tree.h"

KD_Tree::KD_Tree() : root(nullptr), split_threshold(0.1) {
}

KD_Tree::KD_Tree(double threshold) : root(nullptr), split_threshold(threshold) {
}

KD_Tree::~KD_Tree() {
}

void KD_Tree::build(Dataset &data) {
}

KDTreeNode *KD_Tree::getRoot() {
	return nullptr;
}
