#ifndef IMAGE_MATRIX_H
#define IMAGE_MATRIX_H

#include <string>
#include "ImageLoader.h"

class ImageMatrix {
public:
    ImageMatrix();
    explicit ImageMatrix(const std::string &filepath);
    ImageMatrix(const double **imgMatrix, int imgHeight, int imgWidth);
    ImageMatrix(int imgHeight, int imgWidth);
    ImageMatrix(const ImageMatrix &other);
    ~ImageMatrix();

    ImageMatrix operator+(const ImageMatrix &other) const;
    ImageMatrix operator-(const ImageMatrix &other) const;
    ImageMatrix operator*(const double &scalar) const;

    double **get_data() const;
    double get_data(int i, int j) const;
    int get_height() const;
    int get_width() const;

private:
    double **data{};
    int height{};
    int width{};
};

#endif
