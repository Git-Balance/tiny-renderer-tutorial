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

Model *model = NULL;

const int width  = 50;
const int height = 50;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
}

void myline(int xPoint1, int yPoint1, int xPoint2, int yPoint2, 
            TGAImage &image, TGAColor color) {
    /*
     * My line, with code I made myself
     */
    
    /*
     * for (xProgress = xPoint1) (xProgress <= xPoint2) (xProgress++)
     *   // yProgress will have to be relative to xProgress
     *   // Maybe make yProgress a percentage of yPoint2
     *   // No, make yProgress = yPoint2 - a percentage of yPoint1
     *   // Could adding work?
     *   // Either way, we need some way of getting a percentage of yPoint1
     *   progressUp = yPoint1 / yPoint2
     *   yProgress = yPoint1 + ((yPoint2 - yPoint1) * progressUp)
     */

    for (int xProgress = xPoint1; xProgress <= xPoint2; xProgress++) {
        // TODO: This is where the main problems are currently, progressUp
        // I can't figure out how to get a percentage of height
        float progressUp = (float) yPoint2 - yPoint1;
        cout << progressUp << endl;;
        // REMEMBER: yPoint1 should always be less than yPoint2
        int yProgress = yPoint1 + (float)((yPoint2 - yPoint1) * progressUp);

        image.set(xProgress, yProgress, color);
    }

    image.set(xPoint1, yPoint1, red);
    image.set(xPoint2, yPoint2, red);
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

int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    /*
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
    */

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    /*
    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);
    */

    myline(5, 5, 40, 40, image, white);
    myline(5, 5, 40, 20, image, green);

    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
