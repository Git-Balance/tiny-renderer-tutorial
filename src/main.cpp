#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include "tgaimage.h"
#include "model.h"

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
    /*
     * This project involves constantly changing the size of the canvas
     * It is easier to change one variable with a cheatsheet rather than memorize different width/height values
     * 7 = 128
     * 10 = 1024
     */
    constexpr int scale = 9;
    const int width  = std::pow(2, scale);
    const int height = std::pow(2, scale);
    TGAImage framebuffer(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    Model model("./obj/diablo3_pose/diablo3_pose.obj");

    model.draw(width, height, framebuffer, zbuffer);

    framebuffer.write_tga_file("framebuffer.tga");
    zbuffer.write_tga_file("zbuffer.tga");
    return 0;
}
