#include <io/BytesDataInput.hpp>

namespace bedrock_protocol {

class BytesDataOutput : public BytesDataInput {
protected:
    std::string& mBuffer;

private:
    template <typename T>
    void write(T value);

public:
    explicit BytesDataOutput();
    explicit BytesDataOutput(std::string& buffer, bool copyBuffer = false);

    std::string getAndReleaseData();

    void writeBytes(const void* origin, size_t num);

    void writeString(std::string_view value);

    void writeLongString(std::string_view value);

    void writeFloat(float value);

    void writeDouble(double value);

    void writeByte(uint8_t value);

    void writeShort(int16_t value);

    void writeInt(int value);

    void writeInt64(int64_t value);
};

} // namespace bedrock_protocol