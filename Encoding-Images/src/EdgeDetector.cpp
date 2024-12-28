#include "EdgeDetector.h"
#include <cmath>

EdgeDetector::EdgeDetector() = default;

EdgeDetector::~EdgeDetector() = default;

std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix &imgData) {
    std::vector<std::pair<int, int>> edgePixels;

    double *gx[3] = {
            new double[3]{-1, 0, 1},
            new double[3]{-2, 0, 2},
            new double[3]{-1, 0, 1}
    };
    Convolution kernelX(gx, 3, 3, 1, true);
    ImageMatrix convX = kernelX.convolve(imgData);

    double *gy[3] = {
            new double[3]{-1, -2, -1},
            new double[3]{0, 0, 0},
            new double[3]{1, 2, 1}
    };
    Convolution kernelY(gy, 3, 3, 1, true);
    ImageMatrix convY = kernelY.convolve(imgData);

    ImageMatrix gradient = ImageMatrix(imgData.get_height(), imgData.get_width());

    double total = 0;
    double divisor = 0;
    for (int i = 0; i < imgData.get_height(); i++) {
        for (int j = 0; j < imgData.get_width(); j++) {
            double result = sqrt(convX.get_data(i, j) * convX.get_data(i, j) + convY.get_data(i, j) * convY.get_data(i, j));
            gradient.get_data()[i][j] = result;
            total += result;
            divisor++;
        }
    }

    double average = total / divisor;
    for (int i = 0; i < imgData.get_height(); i++) {
        for (int j = 0; j < imgData.get_width(); j++) {
            if (gradient.get_data()[i][j] > average) {
                edgePixels.emplace_back(i, j);
            }
        }
    }
    return edgePixels;
}
