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
    [[nodiscard]] explicit BytesDataInput(bool isLittleEndian = true);
    [[nodiscard]] explicit BytesDataInput(std::string_view buffer, bool copyBuffer = false, bool isLittleEndian = true);

    bool getBytes(void* target, size_t num);

    [[nodiscard]] std::string getString();

    [[nodiscard]] std::string getLongString();

    [[nodiscard]] float getFloat();

    [[nodiscard]] double getDouble();

    [[nodiscard]] uint8_t getByte();

    [[nodiscard]] int16_t getShort();

    [[nodiscard]] int getInt();

    [[nodiscard]] int64_t getInt64();
};

} // namespace bedrock_protocol