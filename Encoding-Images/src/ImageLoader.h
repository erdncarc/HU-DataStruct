#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <string>
#include <vector>

class ImageLoader {
public:
    explicit ImageLoader(const std::string& filepath);
    ImageLoader(const ImageLoader &other);
    ~ImageLoader();

    ImageLoader& operator=(const ImageLoader &other);

    int getHeight() const;
    int getWidth() const;
    double** getImageData() const;

private:
    int height;
    int width;
    double** imageData;
};

#endif