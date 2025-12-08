#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <vector>
#include "tgaimage.h"

#define FACE_VERTEX_NUM 3

/*
 * General TODO:
 *          Move drawing funcitons to draw.cpp
 *          Move basic data types to their own header (Token, Vertex, etc.)
 *          Rework how the structs Vertex and Face work
 *              TGAColor stores its color data in this variable: std::uint8_t bgra[4] = {0,0,0,0};
 *              I was apprehensive about using arrays to store the variables because using regular floats/ints seemed easier, but TGAColor works well
 */

using namespace std;

const TGAColor white = {255, 255, 255, 255}; // BGRA order
const TGAColor blue  = {255, 0,   0,   255};
const TGAColor green = {0,   255, 0,   255};
const TGAColor red   = {0,   0,   255, 255};
const TGAColor pink  = {255, 0,   255, 255};

enum ModelLineType {
    EMPTY = -1,
    VERTEX,
    FACE,
    OTHER = 99
};

using Token = std::string;
using Tokens = std::vector<Token>;

struct Vertex {
    float x;
    float y;
    float z;
};

struct Face {
    /*
        f 6/1/6 5/2/5 8/3/8
        f 6/1/6 8/3/8 7/4/7
        f 12/5/12 11/6/11 10/7/10
     */

    // NOTE: While obj files start their indexes at 1 (at least for faces), the Face struct's indexes will start at 0

    // Refrence: f v1/__/__ v2/__/__ v3/__/__

    int v1;
    int v2;
    int v3;
    // Future variables will be added later
};

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax>bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for (int x=ax; x<=bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        ierror += 2 * std::abs(by-ay);
        y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
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


class Model {
private:
    // TODO: Remove the count variables once vertexes and faces are properly implemented
    size_t vertexCount = 0; // For now, vertexNum = 3 means that there are 3 vertexes, not 4
    size_t faceCount = 0;
    std::vector<Vertex> vertexes; // QUESTION: Should vertexes and faces be converted into arrays once their data is initially red? At that point, we know what the size of the vector is
    std::vector<Face> faces;
public:
    size_t getVertexCount() { return vertexCount; }
    // void setVertexCount(size_t vn) { vertexCount = vn; } // Why would I need to change the vertex count?
    size_t getFaceCount() { return faceCount; }
    std::vector<Vertex> getVertexes() { return vertexes; }
    std::vector<Face> getFaces() { return faces; }

    ModelLineType getLineType(Tokens tokens) {
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

    void addVertex(Tokens tokens) {
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

    void addFace(Tokens tokens) {
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

    void printVertexesAndFacesCount() {
        // TODO: Add proper face support once they are added
        std::cout << vertexes.size() << " " << faces.size() << std::endl;
    }
    void printVertex(int index) {
        Vertex vertex = vertexes.at(index);
        std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }
    void printFace(int index) {
        Face face = faces.at(index);
        std::cout << face.v1 << " " << face.v2 << " " << face.v3 << std::endl;
    }

    Model(std::string filename) {
        std::ifstream in; // Inspired by TGAImage's file handling
        in.open(filename, std::ios::binary);
        if (!in.is_open()) {
            std::string errorMessage = "Error opening ";
            errorMessage.append(filename);
            throw runtime_error(errorMessage);
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
    void drawWireframe(int width, int height, TGAImage &framebuffer) {
        // TODO: Remove width and height parameters, replace with TGAImage.width()/height()

        TGAColor color = blue; // A temporary color value to complete the wireframe rendering assignment

        int scale = max(width, height); // NOTE: I am not in love with this name or the way scaling works; feel free to rework this variable
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
};

int main(int argc, char** argv) {
    constexpr int width  = 1024;
    constexpr int height = 1024;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    Model model("./obj/diablo3_pose/diablo3_pose.obj");

    model.drawWireframe(width, height, framebuffer);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
