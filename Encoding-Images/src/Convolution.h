#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "ImageMatrix.h"

class Convolution {
public:
    Convolution();
    Convolution(double **kernelMatrix, int kernelHeight, int kernelWidth, int stride_,bool pad);
    ImageMatrix convolve(const ImageMatrix &imgData) const;
    ~Convolution();

private:
    double **kernel{};
    int height{};
    int width{};
    int stride{};
    bool padding{};
};

#endif
