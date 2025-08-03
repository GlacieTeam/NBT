// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/io/BytesDataInput.hpp>

namespace bedrock_protocol {

class BytesDataOutput : public BytesDataInput {
protected:
    std::string& mBuffer;

public:
    [[nodiscard]] explicit BytesDataOutput(bool isLittleEndian = true);
    [[nodiscard]] explicit BytesDataOutput(std::string& buffer, bool copyBuffer = false, bool isLittleEndian = true);

    [[nodiscard]] std::string getAndReleaseData();

    void writeBytes(const void* origin, size_t num);

    void writeString(std::string_view  value);

    void writeLongString(std::string_view  value);

    void writeFloat(float value);

    void writeDouble(double value);

    void writeByte(uint8_t value);

    void writeShort(int16_t value);

    void writeInt(int value);

    void writeInt64(int64_t value);
};

} // namespace bedrock_protocol