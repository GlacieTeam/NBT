// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class EndTag : public Tag {
public:
    [[nodiscard]] EndTag() = default;

    [[nodiscard]] NBT_API Type getType() const override;

    [[nodiscard]] NBT_API bool equals(Tag const& other) const override;

    [[nodiscard]] NBT_API std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] NBT_API std::size_t hash() const override;

    NBT_API void write(BytesDataOutput& stream) const override;

    NBT_API void load(BytesDataInput& stream) override;

    NBT_API void write(BinaryStream& stream) const override;

    NBT_API void load(ReadOnlyBinaryStream& stream) override;
};

} // namespace bedrock_protocol
