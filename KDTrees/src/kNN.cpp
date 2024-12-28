#include "kNN.h"

KNN::KNN(int neighbors, double threshold) : k(neighbors), split_threshold(threshold) {
}

void KNN::train(Dataset& data) {
}

int KNN::predict(const Point& queryPoint) {
	return 0;
}