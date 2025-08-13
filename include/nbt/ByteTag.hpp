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
    uint8_t mStorage{0};

public:
    [[nodiscard]] constexpr ByteTag() = default;

    template <std::integral T>
    [[nodiscard]] constexpr explicit ByteTag(T value) : mStorage(static_cast<uint8_t>(value)) {}

    [[nodiscard]] constexpr explicit ByteTag(std::byte byte) : mStorage(std::to_integer<uint8_t>(byte)) {}

    template <std::integral T>
    constexpr ByteTag& operator=(T value) {
        mStorage = static_cast<uint8_t>(value);
        return *this;
    }

    template <std::integral T>
    [[nodiscard]] constexpr operator T() const {
        return static_cast<T>(mStorage);
    }

    [[nodiscard]] constexpr operator std::byte() const { return static_cast<std::byte>(mStorage); }

    [[nodiscard]] ByteTag operator+() const { return ByteTag{+mStorage}; }

    [[nodiscard]] constexpr Type getType() const override { return Type::Byte; }

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    [[nodiscard]] constexpr uint8_t&       storage() { return mStorage; }
    [[nodiscard]] constexpr uint8_t const& storage() const { return mStorage; }
};

} // namespace bedrock_protocol
