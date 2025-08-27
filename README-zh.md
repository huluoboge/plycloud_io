
<div align="center">
  <a href="./README.md">English</a> | <b>中文</b>
</div>

# PlyCloud IO 库

## 概述

PlyCloud IO 是一个基于 C++ 模板的库，用于读写 PLY 格式的点云数据。支持二进制和 ASCII 格式，提供灵活的属性映射功能，并自动处理 PLY 文件头的生成和解析。该库设计简洁易用，采用宏注册系统和可自定义的属性映射。

## 主要特性

- **完整的 PLY 支持**: 支持二进制（紧凑快速）和 ASCII（人类可读）两种格式的读写
- **模板化架构**: 利用 C++ 模板自动生成针对不同点类型的代码
- **灵活的点类型**: 支持自定义点结构，包含多种属性类型（float、double、int8_t、uint8_t 等）
- **属性映射**: 支持将多个文件属性映射到单个对象属性（例如 "red" 和 "r" 都映射到 point.red）
- **宏注册系统**: 使用 `REGISTER_PLY_WRITE_POINT` 和 `REGISTER_PLY_READ_POINT` 宏简化点类型注册
- **自动头管理**: 动态生成和更新 PLY 文件头，确保顶点计数准确
- **自定义回调支持**: 支持注册自定义读取函数进行高级属性处理
- **流式 I/O**: 使用 `PlyPointStreamWriter` 和 `PlyPointStreamReader` 进行灵活的流操作

## 使用方法

### 1. 定义点类型

创建包含所需字段（位置、颜色、强度、法线等）的结构体：

```cpp
struct MyPoint {
    float x, y, z;
    float red, green, blue;
};
```

### 2. 注册读写点类型

使用提供的宏注册点类型及其属性：

```cpp
// 注册写入功能
REGISTER_PLY_WRITE_POINT(MyPoint, 
    (float, x, x)
    (float, y, y) 
    (float, z, z)
    (float, red, red)
    (float, green, green) 
    (float, blue, blue)
)

// 注册读取功能，支持属性映射
REGISTER_PLY_READ_POINT(MyPoint,
    (float, x, x)
    (float, y, y)
    (float, z, z)
    (float, red, red)    // 将 "red" 属性映射到 point.red
    (float, green, green) // 将 "green" 属性映射到 point.green  
    (float, blue, blue)   // 将 "blue" 属性映射到 point.blue
    (float, r, red)       // 同时将 "r" 属性映射到 point.red
    (float, g, green)     // 同时将 "g" 属性映射到 point.green
    (float, b, blue)      // 同时将 "b" 属性映射到 point.blue
)
```

### 3. 写入点到流

```cpp
#include "plycloud_writer.hpp"

MyPoint point;
point.x = 1.0f; point.y = 2.0f; point.z = 3.0f;
point.red = 255.0f; point.green = 128.0f; point.blue = 64.0f;

std::stringstream ss;
plyio::PlyPointStreamWriter<MyPoint> writer(ss);
writer.setBinary(true); // 使用二进制格式
writer.writeHead();
writer.writePoint(point);
writer.updateHead(); // 更新文件头中的实际点数
```

### 4. 从流读取点

```cpp
#include "plycloud_reader.hpp"

std::stringstream ss(plyData);
plyio::PlyPointStreamReader reader(ss);

if (reader.readHead()) {
    reader.printHeader();
    reader.beginReadPoint<MyPoint>();
    
    for (auto it = reader.begin<MyPoint>(); it != reader.end<MyPoint>(); ++it) {
        const MyPoint& pt = *it;
        std::cout << "点: (" << pt.x << ", " << pt.y << ", " << pt.z 
                  << "), 颜色: (" << pt.red << ", " << pt.green << ", " << pt.blue << ")" << std::endl;
    }
}
```

## 高级用法：自定义属性映射

对于更复杂的场景，可以使用自定义属性设置器：

```cpp
std::shared_ptr<plyio::PointAttributeSetter<MyPoint>> setter = 
    std::make_shared<plyio::PointAttributeSetter<MyPoint>>();

// 注册自定义属性处理器
setter->registerAttribute("red", [](MyPoint& pt, const plyio::PlyDataType& data) {
    pt.red = data.as<uint8_t>(); // 从 uint8_t 转换为 float
});
setter->registerAttribute("r", [](MyPoint& pt, const plyio::PlyDataType& data) {
    pt.red = data.as<uint8_t>();
});
// ... 注册其他属性

reader.prepareReadFunction(setter);
```

## 预定义点类型

库在 `plycloud_point.hpp` 中提供了几种常用点类型：

- `PlyPointXYZ`: 基本 3D 坐标
- `PlyPointXYZRGB`: 3D 坐标带 RGB 颜色
- `PlyPointXYZIRGB`: 3D 坐标带强度和 RGB 颜色  
- `PlyPointXYZN`: 3D 坐标带法线
- `PlyPointXYZIRGBN`: 完整点类型，包含坐标、强度、颜色和法线

## 依赖项

- **C++11** 或更高版本
- **Boost Preprocessor**（用于宏处理）- 可选但推荐

## 开始使用

1. 在项目中包含库头文件：
   ```cpp
   #include "plycloud_io.hpp"
   // 或单独包含：
   #include "plycloud_reader.hpp"
   #include "plycloud_writer.hpp"
   #include "plycloud_point.hpp"
   ```

2. 定义点类型并使用提供的宏进行注册

3. 使用 `PlyPointStreamWriter` 进行写入，`PlyPointStreamReader` 进行读取

4. 参考测试文件 [`plycloud_reader_write_test.cpp`](./src/plycloud_reader_write_test.cpp) 获取完整示例

## 构建

库使用 CMake 进行构建：

```bash
mkdir build && cd build
cmake ..
make
```

## 示例

完整示例请参见：
- [`plycloud_reader_write_test.cpp`](./src/plycloud_reader_write_test.cpp) - 完整的读写演示
- 预定义点类型在 [`plycloud_point.hpp`](./src/plycloud_point.hpp)

## 许可证

本项目开源，使用 [MIT 许可证](./LICENSE)。

## 贡献

欢迎贡献！请随时提交 pull request 或为 bug 和功能请求创建 issue。
