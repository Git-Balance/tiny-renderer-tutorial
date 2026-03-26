#pragma once

struct Vertex {
    union {
        float xyz[3];
        struct {
            float x;
            float y;
            float z;
        };
    };
    float& operator[](const int i) { return xyz[i]; }
};

struct Face {
    /*
        f 6/1/6 5/2/5 8/3/8
        f 6/1/6 8/3/8 7/4/7
        f 12/5/12 11/6/11 10/7/10
     */

    // NOTE: While obj files start their indexes at 1 (at least for faces), the Face struct's indexes will start at 0

    // Refrence: f v1/__/__ v2/__/__ v3/__/__

    union {
        // Vertexes
        int v[3];
        struct {
            int v1;
            int v2;
            int v3;
        };
    };
    // Future variables will be added later
};

struct Vec3f {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float xyz[3];
    };
};

struct Vec3i {
    union {
        struct {
            int x;
            int y;
            int z;
        };
        int xyz[3];
    };
};
