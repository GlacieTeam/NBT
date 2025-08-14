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

class IntArrayTag : public Tag {
protected:
    std::vector<int> mStorage;

public:
    [[nodiscard]] IntArrayTag() = default;

    [[nodiscard]] IntArrayTag(std::vector<int> const& arr);
    [[nodiscard]] IntArrayTag(std::initializer_list<int> val);

    [[nodiscard]] operator std::vector<int> const&() const;
    [[nodiscard]] operator std::vector<int>&();

    [[nodiscard]] constexpr Type getType() const override { return Type::IntArray; }

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    [[nodiscard]] std::vector<int>&       storage();
    [[nodiscard]] std::vector<int> const& storage() const;

    [[nodiscard]] size_t size() const;

    void reserve(size_t size);

    bool remove(size_t index);
    bool remove(size_t startIndex, size_t endIndex);
    void clear();

    [[nodiscard]] int&       operator[](size_t index) noexcept;
    [[nodiscard]] int const& operator[](size_t index) const noexcept;

    [[nodiscard]] int&       at(size_t index);
    [[nodiscard]] int const& at(size_t index) const;

    void push_back(int val);

    IntArrayTag& operator=(std::vector<int> const& value);
};

} // namespace bedrock_protocol
