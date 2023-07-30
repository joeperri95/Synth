#pragma once

#include <map>
#include <string>
#include <variant>

typedef std::variant<bool, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double, std::string> AudioParam;

typedef enum {
    TYPE_NOTHING,
    TYPE_BOOL,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,
    TYPE_INT8,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_STR
} AudioParameterType;

class AudioParameter {
public:
    AudioParameter();
    AudioParameter(const AudioParameter &other);
    ~AudioParameter();

    AudioParameter& operator=(const AudioParameter &other);
    AudioParameterType getType();

    void    setParamNothing();
    float    getParamFloat();
    void    setParamFloat(float data);
    double   getParamDouble();
    void   setParamDouble(double data);
    bool     getParamBool();
    void     setParamBool(bool data);
    uint8_t  getParamU8();
    void  setParamU8(uint8_t data);
    uint16_t getParamU16();
    void setParamU16(uint16_t data);
    uint32_t getParamU32();
    void setParamU32(uint32_t data);
    uint64_t getParamU64();
    void setParamU64(uint64_t data);
    int8_t   getParamI8();
    void   setParamI8(int8_t data);
    int16_t  getParamI16();
    void  setParamI16(int16_t data);
    int32_t  getParamI32();
    void  setParamI32(int32_t data);
    int64_t  getParamI64();
    void  setParamI64(int64_t data);
    std::string  getParamString();
    void  setParamString(std::string data);

private:
    AudioParameterType type;
    AudioParam data;
};

