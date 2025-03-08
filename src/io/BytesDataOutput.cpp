#include <io/BytesDataOutput.hpp>

namespace bedrock_protocol {

BytesDataOutput::BytesDataOutput() : BytesDataInput({}, true), mBuffer(mOwnedBuffer) {}

BytesDataOutput::BytesDataOutput(std::string& buffer, bool copyBuffer)
: BytesDataInput(buffer, copyBuffer),
  mBuffer(buffer) {}

template <typename T>
void BytesDataOutput::write(T value) {
    mBuffer.append((const char*)(&value), sizeof(T));
}

std::string BytesDataOutput::getAndReleaseData() { return std::move(mBuffer); }

void BytesDataOutput::writeBytes(const void* origin, size_t num) { mBuffer.append((const char*)origin, num); }

void BytesDataOutput::writeString(std::string_view value) {
    writeShort((int16_t)value.size());
    writeBytes(value.data(), value.size());
}

void BytesDataOutput::writeLongString(std::string_view value) {
    writeInt((int)value.size());
    writeBytes(value.data(), value.size());
}

void BytesDataOutput::writeFloat(float value) { writeBytes(&value, sizeof(float)); }

void BytesDataOutput::writeDouble(double value) { writeBytes(&value, sizeof(double)); }

void BytesDataOutput::writeByte(uint8_t value) { writeBytes(&value, sizeof(uint8_t)); }

void BytesDataOutput::writeShort(int16_t value) { writeBytes(&value, sizeof(int16_t)); }

void BytesDataOutput::writeInt(int value) { writeBytes(&value, sizeof(int)); }

void BytesDataOutput::writeInt64(int64_t value) { writeBytes(&value, sizeof(int64_t)); }

} // namespace bedrock_protocol
