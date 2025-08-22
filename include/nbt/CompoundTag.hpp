// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <map>
#include <nbt/Tag.hpp>
#include <optional>
#include <vector>

namespace bedrock_protocol {

class CompoundTagVariant;
class EndTag;
class ByteTag;
class ShortTag;
class IntTag;
class Int64Tag;
class FloatTag;
class DoubleTag;
class ByteArrayTag;
class StringTag;
class ListTag;
class IntArrayTag;

class CompoundTag : public Tag {
public:
    using TagMap = std::map<std::string, CompoundTagVariant, std::less<>>;

private:
    TagMap mTagMap;

public:
    using iterator               = TagMap::iterator;
    using const_iterator         = TagMap::const_iterator;
    using reverse_iterator       = TagMap::reverse_iterator;
    using const_reverse_iterator = TagMap::const_reverse_iterator;

public:
    [[nodiscard]] CompoundTag() = default;
    [[nodiscard]] CompoundTag(std::initializer_list<TagMap::value_type> tagPairs) : mTagMap(tagPairs) {}

    [[nodiscard]] Type getType() const override;

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    [[nodiscard]] TagMap&       items() noexcept;
    [[nodiscard]] TagMap const& items() const noexcept;

public:
    [[nodiscard]] CompoundTagVariant&       operator[](std::string_view index);
    [[nodiscard]] CompoundTagVariant const& operator[](std::string_view index) const;

    [[nodiscard]] CompoundTagVariant&       at(std::string_view index);
    [[nodiscard]] CompoundTagVariant const& at(std::string_view index) const;

    [[nodiscard]] iterator begin() noexcept;
    [[nodiscard]] iterator end() noexcept;

    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] const_iterator end() const noexcept;

    [[nodiscard]] reverse_iterator rbegin() noexcept;
    [[nodiscard]] reverse_iterator rend() noexcept;

    [[nodiscard]] const_iterator cbegin() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] const_reverse_iterator crend() const noexcept;

    iterator erase(const_iterator where) noexcept;
    iterator erase(const_iterator first, const_iterator last) noexcept;

public:
    void put(std::string_view key, Tag&& tag);
    void put(std::string_view key, std::unique_ptr<Tag> tag);

    void putByte(std::string_view key, uint8_t value);
    void putShort(std::string_view key, int16_t value);
    void putInt(std::string_view key, int32_t value);
    void putInt64(std::string_view key, int64_t value);
    void putFloat(std::string_view key, float value);
    void putDouble(std::string_view key, double value);
    void putString(std::string_view key, std::string_view value);
    void putByteArray(std::string_view key, std::vector<uint8_t> const& value);
    void putIntArray(std::string_view key, std::vector<int> const& value);
    void putCompound(std::string_view key, CompoundTag&& value);
    void putCompound(std::string_view key, std::unique_ptr<CompoundTag> value);
    void putList(std::string_view key, ListTag&& value);
    void putList(std::string_view key, std::unique_ptr<ListTag> value);

    [[nodiscard]] const Tag* get(std::string_view key) const;
    [[nodiscard]] Tag*       get(std::string_view key);

    [[nodiscard]] const ByteTag* getByte(std::string_view key) const;
    [[nodiscard]] ByteTag*       getByte(std::string_view key);

    [[nodiscard]] const ShortTag* getShort(std::string_view key) const;
    [[nodiscard]] ShortTag*       getShort(std::string_view key);

    [[nodiscard]] const IntTag* getInt(std::string_view key) const;
    [[nodiscard]] IntTag*       getInt(std::string_view key);

    [[nodiscard]] const Int64Tag* getInt64(std::string_view key) const;
    [[nodiscard]] Int64Tag*       getInt64(std::string_view key);

    [[nodiscard]] const FloatTag* getFloat(std::string_view key) const;
    [[nodiscard]] FloatTag*       getFloat(std::string_view key);

    [[nodiscard]] const DoubleTag* getDouble(std::string_view key) const;
    [[nodiscard]] DoubleTag*       getDouble(std::string_view key);

    [[nodiscard]] const StringTag* getString(std::string_view key) const;
    [[nodiscard]] StringTag*       getString(std::string_view key);

    [[nodiscard]] const ByteArrayTag* getByteArray(std::string_view key) const;
    [[nodiscard]] ByteArrayTag*       getByteArray(std::string_view key);

    [[nodiscard]] const IntArrayTag* getIntArray(std::string_view key) const;
    [[nodiscard]] IntArrayTag*       getIntArray(std::string_view key);

    [[nodiscard]] const CompoundTag* getCompound(std::string_view key) const;
    [[nodiscard]] CompoundTag*       getCompound(std::string_view key);

    [[nodiscard]] const ListTag* getList(std::string_view key) const;
    [[nodiscard]] ListTag*       getList(std::string_view key);

    [[nodiscard]] bool contains(std::string_view key) const;
    [[nodiscard]] bool contains(std::string_view key, Type type) const;

    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] size_t size() const noexcept;

    bool remove(std::string_view index);
    bool rename(std::string_view index, std::string_view newName);
    void clear() noexcept;

    [[nodiscard]] std::unique_ptr<CompoundTag> clone() const;

public:
    void serialize(BinaryStream& stream) const;
    void serialize(BytesDataOutput& stream) const;

    void deserialize(ReadOnlyBinaryStream& stream);
    void deserialize(BytesDataInput& stream);

    [[nodiscard]] std::string toNetworkNbt() const noexcept;
    [[nodiscard]] std::string toBinaryNbt(bool isLittleEndian = true) const noexcept;

public:
    [[nodiscard]] static std::optional<CompoundTag> fromNetworkNbt(std::string_view binaryData) noexcept;
    [[nodiscard]] static std::optional<CompoundTag>
    fromBinaryNbt(std::string_view binaryData, bool isLittleEndian = true) noexcept;
    [[nodiscard]] static std::optional<CompoundTag>
    fromSnbt(std::string_view snbt, std::optional<size_t> parsedLength = {}) noexcept;
};

} // namespace bedrock_protocol
