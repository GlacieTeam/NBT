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

namespace nbt {

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
class LongArrayTag;

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
    [[nodiscard]] NBT_API CompoundTag() = default;
    [[nodiscard]] NBT_API CompoundTag(std::initializer_list<TagMap::value_type> tagPairs) : mTagMap(tagPairs) {}

    [[nodiscard]] NBT_API Type getType() const override;

    [[nodiscard]] NBT_API bool equals(Tag const& other) const override;

    [[nodiscard]] NBT_API std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] NBT_API std::size_t hash() const override;

    NBT_API void write(BytesDataOutput& stream) const override;

    NBT_API void load(BytesDataInput& stream) override;

    NBT_API void write(bstream::BinaryStream& stream) const override;

    NBT_API void load(bstream::ReadOnlyBinaryStream& stream) override;

    NBT_API void merge(CompoundTag const& other, bool mergeList = false);

    [[nodiscard]] NBT_API TagMap&       items() noexcept;
    [[nodiscard]] NBT_API TagMap const& items() const noexcept;

public:
    [[nodiscard]] NBT_API CompoundTagVariant&       operator[](std::string_view index);
    [[nodiscard]] NBT_API CompoundTagVariant const& operator[](std::string_view index) const;

    [[nodiscard]] NBT_API CompoundTagVariant&       at(std::string_view index);
    [[nodiscard]] NBT_API CompoundTagVariant const& at(std::string_view index) const;

    [[nodiscard]] NBT_API iterator begin() noexcept;
    [[nodiscard]] NBT_API iterator end() noexcept;

    [[nodiscard]] NBT_API const_iterator begin() const noexcept;
    [[nodiscard]] NBT_API const_iterator end() const noexcept;

    [[nodiscard]] NBT_API reverse_iterator rbegin() noexcept;
    [[nodiscard]] NBT_API reverse_iterator rend() noexcept;

    [[nodiscard]] NBT_API const_iterator cbegin() const noexcept;
    [[nodiscard]] NBT_API const_iterator cend() const noexcept;

    [[nodiscard]] NBT_API const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] NBT_API const_reverse_iterator crend() const noexcept;

    NBT_API iterator erase(const_iterator where) noexcept;
    NBT_API iterator erase(const_iterator first, const_iterator last) noexcept;

public:
    NBT_API void put(std::string_view key, Tag&& tag);
    NBT_API void put(std::string_view key, std::unique_ptr<Tag> tag);

    NBT_API void putByte(std::string_view key, uint8_t value);
    NBT_API void putShort(std::string_view key, int16_t value);
    NBT_API void putInt(std::string_view key, int32_t value);
    NBT_API void putInt64(std::string_view key, int64_t value);
    NBT_API void putFloat(std::string_view key, float value);
    NBT_API void putDouble(std::string_view key, double value);
    NBT_API void putString(std::string_view key, std::string_view value);
    NBT_API void putByteArray(std::string_view key, std::vector<uint8_t> const& value);
    NBT_API void putIntArray(std::string_view key, std::vector<int> const& value);
    NBT_API void putLongArray(std::string_view key, std::vector<int64_t> const& value);
    NBT_API void putCompound(std::string_view key, CompoundTag&& value);
    NBT_API void putCompound(std::string_view key, std::unique_ptr<CompoundTag> value);
    NBT_API void putList(std::string_view key, ListTag&& value);
    NBT_API void putList(std::string_view key, std::unique_ptr<ListTag> value);

    [[nodiscard]] NBT_API const Tag* get(std::string_view key) const;
    [[nodiscard]] NBT_API Tag*       get(std::string_view key);

    [[nodiscard]] NBT_API const ByteTag* getByte(std::string_view key) const;
    [[nodiscard]] NBT_API ByteTag*       getByte(std::string_view key);

    [[nodiscard]] NBT_API const ShortTag* getShort(std::string_view key) const;
    [[nodiscard]] NBT_API ShortTag*       getShort(std::string_view key);

    [[nodiscard]] NBT_API const IntTag* getInt(std::string_view key) const;
    [[nodiscard]] NBT_API IntTag*       getInt(std::string_view key);

    [[nodiscard]] NBT_API const Int64Tag* getInt64(std::string_view key) const;
    [[nodiscard]] NBT_API Int64Tag*       getInt64(std::string_view key);

    [[nodiscard]] NBT_API const FloatTag* getFloat(std::string_view key) const;
    [[nodiscard]] NBT_API FloatTag*       getFloat(std::string_view key);

    [[nodiscard]] NBT_API const DoubleTag* getDouble(std::string_view key) const;
    [[nodiscard]] NBT_API DoubleTag*       getDouble(std::string_view key);

    [[nodiscard]] NBT_API const StringTag* getString(std::string_view key) const;
    [[nodiscard]] NBT_API StringTag*       getString(std::string_view key);

    [[nodiscard]] NBT_API const ByteArrayTag* getByteArray(std::string_view key) const;
    [[nodiscard]] NBT_API ByteArrayTag*       getByteArray(std::string_view key);

    [[nodiscard]] NBT_API const IntArrayTag* getIntArray(std::string_view key) const;
    [[nodiscard]] NBT_API IntArrayTag*       getIntArray(std::string_view key);

    [[nodiscard]] NBT_API const LongArrayTag* getLongArray(std::string_view key) const;
    [[nodiscard]] NBT_API LongArrayTag*       getLongArray(std::string_view key);

    [[nodiscard]] NBT_API const CompoundTag* getCompound(std::string_view key) const;
    [[nodiscard]] NBT_API CompoundTag*       getCompound(std::string_view key);

    [[nodiscard]] NBT_API const ListTag* getList(std::string_view key) const;
    [[nodiscard]] NBT_API ListTag*       getList(std::string_view key);

    [[nodiscard]] NBT_API bool contains(std::string_view key) const;
    [[nodiscard]] NBT_API bool contains(std::string_view key, Type type) const;

    [[nodiscard]] NBT_API bool empty() const noexcept;

    [[nodiscard]] NBT_API size_t size() const noexcept;

    NBT_API bool remove(std::string_view index);
    NBT_API bool rename(std::string_view index, std::string_view newName);
    NBT_API void clear() noexcept;

    [[nodiscard]] NBT_API std::unique_ptr<CompoundTag> clone() const;

public:
    NBT_API void serialize(bstream::BinaryStream& stream) const;
    NBT_API void serialize(BytesDataOutput& stream) const;

    NBT_API void deserialize(bstream::ReadOnlyBinaryStream& stream);
    NBT_API void deserialize(BytesDataInput& stream);

    [[nodiscard]] NBT_API std::string toNetworkNbt() const noexcept;
    [[nodiscard]] NBT_API std::string toBinaryNbt(bool isLittleEndian = true) const noexcept;
    [[nodiscard]] NBT_API std::string toBinaryNbtWithHeader(bool isLittleEndian = true) const noexcept;

public:
    [[nodiscard]] NBT_API static std::optional<CompoundTag> fromNetworkNbt(std::string_view binaryData) noexcept;
    [[nodiscard]] NBT_API static std::optional<CompoundTag>
    fromBinaryNbt(std::string_view binaryData, bool isLittleEndian = true) noexcept;
    [[nodiscard]] NBT_API static std::optional<CompoundTag>
    fromBinaryNbtWithHeader(std::string_view binaryData, bool isLittleEndian = true) noexcept;
    [[nodiscard]] NBT_API static std::optional<CompoundTag>
    fromSnbt(std::string_view snbt, std::optional<size_t> parsedLength = {}) noexcept;
};

} // namespace nbt
