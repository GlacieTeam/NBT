// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/Tag.hpp>
#include <vector>

namespace bedrock_protocol {

class ByteArrayTag : public Tag {
protected:
    std::vector<uint8_t> mData;

public:
    [[nodiscard]] ByteArrayTag() = default;

    [[nodiscard]] ByteArrayTag(std::vector<uint8_t> arr);
    [[nodiscard]] ByteArrayTag(std::initializer_list<uint8_t> val);

    [[nodiscard]] operator std::vector<uint8_t>();

    [[nodiscard]] Type getType() const override;

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

public:
    [[nodiscard]] std::vector<uint8_t>&       data();
    [[nodiscard]] std::vector<uint8_t> const& data() const;

    [[nodiscard]] size_t size() const;
};

} // namespace bedrock_protocol
