#include "plycloud_point.hpp"
#include "plycloud_writer.hpp"

#include <sstream>

int main()
{
    // Instantiate a point with coordinates (1, 2, 3)
    PlyPointXYZIRGBN X;
    X.x = 1;
    X.y = 2;
    X.z = 3;

    // Create a stringstream to hold the output
    std::stringstream ss;

    // Initialize a PlyPointStreamWriter to write points into the stringstream
    PlyPointStreamWriter<PlyPointXYZIRGBN> writer(ss);

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

    return 0;
}