
#ifndef PLYCLOUD_IO_PLYCLOUD_POINT_HPP
#define PLYCLOUD_IO_PLYCLOUD_POINT_HPP

#include "plycloud_io.hpp"
// some point struct
struct PlyPointXYZ {
    union {
        /* data */
        struct {
            float x;
            float y;
            float z;
        };
        float data[3];
    };
    PlyPointXYZ()
        : x(0)
        , y(0)
        , z(0)
    {
    }

    float& operator[](int i) { return data[i]; };
    float operator[](int i) const { return data[i]; };
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZ, (float, x, x)(float, y, y)(float, z, z))

struct PlyPointXYZI : public PlyPointXYZ {
    float intensity = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZI, (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity))

struct PlyPointXYZRGB : public PlyPointXYZ {
    uint8_t r = 0, g = 0, b = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZRGB,
    (float, x, x)(float, y, y)(float, z, z)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))

struct PlyPointXYZN : public PlyPointXYZ {
    float nx = 0, ny = 0, nz = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZN, (float, x, x)(float, y, y)(float, z, z)(float, nx, nx)(float, ny, ny)(float, nz, nz))

struct PlyPointXYZIRGB : public PlyPointXYZ {
    float intensity = 0;
    uint8_t r = 0, g = 0, b = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZIRGB,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))

struct PlyPointXYZIN : public PlyPointXYZ {
    float intensity = 0;
    float nx = 0, ny = 0, nz = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZIN,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)(float, nx, nx)(float, ny, ny)(float, nz, nz))

struct PlyPointXYZRGBN : public PlyPointXYZ {
    float nx = 0, ny = 0, nz = 0;
    uint8_t r = 0, g = 0, b = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZRGBN,
    (float, x, x)(float, y, y)(float, z, z)(float, nx, nx)(float, ny, ny)(float, nz, nz)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))

struct PlyPointXYZIRGBN : public PlyPointXYZ {
    float intensity = 0;
    float nx = 0, ny = 0, nz = 0;
    uint8_t r = 0, g = 0, b = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZIRGBN,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)(float, nx, nx)(float, ny, ny)(float, nz, nz)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))

// custom your point type

#endif // PLYCLOUD_IO_PLYCLOUD_POINT_HPP