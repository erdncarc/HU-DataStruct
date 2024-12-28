#ifndef kNN_Data_H
#define kNN_Data_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Point {
	Point(const std::vector<double> &f, std::string l = "") : features(f), label(std::move(l)) {}

	std::vector<double> features;
	std::string label;
};

struct Dataset {
	Dataset() {};

	Dataset(const std::vector<Point> &p, const std::vector<std::string> &h) : points(p), header(h) {}

	std::vector<Point> points;
	std::vector<std::string> header;
	double threshold;
};

#endif
