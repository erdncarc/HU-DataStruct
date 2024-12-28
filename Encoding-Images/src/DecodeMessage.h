#ifndef DECODE_MESSAGE_H
#define DECODE_MESSAGE_H

#include <string>
#include "ImageMatrix.h"

class DecodeMessage {
public:
    DecodeMessage();
    ~DecodeMessage();
    static std::string decodeFromImage(const ImageMatrix &imgData, const std::vector<std::pair<int, int>> &edgePixels);

private:
};

#endif
