# Tiny Renderer Tutorial

Using [the TinyRenderer course by Dmitry V. Sokolov](https://haqr.eu/tinyrenderer/), I am making a renderer that turns [wavefront .obj files](https://en.wikipedia.org/wiki/Wavefront_.obj_file) into a 2d image of a model. The course uses C++ and a library that can create and make simple edits to [TGA files](https://en.wikipedia.org/wiki/Wavefront_.obj_file); everything else (drawing lines, loading files, etc.) has to be coded from scratch.

## Logs

I created a log in `log/log.md` which covers my progress through the tutorial

## Build and Run

This repository is where I experiment and follow the tiny renderer tutorial. There is no guarantee that the current commit will be able to be built successfully

These instructions are made for Bash on Linux

Build by running the following commands
```bash
cmake -B build
cmake --build build
```

Run by running the following command
```bash
./build/tinyrenderer
```

