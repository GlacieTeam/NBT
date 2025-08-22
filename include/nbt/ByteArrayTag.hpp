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
    std::vector<uint8_t> mStorage;

public:
    [[nodiscard]] ByteArrayTag() = default;

    [[nodiscard]] ByteArrayTag(std::vector<uint8_t> const& arr);
    [[nodiscard]] ByteArrayTag(std::vector<std::byte> const& arr);
    [[nodiscard]] ByteArrayTag(std::initializer_list<uint8_t> val);
    [[nodiscard]] ByteArrayTag(std::initializer_list<std::byte> val);
    [[nodiscard]] ByteArrayTag(uint8_t const* data, size_t size);
    [[nodiscard]] ByteArrayTag(std::byte const* data, size_t size);
    [[nodiscard]] ByteArrayTag(std::string_view binaryData);

    [[nodiscard]] operator std::vector<uint8_t> const&() const;
    [[nodiscard]] operator std::vector<uint8_t>&();
    [[nodiscard]] operator std::vector<std::byte>() const;
    [[nodiscard]] operator std::string_view() const;

    [[nodiscard]] Type getType() const override;

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

public:
    [[nodiscard]] std::vector<uint8_t>&       storage() noexcept;
    [[nodiscard]] std::vector<uint8_t> const& storage() const noexcept;

    [[nodiscard]] uint8_t const* data() const noexcept;
    [[nodiscard]] uint8_t*       data() noexcept;

    [[nodiscard]] size_t size() const noexcept;

    void reserve(size_t size);

    bool remove(size_t index);
    bool remove(size_t startIndex, size_t endIndex);
    void clear() noexcept;

    [[nodiscard]] uint8_t&       operator[](size_t index) noexcept;
    [[nodiscard]] uint8_t const& operator[](size_t index) const noexcept;

    [[nodiscard]] uint8_t&       at(size_t index);
    [[nodiscard]] uint8_t const& at(size_t index) const;

    void push_back(uint8_t val);
    void push_back(std::byte val);

    ByteArrayTag& operator=(std::vector<uint8_t> const& value);
    ByteArrayTag& operator=(std::vector<std::byte> const& value);
    ByteArrayTag& operator=(std::string_view value);

    void reinit(uint8_t const* data, size_t size);
    void reinit(std::byte const* data, size_t size);
};

} // namespace bedrock_protocol
