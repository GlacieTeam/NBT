// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <binarystream/BinaryStream.hpp>
#include <memory>
#include <nbt/SnbtFormat.hpp>
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
    [[nodiscard]] bool operator==(Tag const& other) const;

    [[nodiscard]] std::string toSnbt(SnbtFormat snbtFormat = SnbtFormat::Jsonify, uint8_t indent = 4) const noexcept;

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr T& as() noexcept {
        return static_cast<T&>(*this);
    }

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr T const& as() const noexcept {
        return static_cast<T const&>(*this);
    }

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr operator T&() noexcept {
        return as<T&>();
    }

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr operator T const&() const noexcept {
        return as<T const&>();
    }

public:
    [[nodiscard]] static std::unique_ptr<Tag> newTag(Type type);
};

} // namespace bedrock_protocol
