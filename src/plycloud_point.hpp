
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

struct PlyPointXYZNV : public PlyPointXYZN{
    int32_t view_id = 0;
};

struct PlyPointXYZNVRGB: PlyPointXYZNV{
    uint8_t r=0, g=0, b=0;
};

struct PlyPointXYZNVEIT : public PlyPointXYZNV{
    float emit_x=0, emit_y=0, emit_z=0;
    float intensity = 0;
    float timestamp = 0;
};

struct PlyPointXYZNVEITRGB : public PlyPointXYZNVEIT{
    uint8_t r=0, g=0, b=0;
};

struct PlyPointXYZNVEITRGBC : public PlyPointXYZNVEITRGB{
    uint8_t classification; // classification
};

}

// regist write point
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

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZNV,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
)

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZNVRGB,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
)

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZNVEIT,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
    (float, emit_x, emit_x)(float, emit_y, emit_y)(float, emit_z, emit_z)
    (float, intensity, intensity)
    (float, timestamp, timestamp)
)

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZNVEITRGB,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
    (float, emit_x, emit_x)(float, emit_y, emit_y)(float, emit_z, emit_z)
    (float, intensity, intensity)
    (float, timestamp, timestamp)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
)

REGISTER_PLY_WRITE_POINT(plyio::PlyPointXYZNVEITRGBC,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
    (float, emit_x, emit_x)(float, emit_y, emit_y)(float, emit_z, emit_z)
    (float, intensity, intensity)
    (float, timestamp, timestamp)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
    (uint8_t, classification, classification)
)

/*
"Reading a file is different from writing.
To accommodate different attribute names that may appear in a PLY file, \
for example, normal_x might also be called nx.
 Therefore, multiple functions can be registered for a single attribute name,
 allowing for the reading of various cases."
*/

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZ, (float, x, x)(float, y, y)(float, z, z))
REGISTER_PLY_READ_POINT(plyio::PlyPointXYZI, (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity))
REGISTER_PLY_READ_POINT(plyio::PlyPointXYZRGB,
    (float, x, x)(float, y, y)(float, z, z)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)(uint8_t, red, r)(uint8_t, green, g)(uint8_t, blue, b))
REGISTER_PLY_READ_POINT(plyio::PlyPointXYZN,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz) 
    (float, normal_x, nx)(float, normal_y, ny)(float, normal_z, nz)
    )

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZIRGB,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
    (uint8_t, red, r)(uint8_t, green, g)(uint8_t, blue, b)
)

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZIN,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (float, normal_x, nx)(float, normal_y, ny)(float, normal_z, nz)
)

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZRGBN,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (float, normal_x, nx)(float, normal_y, ny)(float, normal_z, nz)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
    (uint8_t, red, r)(uint8_t, green, g)(uint8_t, blue, b)
)

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZIRGBN,
    (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (float, normal_x, nx)(float, normal_y, ny)(float, normal_z, nz)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
    (uint8_t, red, r)(uint8_t, green, g)(uint8_t, blue, b)
)

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZNV,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
)

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZNVRGB,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
)

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZNVEIT,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
    (float, emit_x, emit_x)(float, emit_y, emit_y)(float, emit_z, emit_z)
    (float, intensity, intensity)
    (float, timestamp, timestamp)
)

REGISTER_PLY_READ_POINT(plyio::PlyPointXYZNVEITRGB,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
    (float, emit_x, emit_x)(float, emit_y, emit_y)(float, emit_z, emit_z)
    (float, intensity, intensity)
    (float, timestamp, timestamp)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
)


REGISTER_PLY_READ_POINT(plyio::PlyPointXYZNVEITRGBC,
    (float, x, x)(float, y, y)(float, z, z)
    (float, nx, nx)(float, ny, ny)(float, nz, nz)
    (int32_t, view_id, view_id)
    (float, emit_x, emit_x)(float, emit_y, emit_y)(float, emit_z, emit_z)
    (float, intensity, intensity)
    (float, timestamp, timestamp)
    (uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b)
    (uint8_t, classification, classification)
)

#endif // PLYCLOUD_IO_PLYCLOUD_POINT_HPP