# Tiny Renderer Tutorial

## Why I'm doing this

I found a cool project on GitHub called [Tiny Renderer](https://github.com/ssloy/tinyrenderer). It shows you how OpenGL works by making you remake a tiny version of it. IDK how to write C++ code, but I like the idea of making a renderer from scratch, so I'm either going to do it or get stuck on how to run a C++ program.

## Progress

I'm going to periodically update this whenever I feel like it. I'm not going to worry about keeping this up to date, since the purpose of this project is to learn more about C++ and OpenGL, not write a blog.

### 11/02 - 11/04

Finished lesson 1

While I did copy most of the code from the guy's "snapshots," I did write the code for making the lines all by myself

![image](display/lesson1_wireframe.tga)

## 11/05 - 11/06

Tried to do lesson 2. It did not go well
First, I discovered that my line code had a lot of problems, so I had to scratch that and import the guy's line code
Then, I had a lot of problems regarding the triangle
Long story short, I'm taking a break from this project

![image](lesson2_failedtriangles.tga)

## 11/07

I'm not taking a break. I WILL NOT LET THE TRIANGLES BEAT ME!
Current plan:
- Figure out how other rendering libraries like [this one](https://www.rose-hulman.edu/class/csse/resources/Python/ZelleGraphics.html)
- Try to figure a different way of making triangles (the one I tried before was garbage)

EDIT: Turns out, that example graphics library used another graphics library called [tkinter](https://realpython.com/python-gui-tkinter/)
      If I can find the source code of that, that would work better

## 11/30

I had to take a break
Long story short, a friend recommended I learn linear algebra, since vectors are a big part of how renderers work
I'm also thinging of changing my primary language to Python, as I already know that
I downloaded some libraries to test out, and the next update will detail the results of those experiments

Here are some resources I found
- [pyTGA](https://github.com/MircoT/pyTGA)
- [Some stuff on how rasterization works](https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/projection-stage.html)
- [Khan Academy is amazing](https://www.khanacademy.org/math/linear-algebra/vectors-and-spaces/vectors/v/linear-algebra-parametric-representations-of-lines)

EDIT: I'm going to have to figure out how to install pyTGA safely if I am going to use it. I also need to evaluate my options carefully. I do NOT want to discover halfway though the tutorial that my core graphics library is flawed

