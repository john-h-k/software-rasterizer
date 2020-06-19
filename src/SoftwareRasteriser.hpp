//
// Created by IlleQuiProgrammat on 19/06/2020.
//

#ifndef SOFTWARERASTERISER_SOFTWARERASTERISER_HPP
#define SOFTWARERASTERISER_SOFTWARERASTERISER_HPP

#include <cstdint>
namespace sr {
    class SoftwareRasteriser {
    public:
        SoftwareRasteriser(int width, int height) : width(width), height(height) {
            // 3 is for the colour profile
            this->m_screenBuffer = new uint8_t[height * width * 3];
        }

        void colour(uint8_t r, uint8_t g, uint8_t b);
        inline int getPixelLocation(int x, int y);

        uint8_t *getBuffer();

        int width;
        int height;
    private:
        uint8_t *m_screenBuffer;
    };

}
#endif //SOFTWARERASTERISER_SOFTWARERASTERISER_HPP
