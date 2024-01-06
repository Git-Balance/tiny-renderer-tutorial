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


    if (xPoint1 > xPoint2) {
        swap(xPoint1, xPoint2);
        swap(yPoint1, yPoint2);
    }
    float lineSlope = (float)(yPoint2 - yPoint1)/(float)(xPoint2 - xPoint1);
    cout << lineSlope << endl;
    bool steep = false;
    if (lineSlope > 1.0) {
        swap(xPoint1, yPoint1);
        swap(xPoint2, yPoint2);
        lineSlope = (float)(yPoint2 - yPoint1)/(float)(xPoint2 - xPoint1);
        steep = true;
    }
    // This is the b in y=mx+b
    float yIntercept = yPoint1 - (lineSlope * xPoint1);
    cout << yIntercept << endl;

    for (int xProgress = xPoint1; xProgress <= xPoint2; xProgress++) {
        // TODO: This is where the main problems are currently, progressUp
        // I can't figure out how to get a percentage of height

        /*
         * Current Main problems
         * - [X] Figure out how to make the line go up properly
         *       TLDR, made equation y=mx+b in yProgress
         * - [X] Figure out how to get the b part of y=mx+b
         *       Adding yPoint1 doesn't work, it adds to much
         *       The yProgress part of the code DOES work
        */

        // REMEMBER: yPoint1 should always be less than yPoint2
        int yProgress = (float)(xProgress * lineSlope) + yIntercept;
        cout << yProgress << endl;;

        if (!steep) {
            image.set(xProgress, yProgress, color);
        }
        else {
            image.set(yProgress, xProgress, color);
        }
    }
    /*
    if (!steep) {
        image.set(xPoint1, yPoint1, red);
        image.set(xPoint2, yPoint2, red);
    }
    else {
        image.set(yPoint1, xPoint1, red);
        image.set(yPoint2, xPoint2, red);
    }
    */
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
    // I just copied this code
    // I can rewrite this later myself
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
    cout << "Start green" << endl;
    myline(5, 5, 40, 20, image, green);
    cout << "End green" << endl;
    myline(0, 10, 20, 20, image, blue);
    myline(40, 40, 5, 5, image, white);
    myline(40, 20, 5, 5, image, green);
    myline(5, 10, 10, 20, image, pink);
    myline(20, 10, 10, 20, image, pink);
    myline(20, 10, 30, 40, image, blue);

    myline(50, 700, 700, 50, image, white);
    myline(50, 50, 700, 700, image, white);

    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
