#include "SoftwareRasteriser.hpp"

#include <algorithm>
#include <iostream>


void sr::SoftwareRasteriser::colour(uint8_t r, uint8_t g, uint8_t b) {
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            m_screenBuffer[getPixelLocation(x, y)].r = r;
            m_screenBuffer[getPixelLocation(x, y)].g = g;
            m_screenBuffer[getPixelLocation(x, y)].b = b;
        }
    }
}

uint8_t *sr::SoftwareRasteriser::getBuffer() {
    return (uint8_t*)m_screenBuffer;
}

inline int sr::SoftwareRasteriser::getPixelLocation(int x, int y) {
    return y * m_width + x;
}

void sr::SoftwareRasteriser::render() {
    clearDepthBuffer();
    colour( 0, 0, 0);
    for (int i = 0; i < this->m_numFaces; i++) {
        renderTriangle(i);
    }
    if (m_fragmentShader)
    {
	    for (int y = 0; y < m_height; y++)
	    {
		    for (int x = 0; x < m_width; x++)
		    {
                m_fragmentShader({ x, y, m_depthBuffer[getPixelLocation(x, y)] }, m_frameCount, m_screenBuffer[getPixelLocation(x, y)]);
		    }
	    }
    }
    m_frameCount++;
}

void sr::SoftwareRasteriser::load(Vec3 *vertices, int numFaces, Vec3 *faces, Colour *vertexColours) {
    m_vertices = vertices;
    m_faces = faces;
    m_numFaces = numFaces;
    m_vertexColours = vertexColours;
}

void sr::SoftwareRasteriser::loadFragmentShader(FragmentShaderFunc fragmentShader)
{
    this->m_fragmentShader = fragmentShader;
}

void sr::SoftwareRasteriser::renderTriangle(int index) {
    Vec3 triangleVertices = m_faces[index];

    // Avoid inverted rendering
    if (triangularArea(m_vertices[triangleVertices.x], m_vertices[triangleVertices.y],
            m_vertices[triangleVertices.z]) > 0) {
        return;
    }

    int x_min = this->m_width;
    int y_min = this->m_height;
    int x_max = 0;
    int y_max = 0;
    for (int i = 0; i < 3; i++) {
        x_min = std::min(x_min, m_vertices[triangleVertices[i]].x);
        y_min = std::min(y_min, m_vertices[triangleVertices[i]].y);
        x_max = std::max(x_max, m_vertices[triangleVertices[i]].x);
        y_max = std::max(y_max, m_vertices[triangleVertices[i]].y);
    }
    x_min = std::max(0, x_min);
    y_min = std::max(0, y_min);
    x_max = std::min(m_width, x_max);
    y_max = std::min(m_height, y_max);


    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            Vec3f bary = {};
            Vec3 P = {x, y, 1};
            barycentric(P, m_vertices[triangleVertices.x], m_vertices[triangleVertices.y],
                    m_vertices[triangleVertices.z], bary);

            if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0) {
                m_screenBuffer[getPixelLocation(x, y)].r =
                        m_vertexColours[triangleVertices[0]][0] * bary.x +
                        m_vertexColours[triangleVertices[1]][0] * bary.y +
                        m_vertexColours[triangleVertices[2]][0] * bary.z;
                m_screenBuffer[getPixelLocation(x, y)].g =
                        m_vertexColours[triangleVertices[0]][1] * bary[0] +
                        m_vertexColours[triangleVertices[1]][1] * bary[1] +
                        m_vertexColours[triangleVertices[2]][1] * bary[2];
                m_screenBuffer[getPixelLocation(x, y)].b =
                        m_vertexColours[triangleVertices[0]][2] * bary[0] +
                        m_vertexColours[triangleVertices[1]][2] * bary[1] +
                        m_vertexColours[triangleVertices[2]][2] * bary[2];
            }

        }
    }
}

void sr::SoftwareRasteriser::clearDepthBuffer() {
    for (int y = 0; y < this->m_height; y++) {
        for (int x = 0; x < this->m_width; x++) {
           m_depthBuffer[getPixelLocation(x, y)] = INT32_MIN;
        }
    }
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
