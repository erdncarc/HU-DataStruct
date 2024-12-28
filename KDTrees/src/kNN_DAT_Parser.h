#ifndef KNN_DAT_PARSER_H
#define KNN_DAT_PARSER_H

#include "kNN_Data.h"

#include <fstream>
#include <sstream>
#include <string>

class kNN_Dat_Parser {
public:
	Dataset parse(const std::string &filename) {
		std::ifstream file(filename);
		std::string line;
		Dataset dataset;
		bool readingData = false;
		if (!file.is_open()) {
			std::cerr << "Unable to open file: " << filename << std::endl;
			return dataset;
		}
		while (getline(file, line)) {
			if (line.empty() || line[0] == '#') {
				if (line.find("Threshold") != std::string::npos) {
					if (getline(file, line)) {
						dataset.threshold = std::stod(line);
					}
				}
				continue;
			}
			if (!readingData) {
				std::istringstream iss(line);
				std::string headerItem;
				while (getline(iss, headerItem, ',')) {
					dataset.header.push_back(headerItem);
				}
				readingData = true;
			} else {
				std::istringstream iss(line);
				std::vector<double> features;
				std::string feature;
				std::string label;
				while (getline(iss, feature, ',')) {
					if (std::istringstream(feature) >> std::ws && iss.peek() == std::istringstream::traits_type::eof()) {
						label = feature;
					} else {
						features.push_back(std::stod(feature));
					}
				}
				dataset.points.emplace_back(features, label);
			}
		}
		file.close();
		return dataset;
	}
};

#endif
