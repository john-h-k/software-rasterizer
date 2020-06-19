//
// Created by dardledumdue on 19/06/2020.
//

#include "SoftwareRasteriser.hpp"

void sr::SoftwareRasteriser::colour(uint8_t r, uint8_t g, uint8_t b) {
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            this->m_screenBuffer[getPixelLocation(x, y)] = r;
            this->m_screenBuffer[getPixelLocation(x, y)+1] = g;
            this->m_screenBuffer[getPixelLocation(x, y)+2] = b;
        }
    }
}

uint8_t *sr::SoftwareRasteriser::getBuffer() {
    return this->m_screenBuffer;
}

inline int sr::SoftwareRasteriser::getPixelLocation(int x, int y) {
    return (y * width + x) * 3;
}
