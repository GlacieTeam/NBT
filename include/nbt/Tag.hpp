// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <binarystream/BinaryStream.hpp>
#include <memory>
#include <nbt/io/BytesDataOutput.hpp>

namespace bedrock_protocol {

class Tag {
public:
    enum class Type : uint8_t {
        End         = 0,
        Byte        = 1,
        Short       = 2,
        Int         = 3,
        Int64       = 4,
        Float       = 5,
        Double      = 6,
        ByteArray   = 7,
        String      = 8,
        List        = 9,
        Compound    = 10,
        IntArray    = 11,
        NumTagTypes = 12,
    };

    virtual ~Tag() = default;

    [[nodiscard]] virtual Type getType() const = 0;

    [[nodiscard]] virtual bool equals(Tag const& other) const = 0;

    [[nodiscard]] virtual std::unique_ptr<Tag> copy() const = 0;

    [[nodiscard]] virtual std::size_t hash() const = 0;

    virtual void write(BytesDataOutput& stream) const = 0;

    virtual void load(BytesDataInput& stream) = 0;

    virtual void write(BinaryStream& stream) const = 0;

    virtual void load(ReadOnlyBinaryStream& stream) = 0;

public:
    [[nodiscard]] static std::unique_ptr<Tag> newTag(Type type);

    [[nodiscard]] bool operator==(Tag const& other) const;

    template <std::derived_from<Tag> T>
    [[nodiscard]] T& as() {
        return *(T*)this;
    }

    template <std::derived_from<Tag> T>
    [[nodiscard]] T const& as() const {
        return *(T*)this;
    }
};

} // namespace bedrock_protocol
