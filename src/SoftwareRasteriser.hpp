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
            this->m_depthBuffer = new int32_t[height * width];
            clearDepthBuffer();
        }
        ~SoftwareRasteriser() {
            delete[] m_screenBuffer;
            delete[] m_depthBuffer;
        }

        void colour(uint8_t r, uint8_t g, uint8_t b);
        void clearDepthBuffer();
        void load(int** vertices, int num_faces, int** faces, uint8_t** vertex_colours);
        inline int getPixelLocation(int x, int y);
        void render();
        uint8_t *getBuffer();

        int width;
        int height;
        // TODO: Move into structs
        int **vertices = nullptr;
        int **faces = nullptr;
        uint8_t **vertex_colours = nullptr;
        int num_faces = 0;
    private:


        void renderTriangle(int index);

        uint8_t *m_screenBuffer;
        int32_t *m_depthBuffer;
    };

}
#endif //SOFTWARERASTERISER_SOFTWARERASTERISER_HPP
