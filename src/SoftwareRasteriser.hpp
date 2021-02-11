//
// Created by IlleQuiProgrammat on 19/06/2020.
//

#ifndef SOFTWARERASTERISER_SOFTWARERASTERISER_HPP
#define SOFTWARERASTERISER_SOFTWARERASTERISER_HPP

#include <cstdint>
#include <cmath>
#include <vector>


using dim_t = uint32_t;

namespace sr {
    template <typename TElement>
    class Image
    {
        using dim_t = uint32_t;
    public:
        Image(dim_t width, dim_t y)
            : m_width(width), m_height(height), m_data(width * height)
        {
        }

        inline void fill(TElement&& value)
        {
            for (auto& elem : m_data)
            {
                elem = value;
            }
        }

        inline TElement& operator() (dim_t x, dim_t y)
        {
            auto p = m_data.data();
            auto pRow = p + (y * width);
            return pRow + x;
        }
        inline dim_t width() { return m_width; }
        inline dim_t height() { return m_height; }
    private:
        dim_t m_width, m_height;
        std::vector<TElement> m_data;
    };

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
        SoftwareRasteriser(dim_t height, dim_t width) :
            m_screenBuffer(height, width),
            m_depthBuffer(height, width) {
            clearDepthBuffer();
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
        Image<uint32_t> m_screenBuffer;
        Image<float_t> m_depthBuffer;

        void renderTriangle(int index);
        static bool pointWithinTriangle(int x, int y, Vec3& A, Vec3& B, Vec3& C);
        static bool CWCheck(Vec3& A, Vec3& B, Vec3& C);
        static int triangularArea(Vec3& A, Vec3& B, Vec3& C);
        static void barycentric(Vec3& P, Vec3& A, Vec3& B, Vec3& C, Vec3f& result);
    };

}
#endif //SOFTWARERASTERISER_SOFTWARERASTERISER_HPP
