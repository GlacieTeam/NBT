#include <algorithm>
#include <nbt/io/BytesDataInput.hpp>

namespace bedrock_protocol {

BytesDataInput::BytesDataInput(bool isLittleEndian) {
    mOwnedBuffer    = std::string();
    mBufferView     = mOwnedBuffer;
    mReadPointer    = 0;
    mHasOverflowed  = false;
    mIsLittleEndian = isLittleEndian;
}

BytesDataInput::BytesDataInput(std::string_view buffer, bool copyBuffer, bool isLittleEndian) {
    if (copyBuffer) {
        mOwnedBuffer = buffer;
        mBufferView  = mOwnedBuffer;
    } else {
        mBufferView = buffer;
    }
    mReadPointer    = 0;
    mHasOverflowed  = false;
    mIsLittleEndian = isLittleEndian;
}

bool BytesDataInput::getBytes(void* target, size_t num) {
    if (mHasOverflowed) { return false; }
    if (num == 0) { return true; }

    size_t checkedNumber = num + mReadPointer;
    bool   hasOverflowed = checkedNumber < mReadPointer;

    if (checkedNumber >= mReadPointer && checkedNumber <= mBufferView.length()) {
        std::copy(
            mBufferView.begin() + mReadPointer,
            mBufferView.begin() + mReadPointer + num,
            static_cast<char*>(target)
        );
        mReadPointer += num;
        return true;
    } else {
        if (hasOverflowed || checkedNumber > mBufferView.length()) { mHasOverflowed = true; }
        return false;
    }
}

std::string BytesDataInput::getString() {
    std::string result;
    auto        size = getShort();
    result.resize(size);
    getBytes(result.data(), size);
    return result;
}

std::string BytesDataInput::getLongString() {
    std::string result;
    auto        size = getInt();
    result.resize(size);
    getBytes(result.data(), size);
    return result;
}

float BytesDataInput::getFloat() {
    float result;
    getBytes(&result, sizeof(float));
    if (!mIsLittleEndian) byteswap(result);
    return result;
}

double BytesDataInput::getDouble() {
    double result;
    getBytes(&result, sizeof(double));
    if (!mIsLittleEndian) byteswap(result);
    return result;
}

uint8_t BytesDataInput::getByte() {
    uint8_t result;
    getBytes(&result, sizeof(uint8_t));
    return result;
}

int16_t BytesDataInput::getShort() {
    int16_t result;
    getBytes(&result, sizeof(int16_t));
    if (!mIsLittleEndian) byteswap(result);
    return result;
}

int BytesDataInput::getInt() {
    int result;
    getBytes(&result, sizeof(int));
    if (!mIsLittleEndian) byteswap(result);
    return result;
}

int64_t BytesDataInput::getInt64() {
    int64_t result;
    getBytes(&result, sizeof(int64_t));
    if (!mIsLittleEndian) byteswap(result);
    return result;
}

} // namespace bedrock_protocol