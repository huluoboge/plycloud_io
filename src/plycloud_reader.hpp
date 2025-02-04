#ifndef PLYCLOUD_IO_PLYCLOUD_READER_HPP
#define PLYCLOUD_IO_PLYCLOUD_READER_HPP

#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

enum PlyTypeEnum {
    type_char = 1,
    type_uchar = 2,
    type_short = 3,
    type_ushort = 4,
    type_int = 5,
    type_uint = 6,
    type_float = 7,
    type_double = 8
};
union PlyDataType {
    int8_t charVal;
    uint8_t ucharVal;
    int16_t shortVal;
    uint16_t ushortVal;
    int32_t intVal;
    uint32_t uintVal;
    float floatVal;
    double doubleVal;
};

// template <typename PointType>
// struct NameMapTraits {
//     static void setNameMap(std::unordered_map<std::string, int>& nameMap)
//     {
//     }
// };

namespace inner {
class splitstring : public std::string {
    std::vector<std::string> flds;

public:
    splitstring(const char* s)
        : std::string(s) { };
    std::vector<std::string>& split(char delim, int rep = 0);
};

inline std::vector<std::string>& splitstring::split(char delim, int rep)
{
    if (!flds.empty())
        flds.clear(); // empty vector if necessary
    if (empty()) {
        return flds;
    }
    std::stringstream ss(data());
    std::string token;
    while (std::getline(ss, token, delim)) {
        if (rep == 1 || !token.empty()) {
            flds.push_back(token);
        }
    }
    return flds;
}

inline std::string& trim(std::string& s)
{
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}
}

struct PlyHeader {
    // std::unordered_map<std::string, int> nameMap;
    // std::unordered_map<std::string, int> attributeNameToIndexMap;
    PlyHeader()
    {
    }
    struct Attri {
        std::string name;
        std::string type;

        bool parseType()
        {
            typeFlags = 0;
            if (type == "char") {
                typeFlags = type_char;
            } else if (type == "uchar") {
                typeFlags = type_uchar;
            } else if (type == "short") {
                typeFlags = type_short;
            } else if (type == "ushort") {
                typeFlags = type_ushort;
            } else if (type == "int") {
                typeFlags = type_int;
            } else if (type == "uint") {
                typeFlags = type_uint;
            } else if (type == "float") {
                typeFlags = type_float;
            } else if (type == "double") {
                typeFlags = type_double;
            } else {
                return false;
            }
            return true;
        }
        // bool parseName(PlyHeader& header)
        // {
        //     // nameFlags = name_unknown;
        //     // auto itr = header.nameMap.find(name);
        //     // if (itr != header.nameMap.end()) {
        //     //     nameFlags = itr->second;
        //     //     return true;
        //     // } else {
        //     //     return false;
        //     // }
        // }
        bool read(std::istream& is, bool isBin, PlyDataType& data) const
        {
            if (isBin) {
                if (typeFlags == type_char) {
                    is.read((char*)&data.charVal, 1);
                } else if (typeFlags == type_uchar) {
                    is.read((char*)&data.ucharVal, 1);
                } else if (typeFlags == type_short) {
                    is.read((char*)&data.shortVal, 2);
                } else if (typeFlags == type_ushort) {
                    is.read((char*)&data.ushortVal, 2);
                } else if (typeFlags == type_int) {
                    is.read((char*)&data.intVal, 4);
                } else if (typeFlags == type_uint) {
                    is.read((char*)&data.uintVal, 4);
                } else if (typeFlags == type_float) {
                    is.read((char*)&data.floatVal, 4);
                } else if (typeFlags == type_double) {
                    is.read((char*)&data.doubleVal, 8);
                } else {
                    std::cout << "logic error" << std::endl;
                    return false;
                }
            } else {
                int iVal = 0;
                if (typeFlags == type_char) {
                    is >> iVal;
                    data.charVal = iVal;
                } else if (typeFlags == type_uchar) {
                    is >> iVal;
                    data.ucharVal = iVal;
                } else if (typeFlags == type_short) {
                    is >> iVal;
                    data.shortVal = iVal;
                } else if (typeFlags == type_ushort) {
                    is >> iVal;
                    data.ushortVal = iVal;
                } else if (typeFlags == type_int) {
                    is >> data.intVal;
                } else if (typeFlags == type_uint) {
                    is >> data.uintVal;
                } else if (typeFlags == type_float) {
                    is >> data.floatVal;
                } else if (typeFlags == type_double) {
                    is >> data.doubleVal;
                } else {
                    std::cout << "logic error" << std::endl;
                    return false;
                }
            }
            return true;
        }

        // private:
        // int nameFlags;
        int typeFlags;
    };

    std::vector<Attri> attributes;
    bool isBin = false;

    // 点云数量
    size_t count = 0;
    // 错误信息
    std::string errorInfo;

    // ply body的地址偏移，用来越过header，直接读取点云
    size_t bodyPosition = 0;
    void clear()
    {
        attributes.clear();
        isBin = false;
        count = 0;
        errorInfo.clear();
        bodyPosition = 0;
    }
};

static bool PlyReadHead(std::istream& is, PlyHeader& header)
{
    is.clear();
    is.seekg(0, is.beg);
    header.clear();
    std::string s;
    bool plyInit = false;
    bool startVertex = false;
    while (std::getline(is, s)) {
        if (s.empty()) {
            continue;
        }
        inner::trim(s);
        if (s.empty()) {
            continue;
        }
        if (s == "end_header") {
            break;
        }
        if (!plyInit) {
            if (s == "ply") {
                plyInit = true;
            }
        } else {
            inner::splitstring ss(s.c_str());
            std::vector<std::string>& datas = ss.split(' ');
            if (datas.size() >= 3) {
                if (datas[0] == "format") {
                    if (datas[1] == "ascii") {
                        header.isBin = false;
                    } else if (datas[1] == "binary_little_endian") {
                        header.isBin = true;
                    } else if (datas[1] == "binary_big_endian") {
                        header.isBin = true;
                        header.errorInfo = "not support binary_big_endian for read";
                        return false;
                    }
                } else if (datas[0] == "element") {
                    if (datas[1] == "vertex") {
                        header.count = std::stoll(datas[2]);
                        startVertex = true;
                    } else {
                        startVertex = false;
                    }
                } else if (datas[0] == "property" && startVertex) {
                    PlyHeader::Attri att;
                    att.type = datas[1];
                    att.name = datas[2];
                    if (!att.parseType()) {
                        header.errorInfo = std::string("can't parse file type=") + att.type;
                        std::cout << header.errorInfo << std::endl;
                    }
                    header.attributes.emplace_back(att);
                    // header.attributeNameToIndexMap[att.name] = header.attributes.size() - 1;
                }
            }
        }
    }
    if (!plyInit) {
        header.errorInfo = "is not ply format";
        return false;
    }
    header.bodyPosition = is.tellg();
    return true;
}

class BasePointAttributeSetter {
public:
    virtual void applyAttributes(const PlyHeader& header) = 0;
    template <typename PonitT>
    struct DefaultSetter {
        void operator()(PonitT& pt, const PlyDataType& data)
        {
        }
    };
};

template <typename PointType>
class PointAttributeSetter : public BasePointAttributeSetter {
public:
    using SetterFunc = std::function<void(PointType&, const PlyDataType&)>;

    void registerAttribute(const std::string& name, SetterFunc setter)
    {
        attributeSetterMap[name] = setter;
    }

    std::vector<SetterFunc> attributeSetters;

    virtual void applyAttributes(const PlyHeader& header) override
    {
        attributeSetters.resize(header.attributes.size(), BasePointAttributeSetter::DefaultSetter<PointType>());
        for (int i = 0; i < header.attributes.size(); ++i) {
            auto itr = attributeSetterMap.find(header.attributes[i].name);
            if (itr != attributeSetterMap.end()) {
                attributeSetters[i] = itr->second;
            }
        }
    }

private:
    std::map<std::string, SetterFunc> attributeSetterMap;
};

template <typename PointType>
bool PlyReadPoint(std::istream& is, const PlyHeader& header,
    PointType& pt, const PointAttributeSetter<PointType>& setter)
{
    PlyDataType data;
    for (int i = 0; i < header.attributes.size(); ++i) {
        if (header.attributes[i].read(is, header.isBin, data)) {
            setter.attributeSetters[i](pt, data);
        }
    }
    return !is.fail();
}

template <typename PointType>
class PlyPointFileStreamReaderIterator;

struct PlyPointStreamReader {
    PlyPointStreamReader(std::istream& _is)
        : is(_is)
    {
    }

    bool readHead()
    {
        return PlyReadHead(is, head);
    }

    void printHeader() const
    {
        std::string msg = head.isBin ? "is banary" : "is ascii";
        std::cout << msg << std::endl;
        std::cout << "count = " << head.count << std::endl;
        for (int i = 0; i < head.attributes.size(); ++i) {
            std::cout << head.attributes[i].name << "," << head.attributes[i].type << std::endl;
        }
    }

    void beginReadPoint()
    {
        is.clear();
        is.seekg(head.bodyPosition, is.beg);
    }

    size_t pointSize() const
    {
        return head.count;
    }

    template <typename PointType>
    void prepareReadFunction(PointAttributeSetter<PointType>* setter)
    {
        setter->applyAttributes(head);
        baseSetter = setter;
    }
    template <typename PointType>
    bool readPoint(PointType& pt)
    {
        return PlyReadPoint(is, head, pt, *static_cast<PointAttributeSetter<PointType>*>(baseSetter));
    }

    template <typename PointType>
    PlyPointFileStreamReaderIterator<PointType> begin();

    template <typename PointType>
    PlyPointFileStreamReaderIterator<PointType> end();

    PlyHeader head;
    std::istream& is;
    BasePointAttributeSetter* baseSetter;
};

// TODO add stream state
template <typename PointType>
class PlyPointFileStreamReaderIterator : public std::iterator<std::input_iterator_tag, PointType> {
    PlyPointStreamReader* reader;
    size_t id = 0;
    size_t count = 0;
    PointType pt;

public:
    using value_type = PointType;
    PlyPointFileStreamReaderIterator(PlyPointStreamReader* rd, size_t _id, size_t _count)
        : reader(rd)
        , id(_id)
        , count(_count)
    {
        if (id == 0 && id < count) {
            reader->beginReadPoint();
            reader->readPoint(pt);
        }
    }

    PlyPointFileStreamReaderIterator(const PlyPointFileStreamReaderIterator& mit)
        : reader(mit.reader)
        , id(mit.id)
        , count(mit.count)
    {
        if (id == 0 && id < count) {
            reader->beginReadPoint();
            reader->readPoint(pt);
        }
    }

    PlyPointFileStreamReaderIterator& operator=(const PlyPointFileStreamReaderIterator& mit)
    {
        reader = mit.reader;
        id = mit.id;
        count = mit.count;
        // TODO force stream position == id
        if (id == 0 && id < count) {
            reader->beginReadPoint();
            reader->readPoint(pt);
        }
        return *this;
    }

    PlyPointFileStreamReaderIterator& operator++()
    {
        ++id;
        if (id < count) {
            reader->readPoint(pt);
        }
        return *this;
    }
    PlyPointFileStreamReaderIterator operator++(int)
    {
        PlyPointFileStreamReaderIterator tmp(*this);
        operator++();
        return tmp;
    }
    bool operator==(const PlyPointFileStreamReaderIterator& rhs) const { return (reader == rhs.reader && id == rhs.id); }
    bool operator!=(const PlyPointFileStreamReaderIterator& rhs) const { return !(operator==(rhs)); }
    PointType& operator*()
    {
        return pt;
    }

    PointType* operator->()
    {
        return &pt;
    }
    const PointType* operator->() const
    {
        return &pt;
    }
};

template <typename PointType>
PlyPointFileStreamReaderIterator<PointType> PlyPointStreamReader::begin()
{
    // preparePointAttribute<PointType>();
    readHead();
    beginReadPoint();
    PlyPointFileStreamReaderIterator<PointType> itr(this, 0, pointSize());
    return itr;
}

template <typename PointType>
PlyPointFileStreamReaderIterator<PointType> PlyPointStreamReader::end()
{
    return PlyPointFileStreamReaderIterator<PointType>(this, pointSize(), pointSize());
}

class PlyPointFileStreamReader : public PlyPointStreamReader {
public:
    PlyPointFileStreamReader(const std::string& file)
        : PlyPointStreamReader(ifs)
    {
        ifs.open(file, std::ios::in | std::ios::binary);
        if (!ifs) {
            std::cout << "[ERROR]can't open file " << file << std::endl;
        }
        plyFile = file;
    }
    ~PlyPointFileStreamReader()
    {
        close();
    }

    std::string file() const
    {
        return plyFile;
    }

    bool isOpen() const
    {
        return ifs.is_open();
    }

    void close()
    {
        ifs.close();
    }

    PlyPointFileStreamReader(const PlyPointFileStreamReader&) = delete;
    PlyPointFileStreamReader& operator=(const PlyPointFileStreamReader&) = delete;

protected:
    std::string plyFile;
    std::ifstream ifs;
};

#endif // PLYCLOUD_IO_PLYCLOUD_READER_HPP