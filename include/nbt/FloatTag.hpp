// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class FloatTag : public Tag {
protected:
    float mStorage{0};

public:
    [[nodiscard]] constexpr FloatTag() = default;

    [[nodiscard]] constexpr explicit FloatTag(float value) noexcept : mStorage(value) {}

    constexpr FloatTag& operator=(float value) noexcept {
        mStorage = value;
        return *this;
    }

    constexpr operator float() const noexcept { return mStorage; }

    [[nodiscard]] constexpr Type getType() const override { return Type::Float; }

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    [[nodiscard]] constexpr float&       storage() { return mStorage; }
    [[nodiscard]] constexpr float const& storage() const { return mStorage; }
};

} // namespace bedrock_protocol
