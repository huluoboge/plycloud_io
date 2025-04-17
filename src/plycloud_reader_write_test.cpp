#include "plycloud_point.hpp"
#include "plycloud_reader.hpp"
#include "plycloud_writer.hpp"
#include <iostream>

struct MyPoint {
    float x, y, z;
    uint8_t r, g, b;
};
struct MyPoint2 {
    float x, y, z;
    float r, g, b;
};

// custom make_setter
template <>
std::shared_ptr<plyio::PointAttributeSetter<MyPoint>> plyio::make_setter()
{
    auto setter = std::make_shared<plyio::PointAttributeSetter<MyPoint>>();
    addXYZSetter<MyPoint>(setter);
    addRGBSetter<MyPoint>(setter);
    return setter;
}

template <>
std::shared_ptr<plyio::PointAttributeSetter<MyPoint2>> plyio::make_setter()
{
    auto setter = std::make_shared<plyio::PointAttributeSetter<MyPoint2>>();
    addXYZSetter<MyPoint2>(setter);
    addRGBSetter<MyPoint2>(setter);
    return setter;
}
REGISTER_PLY_WRITE_POINT(MyPoint2, (float, x, x)(float, y, y)(float, z, z)(float, red, r)(float, green, g)(float, blue, b))

struct MyPoint3 {
    float x, y, z;
    float red, green, blue;
};

// regist reader function(auto generate make_setter function) .
// different to write point, you can regist reader function to map one ply attribute to different function
//
REGISTER_PLY_READ_POINT(MyPoint3, (float, x, x)(float, y, y)(float, z, z)(float, red, red)(float, green, green)(float, blue, blue)(float, r, red)(float, g, green)(float, b, blue))

std::string ply_write_test(bool binary)
{
    // Instantiate a point with coordinates (1, 2, 3)
    plyio::PlyPointXYZIRGBN X;
    X.x = 1;
    X.y = 2;
    X.z = 3;
    X.r = 155;
    X.g = 0;
    X.b = 66;
    X.intensity = 0.7;
    X.nx = 0.1;
    X.ny = 0.8;
    X.nz = 0.3;
    // Create a stringstream to hold the output
    std::stringstream ss;

    // Initialize a PlyPointStreamWriter to write points into the stringstream
    plyio::PlyPointStreamWriter<plyio::PlyPointXYZIRGBN> writer(ss);

    // Set the output format to ASCII (non-binary)
    writer.setBinary(binary);

    // Write the header information for the PLY file
    writer.writeHead();

    // Write four identical points to the stream
    writer.writePoint(X);
    writer.writePoint(X);
    writer.writePoint(X);
    writer.writePoint(X);

    // Update the header to reflect the actual number of points written
    writer.updateHead();

    // Output the stringstream content to console
    std::cout << ss.str() << std::endl;
    return ss.str();
}

std::string ply_write_test2(bool binary)
{
    MyPoint2 X;
    X.x = 1;
    X.y = 2;
    X.z = 3;
    X.r = 155;
    X.g = 0;
    X.b = 66;
    // Create a stringstream to hold the output
    std::stringstream ss;

    // Initialize a PlyPointStreamWriter to write points into the stringstream
    plyio::PlyPointStreamWriter<MyPoint2> writer(ss);

    // Set the output format to ASCII (non-binary)
    writer.setBinary(binary);

    // Write the header information for the PLY file
    writer.writeHead();

    // Write four identical points to the stream
    writer.writePoint(X);
    writer.writePoint(X);
    writer.writePoint(X);
    writer.writePoint(X);

    // Update the header to reflect the actual number of points written
    writer.updateHead();

    // Output the stringstream content to console
    std::cout << ss.str() << std::endl;
    return ss.str();
}

int ply_read_test(const std::string& s)
{
    std::stringstream ss(s);
    using namespace plyio;
    try {
        plyio::PlyPointStreamReader reader(ss);

        if (!reader.readHead()) {
            std::cerr << "Failed to read PLY header." << std::endl;
            return 1;
        }

        reader.printHeader();
        reader.beginReadPoint<MyPoint>();

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

int ply_read_test2(const std::string& s)
{
    std::stringstream ss(s);
    using namespace plyio;
    try {
        plyio::PlyPointStreamReader reader(ss);

        if (!reader.readHead()) {
            std::cerr << "Failed to read PLY header." << std::endl;
            return 1;
        }

        reader.printHeader();

        std::shared_ptr<PointAttributeSetter<MyPoint>> setter = std::make_shared<PointAttributeSetter<MyPoint>>();
        setter->registerAttribute("x", set_x<MyPoint>);
        setter->registerAttribute("y", set_y<MyPoint>);
        setter->registerAttribute("z", set_z<MyPoint>);
        setter->registerAttribute("red", [](MyPoint& pt, const PlyDataType& data) {
            pt.r = data.as<uint8_t>();
        });
        setter->registerAttribute("r", [](MyPoint& pt, const PlyDataType& data) {
            pt.r = data.as<uint8_t>();
        });
        setter->registerAttribute("green", [](MyPoint& pt, const PlyDataType& data) {
            pt.g = data.as<uint8_t>();
        });
        setter->registerAttribute("g", [](MyPoint& pt, const PlyDataType& data) {
            pt.g = data.as<uint8_t>();
        });
        setter->registerAttribute("blue", [](MyPoint& pt, const PlyDataType& data) {
            pt.b = data.as<uint8_t>();
        });
        setter->registerAttribute("b", [](MyPoint& pt, const PlyDataType& data) {
            pt.b = data.as<uint8_t>();
        });

        reader.prepareReadFunction(setter);

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

int ply_read_test3(const std::string& s)
{
    std::cout<<"TEST READ POINT3"<<std::endl;
    std::stringstream ss(s);
    using namespace plyio;
    try {
        plyio::PlyPointStreamReader reader(ss);

        if (!reader.readHead()) {
            std::cerr << "Failed to read PLY header." << std::endl;
            return 1;
        }

        reader.printHeader();

        reader.beginReadPoint<MyPoint3>();
        for (int i = 0; i < reader.pointSize(); ++i) {
            MyPoint3 pt;
            reader.readPoint(pt);
            std::cout << "Point: (" << pt.x << ", " << pt.y << ", " << pt.z << "), Color: ("
                      << static_cast<int>(pt.red) << ", " << static_cast<int>(pt.green) << ", " << static_cast<int>(pt.blue) << ")" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
int main()
{
    std::string assciitex = ply_write_test(false);
    std::string binarytex = ply_write_test(true);
    ply_read_test(assciitex);
    ply_read_test2(assciitex);

    ply_read_test(binarytex);
    ply_read_test2(binarytex);

    /////save float rgb, and readout uchar
    std::cout << "========================" << std::endl;
    std::string asstex2 = ply_write_test2(false);
    std::string bintex2 = ply_write_test2(true);

    ply_read_test(asstex2);
    ply_read_test2(asstex2);

    ply_read_test(bintex2);
    ply_read_test2(bintex2);

    ply_read_test3(binarytex);
    ply_read_test3(bintex2);
    return 0;
}