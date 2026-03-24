#include "model.h"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include "tgaimage.h"
#include "draw.h"
#include "colors.h"

using Token = std::string;
using Tokens = std::vector<Token>;

void VertexAdjuster::setScale(int width, int height) {
    scale = std::max(width, height);
}

VertexAdjuster::VertexAdjuster(int width, int height, float pointAdjust) {
    setScale(width, height);
    pointAdjust = pointAdjust;
    pointMultiply = scale / 2.0f;
}

void VertexAdjuster::adjust(Vertex &vertex) {
    for (int dimension = 0; dimension < 3; dimension++) {
        vertex[dimension] += pointAdjust;
        vertex[dimension] *= pointMultiply;
    }
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

size_t Model::getVertexCount() { return sizeof(vertexes); }
// void setVertexCount(size_t vn) { vertexCount = vn; } // Why would I need to change the vertex count?
size_t Model::getFaceCount() { return sizeof(faces); }
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
void Model::drawWireframe(int width, int height, TGAImage &framebuffer) {
    // TODO: Remove width and height parameters, replace with TGAImage.width()/height()

    TGAColor color = blue; // A temporary color value to complete the wireframe rendering assignment

    VertexAdjuster adjuster{width, height, 1.0f};
    for (Face face : faces) {
        Vertex currentVertexes[3];
        for (int vertex = 0; vertex < 3; vertex++) {
            currentVertexes[vertex] = vertexes.at(face.v[vertex]);
        }

        for (int index = 0; index < FACE_VERTEX_NUM; index++) {
            adjuster.adjust(currentVertexes[index]);
        }

        for (int vertexIndex = 0; vertexIndex < FACE_VERTEX_NUM; vertexIndex++) {
            Vertex vertexA = currentVertexes[vertexIndex];
            Vertex vertexB = currentVertexes[(vertexIndex + 1) % FACE_VERTEX_NUM];
            line(vertexA.x, vertexA.y, vertexB.x, vertexB.y, framebuffer, color);
        }
    }
}

void Model::drawTriangles(int width, int height, TGAImage &framebuffer) {
    // TODO: Remove width and height parameters, replace with TGAImage.width()/height()

    int scale = std::max(width, height); // NOTE: I am not in love with this name or the way scaling works; feel free to rework this variable
                                    // Should this use min() instead?
    for (Face face : faces) {
        Vertex currentVertexes[3];
        for (int vertex = 0; vertex < 3; vertex++) {
            currentVertexes[vertex] = vertexes.at(face.v[vertex]);
        }

        float vertexAdjust = 1;
        float vertexMultiply = scale / 2.0f;
        for (int index = 0; index < FACE_VERTEX_NUM; index++) {
            for (int dimension = 0; dimension < 3; dimension++) {
                currentVertexes[index][dimension] += vertexAdjust;
                currentVertexes[index][dimension] *= vertexMultiply;
            }
        }

        // The random color code was taken from the tutorial
        TGAColor color;
        for (int colorIndex = 0; colorIndex < 3; colorIndex++) {
            color[colorIndex] = std::rand() % 255;
        }
        triangle(currentVertexes[0].x, currentVertexes[0].y, currentVertexes[1].x, currentVertexes[1].y, currentVertexes[2].x, currentVertexes[2].y, framebuffer, color);
    }
}

