#ifndef KD_TREE_H
#define KD_TREE_H

#include "KDT_Node.h"
#include "kNN_Data.h"

#include <vector>
#include <iostream>

class KD_Tree {
private:
	KDTreeNode *root;
	double split_threshold;

public:

	KD_Tree();

	KD_Tree(double split_threshold);

	~KD_Tree();

	void build(Dataset &data);

	KDTreeNode *getRoot();

};

#endif
