#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() = default;

ImageProcessor::~ImageProcessor() = default;

std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening imageSharpening;
    ImageMatrix sharpImg = imageSharpening.sharpen(img, 2);

    EdgeDetector edgeDetector;
    edgePixels = edgeDetector.detectEdges(sharpImg);

    DecodeMessage decodeMessage;
    std::string decodedMessage = decodeMessage.decodeFromImage(sharpImg, edgePixels);

    return decodedMessage;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening imageSharpening;
    ImageMatrix sharpImg = imageSharpening.sharpen(img, 2);

    EdgeDetector edgeDetector;
    edgePixels = edgeDetector.detectEdges(sharpImg);

    EncodeMessage encodeMessage;
    ImageMatrix encodedMessage = encodeMessage.encodeMessageToImage(img, message, edgePixels);

    return encodedMessage;
}
