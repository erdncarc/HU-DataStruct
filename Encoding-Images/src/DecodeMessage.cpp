#include <iostream>
#include <bitset>
#include "DecodeMessage.h"

DecodeMessage::DecodeMessage() = default;

DecodeMessage::~DecodeMessage() = default;

std::string DecodeMessage::decodeFromImage(const ImageMatrix &imgData, const std::vector<std::pair<int, int>> &edgePixels) {
    std::string binaryMsg;
    std::string asciiMsg;

    for (auto p: edgePixels) {
        std::bitset<7> binaryForm(static_cast<int>(imgData.get_data()[p.first][p.second]));
        binaryMsg += std::to_string(binaryForm[0]);
    }

    while (static_cast<int>(binaryMsg.length()) % 7 != 0) {
        binaryMsg.insert(0, std::to_string(0));
    }

    for (int i = 0; i < binaryMsg.length(); i += 7) {
        std::bitset<7> bitSet(binaryMsg.substr(i, 7));
        unsigned long long decimalValue = bitSet.to_ullong();
        if (decimalValue <= 32) {
            decimalValue += 33;
        } else if (decimalValue > 126) {
            decimalValue = 126;
        }
        asciiMsg += static_cast<char>(decimalValue);
    }
    return asciiMsg;
}
