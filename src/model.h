#pragma once

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include "tgaimage.h"
#include "draw.h"
#include "colors.h"
#include "geometry.h"

#define FACE_VERTEX_NUM 3

enum ModelLineType {
    EMPTY = -1,
    VERTEX,
    FACE,
    OTHER = 99
};

// FrameAdjuster?
class VertexAdjuster {
private:
    // NOTE: I am not in love with this name or the way scaling works; feel free to rework this variable
     // Should this use min() instead?
    int width, height;
    int scale;
    float pointAdjust;
    float pointMultiply;
public:
    void setScale(int width, int height);
    VertexAdjuster(int width, int height);
    Vec3i adjust(Vertex &v);
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
    void draw(int width, int height, TGAImage &framebuffer, TGAImage &zbuffer);
};
