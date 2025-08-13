// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class Int64Tag : public Tag {
protected:
    int64_t mStorage{0};

public:
    [[nodiscard]] constexpr Int64Tag() = default;

    template <std::integral T>
    [[nodiscard]] constexpr explicit Int64Tag(T value) noexcept : mStorage(static_cast<int64_t>(value)) {}

    template <std::integral T>
    constexpr Int64Tag& operator=(int64_t value) noexcept {
        mStorage = static_cast<int64_t>(value);
        return *this;
    }

    template <std::integral T>
    [[nodiscard]] constexpr operator T() const noexcept {
        return static_cast<int64_t>(mStorage);
    }

    [[nodiscard]] Int64Tag operator+() const noexcept { return Int64Tag{+mStorage}; }

    [[nodiscard]] constexpr Type getType() const override { return Type::Int64; }

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    [[nodiscard]] constexpr int64_t&       storage() { return mStorage; }
    [[nodiscard]] constexpr int64_t const& storage() const { return mStorage; }
};

} // namespace bedrock_protocol
