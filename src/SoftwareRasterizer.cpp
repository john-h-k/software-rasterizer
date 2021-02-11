//
// Created by dardledumdue on 19/06/2020.
//

#include "SoftwareRasterizer.h"

#include <algorithm>
#include <iostream>

uint32_t pack4(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return r | (g << 8) | (b << 16) | (a << 24);
}


void sr::SoftwareRasterizer::colour(uint8_t r, uint8_t g, uint8_t b) {
    m_screenBuffer.fill(pack4(r, g, b, 1));
}

void sr::SoftwareRasterizer::render() {
    clearDepthBuffer();
    colour(0, 0, 0);
    for (int i = 0; i < this->num_faces; i++) {
        renderTriangle(i);
    }
}

void sr::SoftwareRasterizer::load(dim_t **vertices, dim_t num_faces, dim_t **faces, uint8_t **vertex_colours) {
    this->vertices = vertices;
    this->faces = faces;
    this->num_faces = num_faces;
    this->vertex_colours = vertex_colours;
}

void sr::SoftwareRasterizer::renderTriangle(dim_t index) {
    dim_t* triangleVertices = this->faces[index];
    dim_t x_min = this->m_screenBuffer.width();
    dim_t y_min = this->m_screenBuffer.height();
    dim_t x_max = 0;
    dim_t y_max = 0;

    for (dim_t i = 0; i < 3; i++) {
        x_min = std::min(x_min, this->vertices[triangleVertices[i]][0]);
        y_min = std::min(y_min, this->vertices[triangleVertices[i]][1]);
        x_max = std::max(x_max, this->vertices[triangleVertices[i]][0]);
        y_max = std::max(y_max, this->vertices[triangleVertices[i]][1]);
    }

    x_min = std::max(0u, x_min);
    y_min = std::max(0u, y_min);

    x_max = std::min(this->m_screenBuffer.width(), x_max);
    y_max = std::min(this->m_screenBuffer.height(), y_max);

    bool state = false;

    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            if ((x+y)%16 == 0) state = !state;
            float bary[3];
            int P[3] = {x, y, 1};
            barycentric(P, this->vertices[triangleVertices[0]], this->vertices[triangleVertices[1]], this->vertices[triangleVertices[2]], bary);
            if (bary[0] >= 0 && bary[1] >= 0 && bary[2] >= 0) {
                // TODO: Neaten with for loop
                auto r =
                        (float)this->vertex_colours[triangleVertices[0]][0] * bary[0] +
                        (float)this->vertex_colours[triangleVertices[1]][0] * bary[1] +
                        (float)this->vertex_colours[triangleVertices[2]][0] * bary[2];
                auto g =
                        (float)this->vertex_colours[triangleVertices[0]][1] * bary[0] +
                        (float)this->vertex_colours[triangleVertices[1]][1] * bary[1] +
                        (float)this->vertex_colours[triangleVertices[2]][1] * bary[2];
                auto b =
                        (float)this->vertex_colours[triangleVertices[0]][2] * bary[0] +
                        (float)this->vertex_colours[triangleVertices[1]][2] * bary[1] +
                        (float)this->vertex_colours[triangleVertices[2]][2] * bary[2];

                this->m_screenBuffer(x, y) = pack4(r, g, b, 1);
            }
        }
    }
}

void sr::SoftwareRasterizer::clearDepthBuffer() {
    m_depthBuffer.fill(0);
}

bool sr::SoftwareRasterizer::pointWithinTriangle(dim_t x, dim_t y, dim_t* A, dim_t* B, dim_t* C) {
    int P[3] = {x, y, 1};
    return CWCheck(P, B, C) && CWCheck(P, C, A) && CWCheck(P, A, B);
}

bool sr::SoftwareRasterizer::CWCheck(dim_t *A, dim_t *B, dim_t *C) {
    return triangularArea(A, B, C) < 0;
}

int sr::SoftwareRasterizer::triangularArea(dim_t *A, dim_t *B, dim_t *C) {
    return (B[0] - A[0])*(B[1] + A[1]) + (C[0] - B[0])*(B[1] + C[1]) + (A[0] - C[0])*(A[1] + C[1]);
}

void sr::SoftwareRasterizer::barycentric(dim_t *P, dim_t *A, dim_t *B, dim_t *C, float *result) {
    float tot = triangularArea(A, B, C);
    result[0] = float(triangularArea(P, B, C)) / tot;
    result[1] = float(triangularArea(P, C, A)) / tot;
    result[2] = float(triangularArea(P, A, B)) / tot;
}
