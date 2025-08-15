// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class ShortTag : public Tag {
protected:
    short mStorage{0};

public:
    [[nodiscard]] constexpr ShortTag() = default;

    template <std::integral T>
    [[nodiscard]] constexpr explicit ShortTag(T value) noexcept : mStorage(static_cast<short>(value)) {}

    template <std::integral T>
    constexpr ShortTag& operator=(T value) noexcept {
        mStorage = static_cast<short>(value);
        return *this;
    }

    template <std::integral T>
    [[nodiscard]] constexpr operator T() const noexcept {
        return static_cast<T>(mStorage);
    }

    [[nodiscard]] ShortTag operator+() const noexcept { return ShortTag{+mStorage}; }

    [[nodiscard]] constexpr Type getType() const override { return Type::Short; }

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    [[nodiscard]] constexpr short&       storage() noexcept { return mStorage; }
    [[nodiscard]] constexpr short const& storage() const noexcept { return mStorage; }
};

} // namespace bedrock_protocol
