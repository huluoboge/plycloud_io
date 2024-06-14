
#ifndef PLYCLOUD_IO_HPP
#define PLYCLOUD_IO_HPP

#include <boost/preprocessor/cat.hpp>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

template <typename value_type>
struct PlyPropertyTraits {
    static std::ostream& write(std::ostream& os, const std::string& name);
};

template <>
struct PlyPropertyTraits<int8_t> {
    static std::ostream& write(std::ostream& os, const std::string& name)
    {
        os << "property char " << name << std::endl;
        return os;
    }
};

template <>
struct PlyPropertyTraits<uint8_t> {
    static std::ostream& write(std::ostream& os, const std::string& name)
    {
        os << "property uchar " << name << std::endl;
        return os;
    }
};

template <>
struct PlyPropertyTraits<int16_t> {
    static std::ostream& write(std::ostream& os, const std::string& name)
    {
        os << "property short " << name << std::endl;
        return os;
    }
};

template <>
struct PlyPropertyTraits<uint16_t> {
    static std::ostream& write(std::ostream& os, const std::string& name)
    {
        os << "property ushort " << name << std::endl;
        return os;
    }
};

template <>
struct PlyPropertyTraits<int> {
    static std::ostream& write(std::ostream& os, const std::string& name)
    {
        os << "property int " << name << std::endl;
        return os;
    }
};

template <>
struct PlyPropertyTraits<uint32_t> {
    static std::ostream& write(std::ostream& os, const std::string& name)
    {
        os << "property uint " << name << std::endl;
        return os;
    }
};

template <>
struct PlyPropertyTraits<float> {
    static std::ostream& write(std::ostream& os, const std::string& name)
    {
        os << "property float " << name << std::endl;
        return os;
    }
};

template <>
struct PlyPropertyTraits<double> {
    static std::ostream& write(std::ostream& os, const std::string& name)
    {
        os << "property double " << name << std::endl;
        return os;
    }
};

namespace {

template <typename T>
struct type_val {
    enum { type = 0 };
};

template <>
struct type_val<float> {
    enum { type = 1 };
};
template <>
struct type_val<double> {
    enum { type = 1 };
};

inline std::string EncodeCount(size_t count)
{
    std::string buffer;
    buffer.resize(64, ' ');
    std::string sCount = std::to_string(count);
    std::copy(sCount.begin(), sCount.end(), buffer.begin());
    return buffer;
}

inline std::ostream& WritePlyHead(std::ostream& os, bool binary, size_t count)
{
    os << std::fixed << std::setprecision(8);
    os << "ply\n"
       << "format " << (binary ? "binary_little_endian 1.0" : "ascii 1.0") << '\n'
       << "comment generated by plycloud\n"
       << "element vertex " << EncodeCount(count) << '\n';
    return os;
}

}

template <typename value_type>
std::ostream& WritePointProp(std::ostream& os, bool binary, value_type val)
{
    if (binary) {
        os.write(reinterpret_cast<const char*>(&val), sizeof(value_type));
    } else {
        if (type_val<value_type>::type == 0) {
            os << int(val) << ' ';
        } else {
            os << val << ' ';
        }
    }
    return os;
}

template <typename PointType>
struct PlyWriteHeadTraits {
    static std::ostream& writePointHead(std::ostream& os, bool binary, size_t count);
};
template <typename PointType>
struct PlyWritePointTraits {
    static std::ostream& writePoint(std::ostream& os, bool binary, const PointType& point);
};

#define PLY_REGIST_WRITE_HEAD(PointType, ...)                                            \
    template <>                                                                          \
    struct PlyWriteHeadTraits<PointType> {                                               \
        static std::ostream& writePointHead(std::ostream& os, bool binary, size_t count) \
        {                                                                                \
            WritePlyHead(os, binary, count);                                             \
            __VA_ARGS__                                                                  \
            os << "end_header\n";                                                        \
            return os;                                                                   \
        }                                                                                \
    };

#define PLY_REGIST_WRITE_POINT(PointType, ...)                                                 \
    template <>                                                                                \
    struct PlyWritePointTraits<PointType> {                                                    \
        static std::ostream& writePoint(std::ostream& os, bool binary, const PointType& point) \
        {                                                                                      \
            __VA_ARGS__                                                                        \
            if (!binary) {                                                                     \
                os << std::endl;                                                               \
            }                                                                                  \
            return os;                                                                         \
        }                                                                                      \
    };

#define PLY_WRITE_PROPERTY(TYPE, NAME) \
    PlyPropertyTraits<TYPE>::write(os, #NAME);

#define PLY_WRITER_POINT(TYPE, NAME) \
    WritePointProp<TYPE>(os, binary, point.NAME);

// These macros help transform the unusual data structure (type, name, tag)(type, name, tag)...
// into a proper preprocessor sequence of 3-tuples ((type, name, tag))((type, name, tag))...

#define REGISTER_POINT_STRUCT_X(type, name) \
    PLY_WRITE_PROPERTY(type, name)          \
    REGISTER_POINT_STRUCT_Y
#define REGISTER_POINT_STRUCT_Y(type, name) \
    PLY_WRITE_PROPERTY(type, name)          \
    REGISTER_POINT_STRUCT_X

#define REGISTER_POINT_STRUCT_X0
#define REGISTER_POINT_STRUCT_Y0

#define REGISTER_POINT_STRUCT_XX(type, name) \
    PLY_WRITER_POINT(type, name)             \
    REGISTER_POINT_STRUCT_YY
#define REGISTER_POINT_STRUCT_YY(type, name) \
    PLY_WRITER_POINT(type, name)             \
    REGISTER_POINT_STRUCT_XX

#define REGISTER_POINT_STRUCT_XX0
#define REGISTER_POINT_STRUCT_YY0

// Must be used in global namespace with name fully qualified
#define REGISTER_POINT_STRUCT_HEAD(name, seq) \
    PLY_REGIST_WRITE_HEAD(name,               \
        BOOST_PP_CAT(REGISTER_POINT_STRUCT_X seq, 0))

#define REGISTER_POINT_STRUCT_POINT(name, seq) \
    PLY_REGIST_WRITE_POINT(name,               \
        BOOST_PP_CAT(REGISTER_POINT_STRUCT_XX seq, 0))

#define REGISTER_PLY_WRITE_POINT(name, seq) \
    REGISTER_POINT_STRUCT_HEAD(name, seq)   \
    REGISTER_POINT_STRUCT_POINT(name, seq)

/*
stream write points
*/
template <typename PointType>
struct PlyPointStreamWriter {
    using point_type = PointType;

    struct Header {
        size_t count = 0;
        bool isBin = true;
        size_t bodyPosition = 0;
    };

    PlyPointStreamWriter(std::ostream& _os)
        : os(_os)
    {
    }

    void setBinary(bool bin)
    {
        head.isBin = bin;
    }

    bool isBinary() const
    {
        return head.isBin;
    }

    bool writeHead()
    {
        os.clear();
        os.seekp(0, os.beg);
        PlyWriteHeadTraits<PointType>::writePointHead(os, head.isBin, head.count);
        head.bodyPosition = os.tellp();
        return !os.fail();
    }

    void printHeader() const
    {
        std::string msg = head.isBin ? "is banary" : "is ascii";
        std::cout << msg << std::endl;
        std::cout << "count = " << head.count << std::endl;
    }

    void beginWritePoint()
    {
        os.clear();
        os.seekp(head.bodyPosition, os.beg);
        head.count = 0;
    }

    void updateHead()
    {
        os.clear();
        os.seekp(0, os.beg);
        PlyWriteHeadTraits<PointType>::writePointHead(os, head.isBin, head.count);
        size_t pos = os.tellp();
        if (pos != head.bodyPosition) {
            std::cout << "[ERROR]: pos != head.bodyPosition" << std::endl;
        }
        os.seekp(0, os.end);
        os.flush();
    }

    size_t pointSize() const
    {
        return head.count;
    }

    bool writePoint(const PointType& pt)
    {
        PlyWritePointTraits<PointType>::writePoint(os, head.isBin, pt);
        ++head.count;
        return !os.fail();
    }

    Header head;
    std::ostream& os;
};

template <typename PointType>
struct PlyPointFileStreamWriter : public PlyPointStreamWriter<PointType> {
public:
    PlyPointFileStreamWriter(const std::string& file)
        : PlyPointStreamWriter<PointType>(ofs)
    {
        ofs.open(file, std::ios::binary | std::ios::trunc | std::ios::out);
        plyFile = file;
    }

    bool isOpen() const
    {
        return ofs.is_open();
    }

    void close()
    {
        ofs.close();
    }

    std::string file() const
    {
        return plyFile;
    }

    PlyPointFileStreamWriter(const PlyPointFileStreamWriter&) = delete;
    PlyPointFileStreamWriter& operator=(const PlyPointFileStreamWriter&) = delete;

protected:
    std::string plyFile;
    std::ofstream ofs;
};

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

REGISTER_PLY_WRITE_POINT(PlyPointXYZ, (float, x)(float, y)(float, z))

struct PlyPointXYZI : public PlyPointXYZ {
    float intensity = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZI, (float, x)(float, y)(float, z)(float, intensity))

struct PlyPointXYZRGB : public PlyPointXYZ {
    uint8_t r = 0, g = 0, b = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZRGB,
    (float, x)(float, y)(float, z)(uint8_t, r)(uint8_t, g)(uint8_t, b))

struct PlyPointXYZN : public PlyPointXYZ {
    float nx = 0, ny = 0, nz = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZN, (float, x)(float, y)(float, z)(float, nx)(float, ny)(float, nz))

struct PlyPointXYZIRGB : public PlyPointXYZ {
    float intensity = 0;
    uint8_t r = 0, g = 0, b = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZIRGB,
    (float, x)(float, y)(float, z)(float, intensity)(uint8_t, r)(uint8_t, g)(uint8_t, b))

struct PlyPointXYZIN : public PlyPointXYZ {
    float intensity = 0;
    float nx = 0, ny = 0, nz = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZIN,
    (float, x)(float, y)(float, z)(float, intensity)(float, nx)(float, ny)(float, nz))

struct PlyPointXYZRGBN : public PlyPointXYZ {
    float nx = 0, ny = 0, nz = 0;
    uint8_t r = 0, g = 0, b = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZRGBN,
    (float, x)(float, y)(float, z)(float, nx)(float, ny)(float, nz)(uint8_t, r)(uint8_t, g)(uint8_t, b))

struct PlyPointXYZIRGBN : public PlyPointXYZ {
    float intensity = 0;
    float nx = 0, ny = 0, nz = 0;
    uint8_t r = 0, g = 0, b = 0;
};

REGISTER_PLY_WRITE_POINT(PlyPointXYZIRGBN,
    (float, x)(float, y)(float, z)(float, intensity)(float, nx)(float, ny)(float, nz)(uint8_t, r)(uint8_t, g)(uint8_t, b))

// custom your point type

#endif // PLYCLOUD_IO_HPP