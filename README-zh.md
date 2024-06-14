
<div align="center">
  <a href="./README.md">English</a> | <b>中文</b>
</div>

# PlyCloud IO 库

### 概览

PlyCloud IO 提供了一个基于C++模板的框架，用于将点云数据写入PLY文件，同时支持ASCII和二进制两种格式。它包含了定义具有多种属性（如位置、颜色、法线）的点类型、自动生成PLY头部信息以及将点数据流式传输到输出流的功能。

### 主要特性

- **模板特化**：利用C++模板自动为向PLY文件写入不同属性类型生成代码。
- **灵活的点类型**：支持定义带有浮点型、双精度型、int8_t等属性的自定义点结构体，并注册它们以便在生成PLY文件时自动处理。
- **宏基注册**：提供宏（如REGISTER_PLY_WRITE_POINT）简化新点类型及其属性的注册过程。
- **流式写入**：通过`PlyPointStreamWriter`和`PlyPointFileStreamWriter`类方便地将点云写入标准或文件流，效率高。
- **二进制与ASCII支持**：可在优化大小和速度的二进制格式与易于人类阅读的ASCII格式之间选择PLY输出。
- **头部管理**：动态管理PLY头部部分，在写入点时自动更新，确保记录了正确的顶点计数。

### 使用方法

1. **定义你的点类型**：创建一个继承自`PlyPointXYZ`（或直接定义属性）的结构体，并包含所需的字段（如位置、颜色、强度、法线）。
2. **注册点类型**：使用提供的宏来注册你的点类型及其属性。例如：
```cpp
   struct MyPointType {
       float x, y, z;
       uint8_t r, g, b;
   };
   REGISTER_PLY_WRITE_POINT(MyPointType, (float, x)(float, y)(float, z)(uint8_t, r)(uint8_t, g)(uint8_t, b))
```
3. **写入流**：使用你的点类型实例化一个PlyPointFileStreamWriter或PlyPointStreamWriter，开始向流写入点。
### 宏
- **REGISTER_PLY_WRITE_POINT**：注册点类型及其属性以写入PLY文件。
- **PLY_REGIST_WRITE_HEAD** 和 **PLY_REGIST_WRITE_POINT**：内部宏，用于注册头部和点写入逻辑。
- **PLY_WRITE_PROPERTY** 和 **PLY_WRITER_POINT**：辅助宏，分别用于写入属性定义和点。

### 类
- **PlyPropertyTraits**：模板类，用于将属性类型写入输出流。
- **PlyWriteHeadTraits** 和 **PlyWritePointTraits**：模板，定义如何写入特定类型的PLY头部和单个点。
- **PlyPointStreamWriter**：向任意输出流写入点。
- **PlyPointFileStreamWriter**：针对文件流的特殊写入器，具有额外的文件管理能力。

### 依赖项

- 需要Boost Preprocessor进行宏处理。





### 示例

```cpp
#include "plycloud_io.hpp"

int main() {
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
```
完整的示例，请参考提供的结构体定义，如PlyPointXYZRGB，以及它们是如何在库源代码中注册和使用的示例。根据应用需求进行定制或扩展。