#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include <string>
#include <fstream>
#include <vector>
#include "tgaimage.h"

using namespace std;

const TGAColor white = {255, 255, 255, 255}; // BGRA order
const TGAColor red   = {255, 0,   0,   255};
const TGAColor green = {0,   255, 0,   255};
const TGAColor blue  = {0,   0,   255, 255};
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
    std::vector<Vertex> vertexes;
public:
    size_t getVertexCount() { return vertexCount; }
    // void setVertexCount(size_t vn) { vertexCount = vn; } // Why would I need to change the vertex count?
    size_t getFaceCount() { return faceCount; }
    std::vector<Vertex> getVertexes() { return vertexes; }

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

    void printVertexesAndFacesCount() {
        // TODO: Add proper face support once they are added
        std::cout << vertexes.size() << " " << faceCount << std::endl;
    }
    void printVertex(int index) {
        Vertex vertex = vertexes.at(index);
        std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    Model(std::string path) {
        std::ifstream fileStream(path);
        std::string line;
        Tokens tokens;

        while (!fileStream.eof()) {
            getline(fileStream, line);
            tokens = split(line, " ");

            /*
             * Step 1: Count lines that start with `v` or `f`
             */

            switch (getLineType(tokens)) {
                case VERTEX:
                    vertexCount++;
                    addVertex(tokens);
                    break;
                case FACE:
                    faceCount++;
                    break;
                case OTHER:
                case EMPTY:
                    break;
            }
        }
    }
};

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    /*
    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, white);
    framebuffer.set(cx, cy, white);
    */

    /*
    std::srand(std::time({}));
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width, ay = rand()%height;
        int bx = rand()%width, by = rand()%height;
        line(ax, ay, bx, by, framebuffer, { static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255), static_cast<uint8_t>(rand()%255) });
    }
    */
    Model model("./obj/diablo3_pose/diablo3_pose.obj");

    std::cout << model.getVertexCount() << " " << model.getFaceCount() << std::endl;
    std::cout << model.getVertexes().size() << std::endl;
    //std::cout << model.getVertexes().at(0).x << std::endl;
    model.printVertexesAndFacesCount();
    model.printVertex(0);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
