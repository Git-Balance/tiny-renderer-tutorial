# Tiny Renderer Tutorial Log

## Why I'm doing this

When I started this project in 2023, I wanted to experiment with lower level languages. My college C class taught me its basics, but I did not know how to program my own projects. Meanwhile, I had an interest is graphics programming: things like OpenGL and renderers.

Then, I found a project on GitHub called [Tiny Renderer](https://github.com/ssloy/tinyrenderer), which is a tutorial series which guides you through making your own renderer without any third-party libraries. Despite the fact that I had was unexperienced with C++ (the language used by the tutorial), I decided to try it.

## Overview of Progress

During November 2023 and Feburary 2024, I attempted to vaguely follow the tutorial, but coming up to my own solutions at various points. I wrote down my experience in the README.md. While I did not get far in the tutorial, it was a fun experience that exposed me to C++ for the first time.

In 2025 (the time I am writing this), I decided to revisit this project for 2 reasons:
1. I wanted to continue with the project, since it was interesting
2. I learned became a more experienced C++ programmer and I wanted to revise what I had done so far

The tutorial changed during my break; it moved from a [GitHub Wiki](https://github.com/ssloy/tinyrenderer/wiki/Lesson-0:-getting-started) to a [dedicated website](https://haqr.eu/tinyrenderer/)

The progress enteries were moved from the README.md to here

## Progress

### 11/02/23 - 11/04/23

Finished lesson 1

While I did copy most of the code from the guy's "snapshots," I did write the code for making the lines all by myself

![image](display/lesson1_wireframe.png)

### 11/05/23 - 11/06/23

I tried to do lesson 2. It did not go well

First, I discovered that my line code had a lot of problems, so I had to scratch that and import the guy's line code

Then, I had a lot of problems regarding the triangle

Long story short, I'm taking a break from this project

![image](display/lesson2_failedtriangles.png)

### 11/07/23

I'm not taking a break. I WILL NOT LET THE TRIANGLES BEAT ME!

Current plan:
- Figure out how other rendering libraries like [this one](https://www.rose-hulman.edu/class/csse/resources/Python/ZelleGraphics.html)
- Try to figure a different way of making triangles (the one I tried before was garbage)

EDIT: Turns out, that example graphics library used another graphics library called [tkinter](https://realpython.com/python-gui-tkinter/)
      If I can find the source code of that, that would work better

### 11/30/23

I had to take a break

Long story short, a friend recommended I learn linear algebra, since vectors are a big part of how renderers work

I'm also thinging of changing my primary language to Python, as I already know that

I downloaded some libraries to test out, and the next update will detail the results of those experiments

Here are some resources I found
- [pyTGA](https://github.com/MircoT/pyTGA)
- [Some stuff on how rasterization works](https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/projection-stage.html)
- [Khan Academy is amazing](https://www.khanacademy.org/math/linear-algebra/vectors-and-spaces/vectors/v/linear-algebra-parametric-representations-of-lines)

EDIT: I'm going to have to figure out how to install pyTGA safely if I am going to use it. I also need to evaluate my options carefully. I do NOT want to discover halfway though the tutorial that my core graphics library is flawed

### 12/04/23

Started testing how various parts of this project works with python and matplotlib, cus it is easier

### 12/12/23

Taking a different direction from what I planned

Long story short, I am completely rewriting the code for myline, so that I understand how it works

I should have tried to make a line function on my own from the start

### 01/05/24

I finally got the myline code working

Currently, I am only on [step 1](https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm) of the tutorial

If I looked closely, I could have seen that the algorithm that was used by the teacher had a name that I could have looked up and has lots of information online about it

Oh well, at least I can say that I made my own line algorithm
My line drawing algorithm uses y=mx+b

It finds all the values in that equation, then only draws the parts of the equation that is needed

I am proud of my algorithm

HOWEVER, I only have the basic algorithm of step 1.1, I still need to make the rest of the algorithm so it can handle steep lines and stuff like that

![image](display/lesson1_successfulstart.png)

### 01/05/24 - continued

Long story short, step 1.2 and 1.3 were completed

Step 1.2:
- Turns out, I already completed this step earlier
- This handles every "step" (every time a pixel is created), and my algorithm already handles this

Step 1.3:
- This step covers handling steep lines and right-to-left lines
- The code needs to be cleaned up a lot, but otherwise I think it is good

![image](display/lesson1_step2and3.png)

### 01/06/24

The code was cleaned up a little 
- Comments were removed
- A bug was patched out regarding steep left-to-right lines
- The original line code was removed (myline is now just called line)

Also, a wireframe version of the face was made with the new line

Lesson 1 is done

![image](display/lesson1_newwireframe.png)

### 01/25/24

Had some IRL stuff that got in the way of this project
However, I'm on the right track

I figured out how the basics of how I am going to render a triangle

I did some tests in Python

### 02/05/24

I figured out how to work on this project portably

Currently, I use CoCalc of I want to work to work on a programming project portablly

However, CoCalc doesn't allow you to install programs, and it didn't have any preinstalled programs that could view TGA images

I found a workaround in the form of the AppImage version of ImageMagick

ImageMagick allows you to convert an image to a different format, and the AppImage version can be ran on CoCalc (after using --appimage-extract)

TLDR, I can work on this project more consistently

On a side note, I did some testing in python

Initially, I just wanted to see if a Jupyter Notebook supported TGA images

Then it spiralled into me trying to recreate the entire renderer so far in Python because I was bored

It didn't end up working, but it made me think about what I have doon so far

For example, why does the line need to be drawn left-to-right?
My line algorith is different than the one in the tutorial, so I should test that, right?

Additionally, I am thinking about if turning the lines into classes

I probably won't turn the lines or tiangles or other shapes into classes, but it is still a good idea to think about my code along these lines

### 02/08/24

I am working on the triangle code, and I want to document what I have done so far

First, the vectors that represent the 3 points of a triangle are sorted from the highest y to lowest y

Then, the triangle code finds the slope and y intercept of the 3 sides before drawing anything

This is where I am at currently

Additionally, I attached a picture to show what I currently have for future reference on what changes

![image](display/lesson2_originalversion.png)

### 02/11/24

I got a cool glitch

I'm don't even know how this went so perfectly wrong

![image](display/lesson2_coolfail.png)

### 02/11/24 - continued

I almost have the triangle code working

All I need to do is figure out why the triangle doesn't cover the entire area

It probably is an off by one error

![image](display/lesson2_almostgood.png)

### 02/12/24 - 02/20/24

I am having a minor bug that is causing triangles to be drawn in a weird way

It has to with the way vectors are inputed into a triangle

It is easier to look at an example image

![image](display/lesson2_incorrecttriangle.png)

### 02/27/24

I edited the README.md file to fix some formatting issues
- Separated paragraphs by 2 newlines instead of 1
    - I did not realize that Markdown requires 2 newlines, because I usually use a Markdown editor like Obsidian instead of a TUI editor like Vim
- Convert all TGA images into PNG images
    - I have been wanting to do this ever since I learned about ImageMagick, now was just the most convenient time

### 10/29/25

In general, everything was reformated and reorganized over the past few days

- Moved the code files (main.cpp, model.h, etc.) from the root directory to `src/`
- Started building project using `cmake` instead of typing the build/command manually each time
    - (Trying to remember what parameters `g++` needed every time I returned to the project was becoming a serious problem. This was a needed addition)
- Renamed `display/` to `log/`
- Created a `.gitignore`
- Moved the test files to a separate repository
- Created `log.md` to host this diary of progress
- Cleared `README.md`
    - Later it will be rewritten to more accurately describe the current state of the project and have a general guide on compiling the project

The actual code itself will be reformated in a separate log entry

