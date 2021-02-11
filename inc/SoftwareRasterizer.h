//
// Created by durdledumdue on 19/06/2020.
//
// Nathan likes men

#ifndef SoftwareRasterizer_SoftwareRasterizer_HPP
#define SoftwareRasterizer_SoftwareRasterizer_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include "RasterizerDesc.h"

using dim_t = uint32_t;

namespace sr {
    class SoftwareRasterizer {
    public:
        SoftwareRasterizer(const RasterizerDesc& desc) :
            m_screenBuffer(desc.Height, desc.Width),
            m_depthBuffer(desc.Height, desc.Width) {
            clearDepthBuffer();
        }

        void colour(uint8_t r, uint8_t g, uint8_t b);
        void clearDepthBuffer();
        void load(int** vertices, int num_faces, int** faces, uint8_t** vertex_colours);
        inline int getPixelLocation(int x, int y);
        void render();
        uint8_t *getBuffer();

    private:

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

        void renderTriangle(int index);
        bool pointWithinTriangle(int x, int y, int* A, int* B, int* C);
        bool CWCheck(int* A, int* B, int* C);
        int triangularArea(int* A, int* B, int* C);
        void barycentric(int* P, int* A, int* B, int* C, float* result);
        Image<uint32_t> m_screenBuffer;
        Image<float_t> m_depthBuffer;

        RasterizerDesc desc;
        // TODO: Move into structs
        dim_t** vertices = nullptr;
        dim_t** faces = nullptr;
        uint8_t** vertex_colours = nullptr;
        int num_faces = 0;
    };

}
#endif //SoftwareRasterizer_SoftwareRasterizer_HPP
