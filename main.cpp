#include <vector>
#include <cmath>
#include "model.h"
#include "geometry.h"
#include "tgaimage.h"

#include <iostream>

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

Model *model = NULL;

const int width  = 800;
const int height = 800;

void line(int x0, int y0, int x1, int y1, 
            TGAImage &image, TGAColor color) {
    
    bool steep = false;
    if (abs(x0-x1)<abs(y0-y1)) {
        steep = true;
    }
    
    if ((steep && y0 > y1) || (!steep && x0 > x1)) { // I understand why this works
        swap(x0, x1);
        swap(y0, y1);
    }
    
    if (!steep) {
        for (float x=x0; x<=x1; x++) {
            float t = (x-x0)/(float)(x1-x0); // This is the relative position of the current x
            int y = y0*(1.-t) + y1*t;
            image.set(x, y, color);
        }
    }
    else {
        for (float y=y0; y<=y1; y++) {
        float t = (y-y0)/(float)(y1-y0);
            int x = x0*(1.-t) + x1*t;
            image.set(x, y, color);
        }
    }
}

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            line(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
