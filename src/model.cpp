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

size_t Model::getVertexCount() { return vertexCount; }
// void setVertexCount(size_t vn) { vertexCount = vn; } // Why would I need to change the vertex count?
size_t Model::getFaceCount() { return faceCount; }
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
    vertex.x = std::stof(tokens.at(1));
    vertex.y = std::stof(tokens.at(2));
    vertex.z = std::stof(tokens.at(3));
    vertexes.push_back(vertex);
}

void Model::addFace(Tokens tokens) {
    Face face{};
    int indexMax = 3;
    int v[indexMax];
    for (int index = 0; index < indexMax; index++) {
        v[index] = std::stoi(split(tokens.at(index + 1), "/").at(0)) - 1;
    }
    face.v1 = v[0];
    face.v2 = v[1];
    face.v3 = v[2];
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

        switch (getLineType(tokens)) {
            case VERTEX:
                vertexCount++;
                addVertex(tokens);
                break;
            case FACE:
                faceCount++;
                addFace(tokens);
                break;
            case OTHER:
            case EMPTY:
                break;
        }
    }
}
void Model::drawWireframe(int width, int height, TGAImage &framebuffer) {
    // TODO: Remove width and height parameters, replace with TGAImage.width()/height()

    TGAColor color = blue; // A temporary color value to complete the wireframe rendering assignment

    int scale = std::max(width, height); // NOTE: I am not in love with this name or the way scaling works; feel free to rework this variable
                                         // Should this use min() instead?
    for (Face face : faces) {
        Vertex currentVertexes[3];
        currentVertexes[0] = vertexes.at(face.v1);
        currentVertexes[1] = vertexes.at(face.v2);
        currentVertexes[2] = vertexes.at(face.v3);

        float vertexAdjust = 1;
        float vertexMultiply = scale / 2.0f;
        for (int index = 0; index < FACE_VERTEX_NUM; index++) {
            currentVertexes[index].x += vertexAdjust;
            currentVertexes[index].y += vertexAdjust;
            currentVertexes[index].z += vertexAdjust;
            currentVertexes[index].x *= vertexMultiply;
            currentVertexes[index].y *= vertexMultiply;
            currentVertexes[index].z *= vertexMultiply;
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
        currentVertexes[0] = vertexes.at(face.v1);
        currentVertexes[1] = vertexes.at(face.v2);
        currentVertexes[2] = vertexes.at(face.v3);

        float vertexAdjust = 1;
        float vertexMultiply = scale / 2.0f;
        for (int index = 0; index < FACE_VERTEX_NUM; index++) {
            currentVertexes[index].x += vertexAdjust;
            currentVertexes[index].y += vertexAdjust;
            currentVertexes[index].z += vertexAdjust;
            currentVertexes[index].x *= vertexMultiply;
            currentVertexes[index].y *= vertexMultiply;
            currentVertexes[index].z *= vertexMultiply;
        }

        // The random color code was taken from the tutorial
        TGAColor color;
        for (int colorIndex = 0; colorIndex < 3; colorIndex++) {
            color[colorIndex] = std::rand() % 255;
        }
        triangle(currentVertexes[0].x, currentVertexes[0].y, currentVertexes[1].x, currentVertexes[1].y, currentVertexes[2].x, currentVertexes[2].y, framebuffer, color);
    }
}

