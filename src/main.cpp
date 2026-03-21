#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include "tgaimage.h"
#include "model.h"
#include "draw.h"


/*
 * General TODO:
 *          Move drawing funcitons to draw.cpp
 *          Move basic data types to their own header (Token, Vertex, etc.)
 *          Rework how the structs Vertex and Face work
 *              TGAColor stores its color data in this variable: std::uint8_t bgra[4] = {0,0,0,0};
 *              I was apprehensive about using arrays to store the variables because using regular floats/ints seemed easier, but TGAColor works well
 */

using namespace std;

int main(int argc, char** argv) {
    // constexpr int width  = 1024;
    // constexpr int height = 1024;
    /*
     * 7 = 128
     * 10 = 1024
     */
    constexpr int scale = 10;
    const int width  = std::pow(2, scale);
    const int height = std::pow(2, scale);
    TGAImage framebuffer(width, height, TGAImage::RGB);

    Model model("./obj/diablo3_pose/diablo3_pose.obj");

    // model.drawWireframe(width, height, framebuffer);
    model.drawTriangles(width, height, framebuffer);

    // triangleBarycentric(  7, 45, 35, 100, 45,  60, framebuffer, red);
    // triangleBarycentric(120, 35, 90,   5, 45, 110, framebuffer, white);
    // triangleBarycentric(115, 83, 80,  90, 85, 120, framebuffer, green);

    // triangle(532.451, 853.951, 532.631, 847.68, 538.857, 847.014, framebuffer, pink);

    // triangle(10, 10, 10, 20, 20, 20, framebuffer, pink);
    // triangle(10, 10, 10, 20, 20, 10, framebuffer, pink);
    // triangle(30, 30, 40, 30, 40, 20, framebuffer, pink);

    // framebuffer.flip_vertically();
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
