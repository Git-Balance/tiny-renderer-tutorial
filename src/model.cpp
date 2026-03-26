#include "model.h"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include "tgaimage.h"
#include "draw.h"

using Token = std::string;
using Tokens = std::vector<Token>;

void VertexAdjuster::setScale(int width, int height) {
    scale = std::max(width, height);
}

VertexAdjuster::VertexAdjuster(int width, int height) {
    this->width = width;
    this->height = height;
    setScale(width, height);
    pointAdjust = 1.0f;
    pointMultiply = scale / 2.0f;
}

Vec3i VertexAdjuster::adjust(Vertex &v) {
    return Vec3i{
            static_cast<int>((v.x + pointAdjust) * width/2),
            static_cast<int>((v.y + pointAdjust) * height/2),
            static_cast<int>((v.z + pointAdjust) * 255./2),
    };
}

Tokens split(std::string s, std::string delimiter) {
    // Inspiration: https://stackoverflow.com/a/46931770
    Tokens tokens;
    size_t posStart = 0;
    size_t posEnd;
    size_t delimiterLen = delimiter.length();
    while ((posEnd = s.find(delimiter, posStart)) != std::string::npos) {
        tokens.push_back(s.substr(posStart, posEnd - posStart));
        posStart = posEnd + delimiterLen;
    }
    tokens.push_back(s.substr(posStart, s.size() - posStart));
    return tokens;
}

size_t Model::getVertexCount() { return vertexes.size(); }
// void setVertexCount(size_t vn) { vertexCount = vn; } // Why would I need to change the vertex count?
size_t Model::getFaceCount() { return faces.size(); }
std::vector<Vertex> Model::getVertexes() { return vertexes; }
std::vector<Face> Model::getFaces() { return faces; }

ModelLineType Model::getLineType(Tokens tokens) {
    if (tokens.size() == 0) { return EMPTY; }
    size_t posStart = 0;
    Token token = tokens.at(posStart);
    if (token == "v") {
        return VERTEX;
    }
    else if (token == "f") {
        return FACE;
    }
    return OTHER;
}

void Model::addVertex(Tokens tokens) {
    /* Wikipedia https://en.wikipedia.org/wiki/Wavefront_.obj_file
    # List of geometric vertices, with (x, y, z, [w]) coordinates, w is optional and defaults to 1.0.
    v 0.123 0.234 0.345 1.0
     */
    // v 0.11526 0.700717 0.0677257
    Vertex vertex{};
    for (int tokenChar = 1; tokenChar < 4; tokenChar++) {
        vertex.xyz[tokenChar - 1] = std::stof(tokens.at(tokenChar));
    }
    vertexes.push_back(vertex);
}

void Model::addFace(Tokens tokens) {
    Face face{};
    int indexMax = 3;
    for (int index = 0; index < indexMax; index++) {
        face.v[index] = std::stoi(split(tokens.at(index + 1), "/").at(0)) - 1;
    }
    faces.push_back(face);
}

void Model::printVertexesAndFacesCount() {
    // TODO: Add proper face support once they are added
    std::cout << vertexes.size() << " " << faces.size() << std::endl;
}
void Model::printVertex(int index) {
    Vertex vertex = vertexes.at(index);
    std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
}
void Model::printFace(int index) {
    Face face = faces.at(index);
    std::cout << face.v1 << " " << face.v2 << " " << face.v3 << std::endl;
}

Model::Model(std::string filename) {
    std::ifstream in; // Inspired by TGAImage's file handling
    in.open(filename, std::ios::binary);
    if (!in.is_open()) {
        std::string errorMessage = "Error opening ";
        errorMessage.append(filename);
        throw std::runtime_error(errorMessage);
    }
    std::string line;
    Tokens tokens;

    while (!in.eof()) {
        getline(in, line);
        tokens = split(line, " ");

        // TODO: Add function that verifies line is formatted correctly

        switch (getLineType(tokens)) {
            case VERTEX:
                addVertex(tokens);
                break;
            case FACE:
                addFace(tokens);
                break;
            case OTHER:
            case EMPTY:
                break;
        }
    }
}

// TODO: drawWireframe and drawTriangles share code; split repeated code into functions
// Maybe make a `vertexTrio` struct and a `vertexTrio faceToTrio(Face face)` function?
// Or maybe just rework how the `Face` struct works
void Model::draw(int width, int height, TGAImage &framebuffer, TGAImage &zbuffer) {
    // TODO: Remove width and height parameters, replace with TGAImage.width()/height()

    for (Face face : faces) {
        VertexAdjuster projector{width, height};
        Vec3i a = projector.adjust(vertexes.at(face.v[0]));
        Vec3i b = projector.adjust(vertexes.at(face.v[1]));
        Vec3i c = projector.adjust(vertexes.at(face.v[2]));

        // The random color code was taken from the tutorial
        TGAColor rColor;
        for (int colorIndex = 0; colorIndex < 3; colorIndex++) {
            rColor[colorIndex] = std::rand() % 255;
        }
        triangle(a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z, framebuffer, zbuffer, rColor);
    }
}

