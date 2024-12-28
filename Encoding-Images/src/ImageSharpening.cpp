#include "ImageSharpening.h"

ImageSharpening::ImageSharpening() = default;

ImageMatrix ImageSharpening::sharpen(const ImageMatrix &imgData, double k) {
    double *kernelMatrix[3] = {
            new double[3]{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
            new double[3]{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
            new double[3]{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
    };

    Convolution kernel(kernelMatrix, 3, 3, 1, true);
    ImageMatrix noisyImg = kernel.convolve(imgData);

    ImageMatrix sharpImg = ImageMatrix(imgData.get_height(), imgData.get_width());

    for (int i = 0; i < imgData.get_height(); i++) {
        for (int j = 0; j < imgData.get_width(); j++) {
            double result = imgData.get_data()[i][j] + k * (imgData.get_data()[i][j] - noisyImg.get_data()[i][j]);
            if (result < 0) {
                sharpImg.get_data()[i][j] = 0;
            } else if (result > 255) {
                sharpImg.get_data()[i][j] = 255;
            } else {
                sharpImg.get_data()[i][j] = result;
            }
        }
    }
    return sharpImg;
}

ImageSharpening::~ImageSharpening() = default;
