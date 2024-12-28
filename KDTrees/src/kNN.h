#ifndef KNN_H
#define KNN_H

#include "KD_Tree.h"
#include "kNN_Data.h"
#include <vector>

class KNN {
private:


public:
	KD_Tree tree;
	int k;
	double split_threshold;

	KNN(int k, double threshold);

	void train(Dataset &data);

	int predict(const Point &queryPoint);
};

#endif
