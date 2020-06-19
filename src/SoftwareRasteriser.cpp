//
// Created by dardledumdue on 19/06/2020.
//

#include "SoftwareRasteriser.hpp"

#include <algorithm>
#include <iostream>


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

void sr::SoftwareRasteriser::render() {
    clearDepthBuffer();
    colour( 0, 0, 0);
    for (int i = 0; i < this->num_faces; i++) {
        renderTriangle(i);
    }
}

void sr::SoftwareRasteriser::load(int **vertices, int num_faces, int **faces, uint8_t **vertex_colours) {
    this->vertices = vertices;
    this->faces = faces;
    this->num_faces = num_faces;
    this->vertex_colours = vertex_colours;
}

void sr::SoftwareRasteriser::renderTriangle(int index) {
    int* triangleVertices = this->faces[index];
    int x_min = this->width;
    int y_min = this->height;
    int x_max = 0;
    int y_max = 0;
    for (int i = 0; i < 3; i++) {
        x_min = std::min(x_min, this->vertices[triangleVertices[i]][0]);
        y_min = std::min(y_min, this->vertices[triangleVertices[i]][1]);
        x_max = std::max(x_max, this->vertices[triangleVertices[i]][0]);
        y_max = std::max(y_max, this->vertices[triangleVertices[i]][1]);
    }
    x_min = std::max(0, x_min);
    y_min = std::max(0, y_min);
    x_max = std::min(this->width, x_max);
    y_max = std::min(this->height, y_max);
    bool state = false;
    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            if ((x+y)%16 == 0) state = !state;
            if (pointWithinTriangle(x, y, this->vertices[triangleVertices[0]], this->vertices[triangleVertices[1]], this->vertices[triangleVertices[2]]))
                if (state) {
                    this->m_screenBuffer[getPixelLocation(x, y)] = 0xff;
                    this->m_screenBuffer[getPixelLocation(x, y) + 1] = 0xff;
                    this->m_screenBuffer[getPixelLocation(x, y) + 2] = 0xff;
                }
        }
    }
}

void sr::SoftwareRasteriser::clearDepthBuffer() {
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            this->m_depthBuffer[y * this->width + x] = INT32_MIN;
        }
    }
}

bool sr::SoftwareRasteriser::pointWithinTriangle(int x, int y, int* A, int* B, int* C) {
    int P[3] = {x, y, 1};
    return CWCheck(P, B, C) && CWCheck(P, C, A) && CWCheck(P, A, B);
}

bool sr::SoftwareRasteriser::CWCheck(int *A, int *B, int *C) {
    return (B[0] - A[0])*(B[1] + A[1]) + (C[0] - B[0])*(B[1] + C[1]) + (A[0] - C[0])*(A[1] + C[1]) < 0;
}
