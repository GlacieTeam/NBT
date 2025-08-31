// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/io/BytesDataInput.hpp"
#include <algorithm>

namespace nbt {

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

bool BytesDataInput::hasDataLeft() const noexcept { return mReadPointer < mBufferView.size(); }

bool BytesDataInput::getBytes(void* target, size_t num) noexcept {
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

void BytesDataInput::getString(std::string& result) {
    auto size = getShort();
    result.resize(size);
    getBytes(result.data(), size);
}

std::string BytesDataInput::getString() {
    std::string result;
    getString(result);
    return result;
}

void BytesDataInput::getLongString(std::string& result) {
    auto size = getInt();
    result.resize(size);
    getBytes(result.data(), size);
}

std::string BytesDataInput::getLongString() {
    std::string result;
    getLongString(result);
    return result;
}

float BytesDataInput::getFloat() noexcept {
    float result = 0;
    getBytes(&result, sizeof(float));
    if (!mIsLittleEndian) { result = bstream::detail::swapEndian(result); }
    return result;
}

double BytesDataInput::getDouble() noexcept {
    double result = 0;
    getBytes(&result, sizeof(double));
    if (!mIsLittleEndian) { result = bstream::detail::swapEndian(result); }
    return result;
}

uint8_t BytesDataInput::getByte() noexcept {
    uint8_t result = 0;
    getBytes(&result, sizeof(uint8_t));
    return result;
}

int16_t BytesDataInput::getShort() noexcept {
    int16_t result = 0;
    getBytes(&result, sizeof(int16_t));
    if (!mIsLittleEndian) { result = bstream::detail::swapEndian(result); }
    return result;
}

int BytesDataInput::getInt() noexcept {
    int result = 0;
    getBytes(&result, sizeof(int));
    if (!mIsLittleEndian) { result = bstream::detail::swapEndian(result); }
    return result;
}

int64_t BytesDataInput::getInt64() noexcept {
    int64_t result = 0;
    getBytes(&result, sizeof(int64_t));
    if (!mIsLittleEndian) { result = bstream::detail::swapEndian(result); }
    return result;
}

} // namespace nbt