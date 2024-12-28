#ifndef KD_TREE_NODES_H
#define KD_TREE_NODES_H

#include "kNN_Data.h"
#include <string>
#include <utility>
#include <vector>

class KDTreeNode {
public:
	virtual ~KDTreeNode() {};

	virtual bool isLeaf() const = 0;

};

class kd_tree_inter_node : public KDTreeNode {
public:
	int split_dimension;
	double split_value;
	KDTreeNode *left;
	KDTreeNode *right;

	kd_tree_inter_node(int split_dimension, double split_value) : split_dimension(split_dimension), split_value(split_value), left(nullptr), right(nullptr) {}

	bool isLeaf() const override {
		return false;
	}

	~kd_tree_inter_node() override {
	}
};

class kd_tree_leaf_node : public KDTreeNode {
public:
	Dataset data;

	kd_tree_leaf_node(Dataset dataLabelPairs) : data(std::move(dataLabelPairs)) {}

	bool isLeaf() const override {
		return true;
	}

	~kd_tree_leaf_node() override {

	}
};

#endif
