#include "plycloud_reader.hpp"
#include <iostream>

struct MyPoint {
    float x, y, z;
    uint8_t r, g, b;
};

template <typename PointType>
void set_x(PointType& pt, const PlyDataType& data)
{
    pt.x = data.floatVal;
}

template <typename PointType>
void set_y(PointType& pt, const PlyDataType& data)
{
    pt.y = data.floatVal;
}

template <typename PointType>
void set_z(PointType& pt, const PlyDataType& data)
{
    pt.z = data.floatVal;
}

int main()
{
    const char* file = "full_all.ply";
    try {
        PlyPointFileStreamReader reader(file);

        if (!reader.readHead()) {
            std::cerr << "Failed to read PLY header." << std::endl;
            return 1;
        }

        reader.printHeader();

        PointAttributeSetter<MyPoint> setter;
        setter.registerAttribute("x", set_x<MyPoint>);
        setter.registerAttribute("y", set_y<MyPoint>);
        setter.registerAttribute("z", set_z<MyPoint>);
        setter.registerAttribute("red", [](MyPoint& pt, const PlyDataType& data) { pt.r = data.ucharVal; });
        setter.registerAttribute("r", [](MyPoint& pt, const PlyDataType& data) { pt.r = data.ucharVal; });
        setter.registerAttribute("green", [](MyPoint& pt, const PlyDataType& data) { pt.g = data.ucharVal; });
        setter.registerAttribute("g", [](MyPoint& pt, const PlyDataType& data) { pt.g = data.ucharVal; });
        setter.registerAttribute("blue", [](MyPoint& pt, const PlyDataType& data) { pt.b = data.ucharVal; });
        setter.registerAttribute("b", [](MyPoint& pt, const PlyDataType& data) { pt.b = data.ucharVal; });

        reader.prepareReadFunction(&setter);

        for (auto it = reader.begin<MyPoint>(); it != reader.end<MyPoint>(); ++it) {
            const MyPoint& pt = *it;
            std::cout << "Point: (" << pt.x << ", " << pt.y << ", " << pt.z << "), Color: (" << static_cast<int>(pt.r) << ", " << static_cast<int>(pt.g) << ", " << static_cast<int>(pt.b) << ")" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}