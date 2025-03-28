
#ifndef PLYCLOUD_IO_PLYCLOUD_IO_HPP
#define PLYCLOUD_IO_PLYCLOUD_IO_HPP

#include "plycloud_reader.hpp"
#include "plycloud_writer.hpp"

namespace plyio {

template <typename PointType>
struct PlyPointFileStreamAppend : public PlyPointStreamWriter<PointType> {
public:
    PlyPointFileStreamAppend(const std::string& file)
        : PlyPointStreamWriter<PointType>(fs)
    {
        fs.open(file, std::ios::out | std::ios::in);
    }

    void close()
    {
        fs.close();
    }

    bool readHead()
    {
        PlyPointStreamReader reader(fs);
        if (!reader.readHead()) {
            return false;
        }
        reader.preparePointAttribute<PointType>();
        PlyPointStreamWriter<PointType>::head.count = reader.head.count;
        PlyPointStreamWriter<PointType>::head.isBin = reader.head.isBin;
        PlyPointStreamWriter<PointType>::head.bodyPosition = reader.head.bodyPosition;
        return true;
    }

    void beginApend()
    {
        fs.seekp(0, fs.end);
    }

    std::fstream fs;
};
}
#endif // PLYCLOUD_IO_HPP