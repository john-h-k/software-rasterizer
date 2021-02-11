//
// Created by IlleQuiProgrammat on 19/06/2020.
//

#ifndef SOFTWARERASTERISER_SOFTWARERASTERISER_HPP
#define SOFTWARERASTERISER_SOFTWARERASTERISER_HPP

#include <cstdint>
namespace sr {
    struct Colour {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        Colour(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
        Colour() = default;
        uint8_t& operator[](int a) {
            switch(a) {
                case 0:
                    return r;
                case 1:
                    return g;
                case 2:
                    return b;
                default:
                    return b;
            }
        }
    };

    struct Vec3 {
        int x;
        int y;
        int z;
        Vec3(int x, int y, int z) : x(x), y(y), z(z) {}
        Vec3() = default;
        int& operator[](int a) {
            switch(a) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    return z;
            }
        }
    };

    struct Vec3f {
        float x;
        float y;
        float z;
        Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
        Vec3f() = default;
        float& operator[](int a) {
            switch(a) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    return z;
            }
        }
    };

    class SoftwareRasteriser {
    public:
        SoftwareRasteriser(int width, int height) : width(width), height(height) {
            this->m_screenBuffer = new Colour[height * width];
            this->m_depthBuffer = new int32_t[height * width];
            clearDepthBuffer();
        }
        ~SoftwareRasteriser() {
            delete[] m_screenBuffer;
            delete[] m_depthBuffer;
        }

        void colour(uint8_t r, uint8_t g, uint8_t b);
        void clearDepthBuffer();
        void load(Vec3 *vertices, int num_faces, Vec3 *faces, Colour *vertex_colours);
        inline int getPixelLocation(int x, int y);
        void render();
        uint8_t *getBuffer();

        int width;
        int height;
        Vec3 *vertices = nullptr;
        Vec3 *faces = nullptr;
        Colour *vertex_colours = nullptr;
        int num_faces = 0;
    private:


        void renderTriangle(int index);
        static bool pointWithinTriangle(int x, int y, Vec3& A, Vec3& B, Vec3& C);
        static bool CWCheck(Vec3& A, Vec3& B, Vec3& C);
        static int triangularArea(Vec3& A, Vec3& B, Vec3& C);
        static void barycentric(Vec3& P, Vec3& A, Vec3& B, Vec3& C, Vec3f& result);
        Colour *m_screenBuffer;
        int32_t *m_depthBuffer;
    };

}
#endif //SOFTWARERASTERISER_SOFTWARERASTERISER_HPP
