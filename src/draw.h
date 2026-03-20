#pragma once

#include "tgaimage.h"
#include <cstdlib>
#include <utility>


void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color);

float slope(int ax, int ay, int bx, int by);

double signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy);

void triangle(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color);

void triangleBarycentric(int ax, int ay, int bx, int by, int cx, int cy, TGAImage &framebuffer, TGAColor color);



