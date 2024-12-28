#include "Convolution.h"

Convolution::Convolution() = default;

Convolution::Convolution(double **kernelMatrix, int kernelHeight, int kernelWidth, int stride_, bool pad)
        : kernel(kernelMatrix), height(kernelHeight), width(kernelWidth), stride(stride_), padding(pad) {
}

ImageMatrix Convolution::convolve(const ImageMatrix &imgData) const {
    ImageMatrix outputData = ImageMatrix(((imgData.get_height() - height + 2 * padding) / stride) + 1, ((imgData.get_width() - width + 2 * padding) / stride) + 1);
    ImageMatrix processedData = padding ? ImageMatrix(imgData.get_height() + 2, imgData.get_width() + 2) : ImageMatrix(imgData.get_height(), imgData.get_width());

    for (int i = 0; i < processedData.get_height(); i++) {
        for (int j = 0; j < processedData.get_width(); j++) {
            if (padding) {
                if (i == 0 || i == processedData.get_height() - 1 || j == 0 || j == processedData.get_width() - 1) {
                    processedData.get_data()[i][j] = 0;
                } else {
                    processedData.get_data()[i][j] = imgData.get_data(i - 1, j - 1);
                }
            } else {
                processedData.get_data()[i][j] = imgData.get_data(i, j);
            }
        }
    }

    for (int i = 0; i < processedData.get_height(); i += stride) {
        for (int j = 0; j < processedData.get_width(); j += stride) {
            double total = 0;
            bool control = true;

            for (int k = 0; k < height; k++) {
                for (int l = 0; l < width; l++) {
                    if (k + i >= processedData.get_height() || l + j >= processedData.get_width()) {
                        control = false;
                    } else {
                        total += processedData.get_data()[k + i][l + j] * kernel[k][l];
                    }
                }
            }
            if (control) {
                outputData.get_data()[i / stride][j / stride] = total;
            }
        }
    }
    return outputData;
}

Convolution::~Convolution() = default;
