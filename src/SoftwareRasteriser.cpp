//
// Created by dardledumdue on 19/06/2020.
//

#include "SoftwareRasteriser.hpp"

#include <algorithm>
#include <iostream>

uint32_t pack4(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return r | (g << 8) | (b << 16) | (a << 24);
}

void sr::SoftwareRasteriser::colour(uint8_t r, uint8_t g, uint8_t b) {
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            auto colour = pack4(r, g, b, 1);
            this->m_screenBuffer(x, y) = colour;
        }
    }
}

uint8_t *sr::SoftwareRasteriser::getBuffer() {
    return (uint8_t*)&this->m_screenBuffer(0, 0);
}

void sr::SoftwareRasteriser::render() {
    clearDepthBuffer();
    colour( 0, 0, 0);
    for (int i = 0; i < this->num_faces; i++) {
        renderTriangle(i);
    }
}

void sr::SoftwareRasteriser::load(Vec3 *vertices, int num_faces, Vec3 *faces, Colour *vertex_colours) {
    this->vertices = vertices;
    this->faces = faces;
    this->num_faces = num_faces;
    this->vertex_colours = vertex_colours;
}

void sr::SoftwareRasteriser::renderTriangle(int index) {
    Vec3 triangleVertices = this->faces[index];

    // Avoid inverted rendering
    if (triangularArea(this->vertices[triangleVertices.x], this->vertices[triangleVertices.y],
            this->vertices[triangleVertices.z]) > 0) {
        return;
    }

    int x_min = this->width;
    int y_min = this->height;
    int x_max = 0;
    int y_max = 0;
    for (int i = 0; i < 3; i++) {
        x_min = std::min(x_min, this->vertices[triangleVertices[i]].x);
        y_min = std::min(y_min, this->vertices[triangleVertices[i]].y);
        x_max = std::max(x_max, this->vertices[triangleVertices[i]].x);
        y_max = std::max(y_max, this->vertices[triangleVertices[i]].y);
    }
    x_min = std::max(0, x_min);
    y_min = std::max(0, y_min);
    x_max = std::min(this->width, x_max);
    y_max = std::min(this->height, y_max);


    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            Vec3f bary = {};
            Vec3 P = {x, y, 1};
            barycentric(P, this->vertices[triangleVertices.x], this->vertices[triangleVertices.y],
                    this->vertices[triangleVertices.z], bary);

            if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0) {
                auto r =
                        (float)this->vertex_colours[triangleVertices[0]][0] * bary.x +
                        (float)this->vertex_colours[triangleVertices[1]][0] * bary.y +
                        (float)this->vertex_colours[triangleVertices[2]][0] * bary.z;
                auto g =
                        (float)this->vertex_colours[triangleVertices[0]][1] * bary[0] +
                        (float)this->vertex_colours[triangleVertices[1]][1] * bary[1] +
                        (float)this->vertex_colours[triangleVertices[2]][1] * bary[2];
                auto b =
                        (float)this->vertex_colours[triangleVertices[0]][2] * bary[0] +
                        (float)this->vertex_colours[triangleVertices[1]][2] * bary[1] +
                        (float)this->vertex_colours[triangleVertices[2]][2] * bary[2];

                auto colour = pack4(r, g, b, 1);
                this->m_screenBuffer(x, y) = colour;
            }
        }
    }
}

template <typename T>
struct template_resolver
{
};

template <template <typename > class C, typename T>
struct template_resolver<C<T>>
{
    using limits = std::numeric_limits<T>;
};

void sr::SoftwareRasteriser::clearDepthBuffer() {

    using limits = template_resolver<decltype(this->m_depthBuffer)>::limits;

    this->m_depthBuffer.fill(limits::min());
}

bool sr::SoftwareRasteriser::pointWithinTriangle(int x, int y, Vec3& A, Vec3& B, Vec3& C) {
    Vec3 P = {x, y, 1};
    return CWCheck(P, B, C) && CWCheck(P, C, A) && CWCheck(P, A, B);
}

bool sr::SoftwareRasteriser::CWCheck(Vec3& A, Vec3& B, Vec3& C) {
    return triangularArea(A, B, C) < 0;
}

int sr::SoftwareRasteriser::triangularArea(Vec3& A, Vec3& B, Vec3& C) {
    return (B.x - A.x)*(B.y + A.y) + (C.x - B.x)*(B.y + C.y) + (A.x - C.x)*(A.y + C.y);
}

void sr::SoftwareRasteriser::barycentric(Vec3& P, Vec3& A, Vec3& B, Vec3& C, Vec3f& result) {
    float tot = triangularArea(A, B, C);
    result.x = float(triangularArea(P, B, C)) / tot;
    result.y = float(triangularArea(P, C, A)) / tot;
    result.z = float(triangularArea(P, A, B)) / tot;
}
