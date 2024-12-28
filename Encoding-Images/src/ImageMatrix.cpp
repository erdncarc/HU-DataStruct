#include "ImageMatrix.h"

ImageMatrix::ImageMatrix() = default;

ImageMatrix::ImageMatrix(int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth) {
    data = new double *[height];
    for (int i = 0; i < height; i++) {
        data[i] = new double[width];
    }
}

ImageMatrix::ImageMatrix(const double **imgMatrix, int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth) {
    data = new double *[height];
    for (int i = 0; i < height; i++) {
        data[i] = new double[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imgMatrix[i][j];
        }
    }
}

int ImageMatrix::get_height() const {
    return height;
}

int ImageMatrix::get_width() const {
    return width;
}

double ImageMatrix::get_data(int i, int j) const {
    return data[i][j];
}

double **ImageMatrix::get_data() const {
    return data;
}

ImageMatrix ImageMatrix::operator+(const ImageMatrix &other) const {
    ImageMatrix result(height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            result.get_data()[i][j] = this->get_data()[i][j] + other.get_data()[i][j];
        }
    }
    return result;
}

ImageMatrix ImageMatrix::operator-(const ImageMatrix &other) const {
    ImageMatrix result(height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            result.get_data()[i][j] = this->get_data()[i][j] - other.get_data()[i][j];
        }
    }
    return result;
}

ImageMatrix ImageMatrix::operator*(const double &scalar) const {
    ImageMatrix result(height, width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            result.get_data()[i][j] = scalar * this->get_data()[i][j];
        }
    }
    return result;
}

ImageMatrix::ImageMatrix(const std::string &filepath) {
    ImageLoader imageLoader(filepath);

    height = imageLoader.getHeight();
    width = imageLoader.getWidth();

    data = new double *[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
    }

    double **imageData = imageLoader.getImageData();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imageData[i][j];
        }
    }
}

ImageMatrix::~ImageMatrix() {
    if (data != nullptr) {
        for (int i = 0; i < height; i++) {
            delete[] data[i];
        }
        delete[] data;
    }
}

ImageMatrix::ImageMatrix(const ImageMatrix &other) {
    data = new double *[other.height];
    for (int i = 0; i < other.height; i++) {
        data[i] = new double[other.width];
    }

    for (int i = 0; i < other.height; i++) {
        for (int j = 0; j < other.width; j++) {
            get_data()[i][j] = other.get_data()[i][j];
        }
    }
}
