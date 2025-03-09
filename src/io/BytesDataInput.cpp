#include <io/BytesDataInput.hpp>

namespace bedrock_protocol {

BytesDataInput::BytesDataInput() {
    mOwnedBuffer   = std::string();
    mBufferView    = mOwnedBuffer;
    mReadPointer   = 0;
    mHasOverflowed = false;
}

BytesDataInput::BytesDataInput(std::string_view buffer, bool copyBuffer) {
    if (copyBuffer) {
        mOwnedBuffer = buffer;
        mBufferView  = mOwnedBuffer;
    } else {
        mBufferView = buffer;
    }
    mReadPointer   = 0;
    mHasOverflowed = false;
}

template <typename T>
T BytesDataInput::swapEndian(T u) {
    union {
        T             u;
        unsigned char u8[sizeof(T)];
    } source, dest;
    source.u = u;
    for (size_t k = 0; k < sizeof(T); k++) dest.u8[k] = source.u8[sizeof(T) - k - 1];
    return dest.u;
}

template <typename T>
bool BytesDataInput::read(T* target) {
    if (mHasOverflowed) { return false; }
    size_t newReadPointer = mReadPointer + sizeof(T);
    if (newReadPointer < mReadPointer || newReadPointer > mBuffer.length()) {
        mHasOverflowed = true;
        return false;
    }
    std::copy(mBuffer.begin() + mReadPointer, mBuffer.begin() + newReadPointer, reinterpret_cast<char*>(target));
    mReadPointer = newReadPointer;
    return true;
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
    return result;
}

double BytesDataInput::getDouble() {
    double result;
    getBytes(&result, sizeof(double));
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
    return result;
}

int BytesDataInput::getInt() {
    int result;
    getBytes(&result, sizeof(int));
    return result;
}

int64_t BytesDataInput::getInt64() {
    int64_t result;
    getBytes(&result, sizeof(int64_t));
    return result;
}

} // namespace bedrock_protocol