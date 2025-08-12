// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class IntTag : public Tag {
protected:
    int mData{0};

public:
    [[nodiscard]] constexpr IntTag() = default;

    template <std::integral T>
    [[nodiscard]] constexpr explicit IntTag(T value) noexcept : mData(static_cast<int>(value)) {}

    template <std::integral T>
    constexpr IntTag& operator=(int value) noexcept {
        mData = static_cast<int>(value);
        return *this;
    }

    template <std::integral T>
    [[nodiscard]] constexpr operator T() const noexcept {
        return static_cast<T>(mData);
    }

    [[nodiscard]] IntTag operator+() const noexcept { return IntTag{+mData}; }

    [[nodiscard]] Type getType() const override;

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    [[nodiscard]] int&       data();
    [[nodiscard]] int const& data() const;
};

} // namespace bedrock_protocol
