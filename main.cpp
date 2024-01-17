#include <vector>
#include <cmath>
#include "model.h"
#include "geometry.h"
#include "tgaimage.h"

#include <iostream>

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
const TGAColor blue  = TGAColor(0,   0,   255, 255);
const TGAColor pink  = TGAColor(255, 0,   255, 255);

Model *model = NULL;

const int width  = 750;
const int height = 750;

void line(int xPoint1, int yPoint1, int xPoint2, int yPoint2, 
            TGAImage &image, TGAColor color) {
    // Had to move the steep code above the x1 > x2 code to fix a bug
    // Also, using lineSlope to figure out if a line is steep or not was unrealiable
    // The version done in the official code was more realiable (comparing absolute values)
    bool steep = false;
    if (abs(xPoint1 - xPoint2) < abs(yPoint1 - yPoint2)) {
        swap(xPoint1, yPoint1);
        swap(xPoint2, yPoint2);
        steep = true;
    }
    if (xPoint1 > xPoint2) {
        swap(xPoint1, xPoint2);
        swap(yPoint1, yPoint2);
    }
    float lineSlope = (float)(yPoint2 - yPoint1)/(float)(xPoint2 - xPoint1);
    float yIntercept = yPoint1 - (lineSlope * xPoint1);

    for (int xProgress = xPoint1; xProgress <= xPoint2; xProgress++) {
        int yProgress = (float)(xProgress * lineSlope) + yIntercept;
        if (!steep) {
            image.set(xProgress, yProgress, color);
        }
        else {
            image.set(yProgress, xProgress, color);
        }
    }
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, 
                TGAImage &image, TGAColor color) {
    /*
        Requirements:
        - It should be (surprise!) simple and fast.
        - It should be symmetrical: the picture should not depend on the order of vertices passed to the drawing function.
        - If two triangles have two common vertices, there should be no holes between them because of rasterization rounding.

        Should do:
        1. Sort vertices of the triangle by their y-coordinates;
        2. Rasterize simultaneously the left and the right sides of the triangle;
        3. Draw a horizontal line segment between the left and the right boundary points.
    */

    // sort y-coordinates
    if (t0.y<t1.y) {
        swap(t0, t1);
    }
    if (t0.y<t2.y) {
        swap(t0, t2);
    }
    if (t1.y<t2.y) {
        swap(t1, t2);
    }

    float t0dt1 = sqrt(pow(t1.x-t0.x, 2)+pow(t1.y-t0.y, 2));
    float t1dt2 = sqrt(pow(t2.x-t1.x, 2)+pow(t2.y-t1.y, 2));
    float t0dt2 = sqrt(pow(t2.x-t0.x, 2)+pow(t2.y-t0.y, 2));

    // AHHHHHHHHHHHHHHHHHHHHHHHHHH
    float t0a   = acos((pow(t0dt1, 2.)+pow(t0dt2, 2.)-pow(t1dt2, 2.))/(2*t0dt1*t0dt2))*(180.0/3.141592653589793238463);
    float t1a   = acos((pow(t0dt1, 2.)+pow(t1dt2, 2.)-pow(t0dt2, 2.))/(2*t0dt1*t1dt2))*(180.0/3.141592653589793238463);
    float t2a   = acos((pow(t1dt2, 2.)+pow(t0dt2, 2.)-pow(t0dt1, 2.))/(2*t1dt2*t0dt2))*(180.0/3.141592653589793238463);

    cout << t0a << " " << t1a << " " << t2a << endl;
    cout << t0dt1 << " " << t1dt2 << " " << t0dt2 << endl;

    if (t0a>t1a) {
        swap(t0, t1);
    }
    if (t0a>t2a) {
        swap(t0, t2);
    }
    if (t1a>t2a) {
        swap(t1, t2);
    }

    bool steep = false;
    if (abs(t1.x-t2.x)<abs(t1.y-t2.y)) {
        swap(t1.x, t1.y);
        swap(t2.x, t2.y);
        steep = true;
    }
    if (t1.x>t2.x) {
        swap(t1.x, t2.x);
        swap(t1.y, t2.y);
    }
    int dx = t2.x-t1.x;
    int dy = t2.y-t1.y;
    int derror2 = abs(dy)*2;
    int error2 = 0;
    int y = t1.y;
    
    for (int x=t1.x; x<=t2.x*0.6; x++) {
        if (steep) {
            line(y, x, t0.x, t0.y, image, white);
            image.set(y, x, color);
        } else {
            line(x, y, t0.x, t0.y, image, white);
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (t2.y>t1.y?1:-1);
            error2 -= dx*2;
        }
    }
    
    /*
    dx = t2.x-t1.x;
    dy = t2.y-t1.y;
    derror2 = abs(dy)*2;
    error2 = 0;
    y = t2.y;
    for (int x=t2.x; x>=t1.x*0.6; x--) {
        if (steep) {
            line(y, x, t0.x, t0.y, image, white);
            image.set(y, x, color);
        } else {
            line(x, y, t0.x, t0.y, image, white);
            image.set(x, y, color);
        }
        error2 -= derror2;
        if (error2 < dx) {
            y -= (t2.y>t1.y?1:-1);
            error2 += dx*2;
        }
    }
    */

    // draw left and right sides of triangle
    line(t1.x, t1.y, t0.x, t0.y, image, blue);
    line(t2.x, t2.y, t0.x, t0.y, image, blue);

}

void mytriangle(Vec2i vector1, Vec2i vector2, Vec2i vector3,
                TGAImage &image, TGAColor color) {
    line(vector1.x, vector1.y, vector2.x, vector2.y, image, red);
    line(vector2.x, vector2.y, vector3.x, vector3.y, image, red);
    line(vector1.x, vector1.y, vector3.x, vector3.y, image, red);
}

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    // I just copied this code
    // I can rewrite this later myself
    // Also, the if (bool) parts of code is to make debugging easier
    if (false) {
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
    }

    if (true) {
        Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
        Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
        Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

        mytriangle(t0[0], t0[1], t0[2], image, blue);
        mytriangle(t1[0], t1[1], t1[2], image, white);
        mytriangle(t2[0], t2[1], t2[2], image, green);
    }

    if (false) {
        line(5, 5, 40, 40, image, white);
        line(5, 5, 40, 20, image, green);
        line(0, 10, 20, 20, image, blue);
        line(40, 40, 5, 5, image, white);
        line(40, 20, 5, 5, image, green);
        line(5, 10, 10, 20, image, pink);
        line(20, 10, 10, 20, image, pink);
        line(20, 10, 30, 40, image, blue);

        line(50, 700, 700, 50, image, white);
        line(50, 50, 700, 700, image, white);

        line(100, 50, 100, 700, image, white);
        line(200, 50, 210, 700, image, white);
        line(300, 50, 290, 700, image, white);
        line(400, 700, 400, 50, image, white);
        line(50, 325, 700, 325, image, white);
        line(700, 425, 50, 425, image, white);
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
