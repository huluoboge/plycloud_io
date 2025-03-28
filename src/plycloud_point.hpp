
#ifndef PLYCLOUD_IO_PLYCLOUD_POINT_HPP
#define PLYCLOUD_IO_PLYCLOUD_POINT_HPP

#include "plycloud_io.hpp"

namespace plyio {

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
    PlyPointXYZ(float _x, float _y, float _z)
        : x(_x)
        , y(_y)
        , z(_z)
    {
    }
    float& operator[](int i) { return data[i]; };
    float operator[](int i) const { return data[i]; };
};

struct PlyPointXYZI : public PlyPointXYZ {
    float intensity = 0;
};
struct PlyPointXYZRGB : public PlyPointXYZ {
    uint8_t r = 0, g = 0, b = 0;
};

struct PlyPointXYZN : public PlyPointXYZ {
    float nx = 0, ny = 0, nz = 0;
};

struct PlyPointXYZIRGB : public PlyPointXYZ {
    float intensity = 0;
    uint8_t r = 0, g = 0, b = 0;
};

struct PlyPointXYZIN : public PlyPointXYZ {
    float intensity = 0;
    float nx = 0, ny = 0, nz = 0;
};

struct PlyPointXYZRGBN : public PlyPointXYZ {
    float nx = 0, ny = 0, nz = 0;
    uint8_t r = 0, g = 0, b = 0;
};
struct PlyPointXYZIRGBN : public PlyPointXYZ {
    float intensity = 0;
    float nx = 0, ny = 0, nz = 0;
    uint8_t r = 0, g = 0, b = 0;
};

}

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZ, (float, x, x)(float, y, y)(float, z, z))
REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZI, (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity))
REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZRGB,
    (float, x, x)(float, y, y)(float, z, z)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))
REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZN, (float, x, x)(float, y, y)(float, z, z)(float, nx, nx)(float, ny, ny)(float, nz, nz))

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZIRGB,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZIN,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)(float, nx, nx)(float, ny, ny)(float, nz, nz))

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZRGBN,
    (float, x, x)(float, y, y)(float, z, z)(float, nx, nx)(float, ny, ny)(float, nz, nz)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZIRGBN,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)(float, nx, nx)(float, ny, ny)(float, nz, nz)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))

// MAKE_SETTER(plyio::PlyPointXYZIRGBN, addNormalSetter(setter); addNormalSetter(setter);)
// custom your point type

#endif // PLYCLOUD_IO_PLYCLOUD_POINT_HPP