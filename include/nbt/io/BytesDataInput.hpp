#pragma once
#include <cstdint>
#include <iostream>

namespace bedrock_protocol {

template <typename T>
void byteswap(T& v) {
    union {
        T             value;
        unsigned char bytes[sizeof(T)];
    };
    value = v;
    std::reverse(bytes, bytes + sizeof(T));
    v = value;
}

class BytesDataOutput;

class BytesDataInput {
    friend class BytesDataOutput;

protected:
    size_t           mReadPointer;
    bool             mHasOverflowed;
    std::string      mOwnedBuffer;
    std::string_view mBufferView;
    bool             mIsLittleEndian;

public:
    explicit BytesDataInput(bool isLittleEndian = true);
    explicit BytesDataInput(std::string_view buffer, bool copyBuffer = false, bool isLittleEndian = true);

    bool getBytes(void* target, size_t num);

    std::string getString();

    std::string getLongString();

    float getFloat();

    double getDouble();

    uint8_t getByte();

    int16_t getShort();

    int getInt();

    int64_t getInt64();
};

} // namespace bedrock_protocol