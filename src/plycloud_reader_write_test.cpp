#include "plycloud_point.hpp"
#include "plycloud_reader.hpp"
#include "plycloud_writer.hpp"
#include <iostream>

struct MyPoint {
    float x, y, z;
    uint8_t r, g, b;
};

std::string ply_write_trest()
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
    writer.setBinary(false);

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

        std::shared_ptr<PointAttributeSetter<MyPoint>> setter = std::make_shared<PointAttributeSetter<MyPoint>>();
        setter->registerAttribute("x", set_x<MyPoint>);
        setter->registerAttribute("y", set_y<MyPoint>);
        setter->registerAttribute("z", set_z<MyPoint>);
        setter->registerAttribute("red", [](MyPoint& pt, const PlyDataType& data) {
            pt.r = data.ucharVal;
        });
        setter->registerAttribute("r", [](MyPoint& pt, const PlyDataType& data) {
            pt.r = data.ucharVal;
        });
        setter->registerAttribute("green", [](MyPoint& pt, const PlyDataType& data) {
            pt.g = data.ucharVal;
        });
        setter->registerAttribute("g", [](MyPoint& pt, const PlyDataType& data) {
            pt.g = data.ucharVal;
        });
        setter->registerAttribute("blue", [](MyPoint& pt, const PlyDataType& data) {
            pt.b = data.ucharVal;
        });
        setter->registerAttribute("b", [](MyPoint& pt, const PlyDataType& data) {
            pt.b = data.ucharVal;
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
int main()
{
    std::string s = ply_write_trest();
    return ply_read_test(s);
}