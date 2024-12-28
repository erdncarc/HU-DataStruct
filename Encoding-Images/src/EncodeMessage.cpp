#include "EncodeMessage.h"
#include <iostream>
#include <bitset>

EncodeMessage::EncodeMessage() = default;

EncodeMessage::~EncodeMessage() = default;

ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>> &edgePixels) {
    std::string changedMsg;
    for (int i = 0; i < message.length(); i++) {
        if (prime(i)) {
            int decimalValue = static_cast<int>(message[i]) + fibonacci(i);
            if (decimalValue > 126) {
                decimalValue = 126;
            }
            changedMsg += static_cast<char>(decimalValue);
        } else {
            changedMsg += message[i];
        }
    }

    changedMsg = circularShift(changedMsg);

    std::string binaryMsg;
    for (char i: changedMsg) {
        std::bitset<7> binary(static_cast<unsigned char>(i));
        binaryMsg += binary.to_string();
    }

    ImageMatrix outputMatrix(img.get_height(), img.get_width());
    for (int i = 0; i < outputMatrix.get_height(); i++) {
        for (int j = 0; j < outputMatrix.get_width(); j++) {
            outputMatrix.get_data()[i][j] = img.get_data(i, j);
        }
    }

    int i = 0;
    for (auto p: edgePixels) {
        if (i < binaryMsg.length()) {
            int pixelValue = static_cast<int>(outputMatrix.get_data(p.first, p.second));
            char binaryValue = binaryMsg[i];

            if (pixelValue % 2 == 0 && binaryValue == '1') {
                outputMatrix.get_data()[p.first][p.second]++;
            } else if (pixelValue % 2 != 0 && binaryValue == '0') {
                outputMatrix.get_data()[p.first][p.second]--;
            }
            i++;
        }
    }
    return outputMatrix;
}

bool EncodeMessage::prime(int number) {
    if (number <= 1) {
        return false;
    } else if (number == 2) {
        return true;
    } else {
        for (int i = 2; i < number; i++) {
            if (number % i == 0) {
                return false;
            }
        }
        return true;
    }
}

int EncodeMessage::fibonacci(int number) {
    int previous = 0;
    int current = 1;
    int next;
    if (number == 0) {
        return 0;
    }
    for (int i = 2; i <= number; i++) {
        next = previous + current;
        previous = current;
        current = next;
    }
    return current;
}

std::string EncodeMessage::circularShift(const std::string &str) {
    std::string::size_type n = str.length();
    if (n == 0) {
        return str;
    }

    int shift = static_cast<int>(n / 2);
    std::string result = str.substr(n - shift) + str.substr(0, n - shift);
    return result;
}
