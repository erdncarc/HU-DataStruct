#ifndef IMAGE_SHARPENING_H
#define IMAGE_SHARPENING_H

#include "ImageMatrix.h"
#include "Convolution.h"

class ImageSharpening {
public:
    ImageSharpening();
    ~ImageSharpening();
    static ImageMatrix sharpen(const ImageMatrix &imgData, double k);

private:
};

#endif
