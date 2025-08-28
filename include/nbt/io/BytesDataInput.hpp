// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <binarystream/ReadOnlyBinaryStream.hpp>

namespace bedrock_protocol {

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
    [[nodiscard]] NBT_API explicit BytesDataInput(bool isLittleEndian = true);
    [[nodiscard]] NBT_API explicit BytesDataInput(
        std::string_view buffer,
        bool             copyBuffer     = false,
        bool             isLittleEndian = true
    );

    NBT_API bool getBytes(void* target, size_t num);

    [[nodiscard]] NBT_API std::string getString();

    [[nodiscard]] NBT_API std::string getLongString();

    [[nodiscard]] NBT_API float getFloat();

    [[nodiscard]] NBT_API double getDouble();

    [[nodiscard]] NBT_API uint8_t getByte();

    [[nodiscard]] NBT_API int16_t getShort();

    [[nodiscard]] NBT_API int getInt();

    [[nodiscard]] NBT_API int64_t getInt64();
};

} // namespace bedrock_protocol