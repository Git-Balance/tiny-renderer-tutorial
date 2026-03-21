#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include "tgaimage.h"
#include "draw.h"
#include "colors.h"

#define FACE_VERTEX_NUM 3

enum ModelLineType {
    EMPTY = -1,
    VERTEX,
    FACE,
    OTHER = 99
};

struct Vertex {
    union {
        float xyz[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
    float& operator[](const int i) { return xyz[i]; }
};

struct Face {
    /*
        f 6/1/6 5/2/5 8/3/8
        f 6/1/6 8/3/8 7/4/7
        f 12/5/12 11/6/11 10/7/10
     */

    // NOTE: While obj files start their indexes at 1 (at least for faces), the Face struct's indexes will start at 0

    // Refrence: f v1/__/__ v2/__/__ v3/__/__

    union {
        // Vertexes
        int v[3];
        struct {
            int v1;
            int v2;
            int v3;
        };
    };
    // Future variables will be added later
};

using Token = std::string;
using Tokens = std::vector<Token>;

Tokens split(std::string s, std::string delimiter);

class Model {
private:
    std::vector<Vertex> vertexes; // QUESTION: Should vertexes and faces be converted into arrays once their data is initially red? At that point, we know what the size of the vector is
    std::vector<Face> faces;
public:
    size_t getVertexCount();
    size_t getFaceCount();
    std::vector<Vertex> getVertexes();
    std::vector<Face> getFaces();

    ModelLineType getLineType(Tokens tokens);

    void addVertex(Tokens tokens);
    void addFace(Tokens tokens);
    void printVertexesAndFacesCount();
    void printVertex(int index);
    void printFace(int index);

    Model(std::string filename);
    void drawWireframe(int width, int height, TGAImage &framebuffer);
    void drawTriangles(int width, int height, TGAImage &framebuffer);
};
