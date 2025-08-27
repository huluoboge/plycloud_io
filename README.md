<div align="center">
  <b>English</b> | <a href="./README-zh.md">中文</a>
</div>

# PlyCloud IO Library

## Overview

PlyCloud IO is a C++ template-based library for reading and writing point cloud data in PLY format. It supports both binary and ASCII formats, provides flexible attribute mapping, and automatically handles PLY header generation and parsing. The library is designed for ease of use with macro-based registration systems and customizable property mapping.

## Key Features

- **Complete PLY Support**: Read and write PLY files in both binary (compact and fast) and ASCII (human-readable) formats
- **Template-Based Architecture**: Leverages C++ templates for automatic code generation tailored to different point types
- **Flexible Point Types**: Support custom point structures with various property types (float, double, int8_t, uint8_t, etc.)
- **Attribute Mapping**: Map multiple file attributes to a single object property (e.g., "red" and "r" both map to point.red)
- **Macro-Based Registration**: Streamline point type registration with `REGISTER_PLY_WRITE_POINT` and `REGISTER_PLY_READ_POINT` macros
- **Automatic Header Management**: Dynamically generates and updates PLY headers with accurate vertex counts
- **Custom Callback Support**: Register custom read functions for advanced attribute processing
- **Stream-Based I/O**: Use `PlyPointStreamWriter` and `PlyPointStreamReader` for flexible stream operations

## Usage

### 1. Define Your Point Type

Create a structure with the desired fields (position, color, intensity, normals, etc.):

```cpp
struct MyPoint {
    float x, y, z;
    float red, green, blue;
};
```

### 2. Register Point Types for Reading and Writing

Use the provided macros to register your point type with its attributes:

```cpp
// Register for writing
REGISTER_PLY_WRITE_POINT(MyPoint, 
    (float, x, x)
    (float, y, y) 
    (float, z, z)
    (float, red, red)
    (float, green, green) 
    (float, blue, blue)
)

// Register for reading with attribute mapping
REGISTER_PLY_READ_POINT(MyPoint,
    (float, x, x)
    (float, y, y)
    (float, z, z)
    (float, red, red)    // Maps "red" attribute to point.red
    (float, green, green) // Maps "green" attribute to point.green  
    (float, blue, blue)   // Maps "blue" attribute to point.blue
    (float, r, red)       // Also maps "r" attribute to point.red
    (float, g, green)     // Also maps "g" attribute to point.green
    (float, b, blue)      // Also maps "b" attribute to point.blue
)
```

### 3. Write Points to Stream

```cpp
#include "plycloud_writer.hpp"

MyPoint point;
point.x = 1.0f; point.y = 2.0f; point.z = 3.0f;
point.red = 255.0f; point.green = 128.0f; point.blue = 64.0f;

std::stringstream ss;
plyio::PlyPointStreamWriter<MyPoint> writer(ss);
writer.setBinary(true); // Use binary format
writer.writeHead();
writer.writePoint(point);
writer.updateHead(); // Update header with actual point count
```

### 4. Read Points from Stream

```cpp
#include "plycloud_reader.hpp"

std::stringstream ss(plyData);
plyio::PlyPointStreamReader reader(ss);

if (reader.readHead()) {
    reader.printHeader();
    reader.beginReadPoint<MyPoint>();
    
    for (auto it = reader.begin<MyPoint>(); it != reader.end<MyPoint>(); ++it) {
        const MyPoint& pt = *it;
        std::cout << "Point: (" << pt.x << ", " << pt.y << ", " << pt.z 
                  << "), Color: (" << pt.red << ", " << pt.green << ", " << pt.blue << ")" << std::endl;
    }
}
```

## Advanced Usage: Custom Attribute Mapping

For more complex scenarios, you can use custom attribute setters:

```cpp
std::shared_ptr<plyio::PointAttributeSetter<MyPoint>> setter = 
    std::make_shared<plyio::PointAttributeSetter<MyPoint>>();

// Register custom attribute handlers
setter->registerAttribute("red", [](MyPoint& pt, const plyio::PlyDataType& data) {
    pt.red = data.as<uint8_t>(); // Convert from uint8_t to float
});
setter->registerAttribute("r", [](MyPoint& pt, const plyio::PlyDataType& data) {
    pt.red = data.as<uint8_t>();
});
// ... register other attributes

reader.prepareReadFunction(setter);
```

## Predefined Point Types

The library provides several commonly used point types in `plycloud_point.hpp`:

- `PlyPointXYZ`: Basic 3D coordinates
- `PlyPointXYZRGB`: 3D coordinates with RGB color
- `PlyPointXYZIRGB`: 3D coordinates with intensity and RGB color  
- `PlyPointXYZN`: 3D coordinates with normals
- `PlyPointXYZIRGBN`: Complete point with coordinates, intensity, color, and normals

## Dependencies

- **C++11** or later
- **Boost Preprocessor** (for macro processing) - optional but recommended

## Getting Started

1. Include the library headers in your project:
   ```cpp
   #include "plycloud_io.hpp"
   // or individual headers:
   #include "plycloud_reader.hpp"
   #include "plycloud_writer.hpp"
   #include "plycloud_point.hpp"
   ```

2. Define your point types and register them using the provided macros

3. Use `PlyPointStreamWriter` for writing and `PlyPointStreamReader` for reading

4. Refer to the test file [`plycloud_reader_write_test.cpp`](./src/plycloud_reader_write_test.cpp) for complete examples

## Building

The library uses CMake for building:

```bash
mkdir build && cd build
cmake ..
make
```

## Examples

For comprehensive examples, see:
- [`plycloud_reader_write_test.cpp`](./src/plycloud_reader_write_test.cpp) - Complete read/write demonstration
- Predefined point types in [`plycloud_point.hpp`](./src/plycloud_point.hpp)

## License

This project is open source and available under the [MIT License](./LICENSE).

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.
