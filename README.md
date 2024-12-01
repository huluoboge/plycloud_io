<div align="center">
  <b>English</b> | <a href="./README-zh.md">中文</a>
</div>

# PlyCloud IO Library Readme

## Overview

The PlyCloud IO library furnishes a C++ template-based framework for exporting point cloud data to PLY files, compatible with both ASCII and binary formats. It encompasses functionalities to delineate point types replete with diverse attributes (e.g., position, color, normal), spontaneously generate PLY headers, and transmit point datasets to output streams.

## Key Features

- **Template Specialization**: Leverages C++ templates to autonomously generate code tailored for inscribing varied property types into PLY files.
- **Flexible Point Types**: Backs the definition of bespoke point structures imbued with properties akin to `float`, `double`, `int8_t`, etc., and enrolls them for automated management in the course of PLY file genesis.
- **Macro-Based Registration**: Proffers macros (`REGISTER_PLY_WRITE_POINT`) to streamline the enrollment of nascent point types alongside their attributes.
- **Stream Writing**: Employs `PlyPointStreamWriter` and `PlyPointFileStreamWriter` classes to expedite the transcription of point clouds to either conventional or file-based streams efficiently.
- **Binary & ASCII Support**: Facilitates the election between binary (streamlined for compactness and velocity) and ASCII (human-legible) formats for PLY yield.
- **Header Management**: Dynamically curates the PLY header segment, refreshing it in consonance with point inscription, thereby ascertaining the precise enumeration of vertices is chronicled.

## Usage

### Define Your Point Type
Craft a structure inheriting from `PlyPointXYZ` (or explicates properties outright) encompassing the yearned fields (e.g., position, color, intensity, normals).

### Register Point Type
Employ the dispensed macros to enroll your point prototype along with its characteristics. A model:
```cpp
struct MyPointType {
    float x, y, z;
    uint8_t r, g, b;
};
REGISTER_PLY_WRITE_POINT(MyPointType, (float, x, x)(float, y, y)(float, z, z)(uint8_t, r, r)(uint8_t, g, g)(uint8_t, b, b))

```
## Write to Stream
Instantiate a `PlyPointFileStreamWriter` or `PlyPointStreamWriter`, utilizing your point archetype, and commence penning points to the conduit.

### Macros
- `REGISTER_PLY_WRITE_POINT`: Registers a point prototype inclusive of its attributes for PLY documentation.
- `PLY_REGIST_WRITE_HEAD` and `PLY_REGIST_WRITE_POINT`: Intrinsic macros for enrolling header and point inscription rationale.
- `PLY_WRITE_PROPERTY` and `PLY_WRITER_POINT`: Auxiliary macros facilitating property delineations and point inscriptions individually.

### Classes
- `PlyPropertyTraits`: A template class for scripting property categories to an emission conduit.
- `PlyWriteHeadTraits` and `PlyWritePointTraits`: Templates delineating the methodology of scripting the PLY header and discrete points of a specified category.
- `PlyPointStreamWriter`: Scripts points to any output conduit.
- `PlyPointFileStreamWriter`: A specialized scribe for file conduits, augmented with supplementary file governance capabilities.

### Dependencies
Requires Boost Preprocessor for macro processing.

## Getting Started
Incorporate `PLYCLOUD_IO_HPP` into your project and adhere to the directives outlined in the Usage section to embark on documenting point clouds as PLY files.

## Customization
The library is architectured for facile extension to accommodate custom point prototypes and supplementary attribute administration. Simply delineate your proprietary point structures and harness the registration macros accordingly.

## Example
For a holistic illustration, refer to the dispensed structure definitions, such as `PlyPointXYZRGB`, and their enrollment and utilization within the library's source code.

## Note
The code excerpt provided encompasses a comprehensive anthology of predefined point prototypes (e.g., `PlyPointXYZRGB`, `PlyPointXYZN`) with exemplars of their registration and usage. Tailor or augment these in accordance with your application's requirements.