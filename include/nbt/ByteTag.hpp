// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class ByteTag : public Tag {
protected:
    uint8_t mData;

public:
    [[nodiscard]] ByteTag() = default;
    [[nodiscard]] ByteTag(uint8_t data);

    template <std::integral T>
    constexpr ByteTag& operator=(T value) {
        mData = (uint8_t)value;
        return *this;
    }

    template <std::integral T>
    [[nodiscard]] constexpr operator T() const {
        return (T)mData;
    }

    [[nodiscard]] constexpr operator std::byte() const { return (std::byte)mData; }

    template <std::integral T>
    [[nodiscard]] constexpr explicit ByteTag(T value = 0) : mData((uint8_t)value) {}

    [[nodiscard]] constexpr explicit ByteTag(std::byte b) : mData(std::to_integer<uint8_t>(b)) {}

    [[nodiscard]] ByteTag operator-() const { return ByteTag{-mData}; }

    [[nodiscard]] Type getType() const override;

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    [[nodiscard]] uint8_t&       data();
    [[nodiscard]] uint8_t const& data() const;
};

} // namespace bedrock_protocol
