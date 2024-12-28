#ifndef ENCODE_MESSAGE_H
#define ENCODE_MESSAGE_H

#include <string>
#include <vector>
#include "ImageMatrix.h"

class EncodeMessage {
public:
    static int fibonacci(int number);
    static bool prime(int number);
    static std::string circularShift(const std::string &str);
    EncodeMessage();
    ~EncodeMessage();
    static ImageMatrix encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& edgePixels);

private:
};

#endif
