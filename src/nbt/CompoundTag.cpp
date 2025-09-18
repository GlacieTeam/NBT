// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/CompoundTag.hpp"
#include "nbt/ByteArrayTag.hpp"
#include "nbt/ByteTag.hpp"
#include "nbt/CompoundTagVariant.hpp"
#include "nbt/DoubleTag.hpp"
#include "nbt/EndTag.hpp"
#include "nbt/FloatTag.hpp"
#include "nbt/Int64Tag.hpp"
#include "nbt/IntArrayTag.hpp"
#include "nbt/IntTag.hpp"
#include "nbt/ListTag.hpp"
#include "nbt/LongArrayTag.hpp"
#include "nbt/ShortTag.hpp"
#include "nbt/StringTag.hpp"
#include <algorithm>

namespace nbt {

bool CompoundTag::equals(Tag const& other) const {
    if (other.getType() != Type::Compound) { return false; }
    const auto& otherTag = static_cast<const CompoundTag&>(other);
    if (size() != otherTag.size()) { return false; }
    return std::all_of(mTagMap.begin(), mTagMap.end(), [&](const auto& kv) {
        const auto* tag = otherTag.get(kv.first);
        return tag && kv.second.get()->equals(*tag);
    });
}

std::unique_ptr<Tag> CompoundTag::copy() const { return clone(); }

std::size_t CompoundTag::hash() const {
    size_t           hash   = 0x811c9dc5;
    constexpr size_t prime  = 0x01000193;
    auto             binary = toBinaryNbt();
    for (char byte : binary) {
        hash ^= static_cast<size_t>(byte);
        hash  = hash * prime;
    }
    return hash;
}

Tag::Type CompoundTag::getType() const { return Type::Compound; }

std::unique_ptr<CompoundTag> CompoundTag::clone() const {
    auto tag = std::make_unique<CompoundTag>();
    for (const auto& [key, value] : mTagMap) { tag->mTagMap.emplace(key, value.toUniqueCopy()); }
    return tag;
}

void CompoundTag::write(io::BytesDataOutput& stream) const {
    for (const auto& [key, tag] : mTagMap) {
        auto type = tag->getType();
        stream.writeByte(static_cast<uint8_t>(type));
        if (type != Type::End) {
            stream.writeString(key);
            tag->write(stream);
        }
    }
    stream.writeByte(static_cast<uint8_t>(Type::End));
}

void CompoundTag::load(io::BytesDataInput& stream) {
    while (true) {
        const Type type = static_cast<Type>(stream.getByte());
        if (type == Type::End) { return; }
        auto key    = stream.getStringView();
        auto tagPtr = Tag::newTag(type);
        tagPtr->load(stream);
        mTagMap.emplace(std::move(key), std::move(*tagPtr));
    }
}

void CompoundTag::write(bstream::BinaryStream& stream) const {
    for (const auto& [key, tag] : mTagMap) {
        auto type = tag->getType();
        stream.writeUnsignedChar(static_cast<uint8_t>(type));
        if (type != Type::End) {
            stream.writeString(key);
            tag->write(stream);
        }
    }
    stream.writeUnsignedChar(static_cast<uint8_t>(Type::End));
}

void CompoundTag::load(bstream::ReadOnlyBinaryStream& stream) {
    while (true) {
        const Type type = static_cast<Type>(stream.getByte());
        if (type == Type::End) { return; }
        auto key    = stream.getStringView();
        auto tagPtr = Tag::newTag(type);
        tagPtr->load(stream);
        mTagMap.emplace(std::move(key), std::move(*tagPtr));
    }
}

void CompoundTag::merge(CompoundTag const& other, bool mergeList) {
    for (auto const& [key, val] : other) { mTagMap[key].merge(val, mergeList); }
}

bool CompoundTag::put(std::string_view key, Tag&& tag) {
    auto [_, result] = mTagMap.emplace(key, std::forward<Tag>(tag));
    return result;
}

bool CompoundTag::put(std::string_view key, std::unique_ptr<Tag>&& tag) {
    if (tag) {
        auto [_, result] = mTagMap.emplace(key, std::move(*tag));
        return result;
    }
    return false;
}

bool CompoundTag::putByte(std::string_view key, uint8_t value) {
    auto [_, result] = mTagMap.emplace(key, ByteTag(value));
    return result;
}

bool CompoundTag::putShort(std::string_view key, int16_t value) {
    auto [_, result] = mTagMap.emplace(key, ShortTag(value));
    return result;
}

bool CompoundTag::putInt(std::string_view key, int32_t value) {
    auto [_, result] = mTagMap.emplace(key, IntTag(value));
    return result;
}

bool CompoundTag::putInt64(std::string_view key, int64_t value) {
    auto [_, result] = mTagMap.emplace(key, Int64Tag(value));
    return result;
}

bool CompoundTag::putFloat(std::string_view key, float value) {
    auto [_, result] = mTagMap.emplace(key, FloatTag(value));
    return result;
}

bool CompoundTag::putDouble(std::string_view key, double value) {
    auto [_, result] = mTagMap.emplace(key, DoubleTag(value));
    return result;
}

bool CompoundTag::putString(std::string_view key, std::string_view value) {
    auto [_, result] = mTagMap.emplace(key, StringTag(value));
    return result;
}

bool CompoundTag::putByteArray(std::string_view key, std::vector<uint8_t> const& value) {
    auto [_, result] = mTagMap.emplace(key, ByteArrayTag(value));
    return result;
}

bool CompoundTag::putIntArray(std::string_view key, std::vector<int> const& value) {
    auto [_, result] = mTagMap.emplace(key, IntArrayTag(value));
    return result;
}

bool CompoundTag::putLongArray(std::string_view key, std::vector<int64_t> const& value) {
    auto [_, result] = mTagMap.emplace(key, LongArrayTag(value));
    return result;
}

bool CompoundTag::putCompound(std::string_view key, CompoundTag&& value) {
    auto [_, result] = mTagMap.emplace(key, std::move(value));
    return result;
}

bool CompoundTag::putCompound(std::string_view key, std::unique_ptr<CompoundTag>&& value) {
    if (value) {
        auto [_, result] = mTagMap.emplace(key, std::move(*value));
        return result;
    }
    return false;
}

bool CompoundTag::putList(std::string_view key, ListTag&& value) {
    auto [_, result] = mTagMap.emplace(key, std::move(value));
    return result;
}

bool CompoundTag::putList(std::string_view key, std::unique_ptr<ListTag>&& value) {
    if (value) {
        auto [_, result] = mTagMap.emplace(key, std::move(*value));
        return result;
    }
    return false;
}

void CompoundTag::set(std::string_view key, Tag&& tag) { operator[](key) = tag; }

void CompoundTag::set(std::string_view key, std::unique_ptr<Tag>&& tag) { operator[](key) = std::move(tag); }

void CompoundTag::setByte(std::string_view key, uint8_t value) { operator[](key) = value; }

void CompoundTag::setShort(std::string_view key, int16_t value) { operator[](key) = value; }

void CompoundTag::setInt(std::string_view key, int32_t value) { operator[](key) = value; }

void CompoundTag::setInt64(std::string_view key, int64_t value) { operator[](key) = value; }

void CompoundTag::setFloat(std::string_view key, float value) { operator[](key) = value; }

void CompoundTag::setDouble(std::string_view key, double value) { operator[](key) = value; }

void CompoundTag::setString(std::string_view key, std::string_view value) { operator[](key) = value; }

void CompoundTag::setByteArray(std::string_view key, std::vector<uint8_t> const& value) { operator[](key) = value; }

void CompoundTag::setIntArray(std::string_view key, std::vector<int> const& value) { operator[](key) = value; }

void CompoundTag::setLongArray(std::string_view key, std::vector<int64_t> const& value) { operator[](key) = value; }

void CompoundTag::setCompound(std::string_view key, CompoundTag&& value) { operator[](key) = value; }

void CompoundTag::setCompound(std::string_view key, std::unique_ptr<CompoundTag>&& value) {
    operator[](key) = std::move(*value);
}

void CompoundTag::setList(std::string_view key, ListTag&& value) { operator[](key) = value; }

void CompoundTag::setList(std::string_view key, std::unique_ptr<ListTag>&& value) {
    operator[](key) = std::move(*value);
}

const Tag* CompoundTag::get(std::string_view key) const {
    if (mTagMap.contains(key)) { return mTagMap.at(std::string(key)).get(); }
    return nullptr;
}

Tag* CompoundTag::get(std::string_view key) {
    if (mTagMap.contains(key)) { return mTagMap.at(std::string(key)).get(); }
    return nullptr;
}

const ByteTag* CompoundTag::getByte(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::Byte) { return static_cast<const ByteTag*>(tag); }
    }
    return nullptr;
}

ByteTag* CompoundTag::getByte(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Byte) { return static_cast<ByteTag*>(tag); }
    }
    return nullptr;
}

const ShortTag* CompoundTag::getShort(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::Short) { return static_cast<const ShortTag*>(tag); }
    }
    return nullptr;
}

ShortTag* CompoundTag::getShort(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Short) { return static_cast<ShortTag*>(tag); }
    }
    return nullptr;
}

const IntTag* CompoundTag::getInt(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::Int) { return static_cast<const IntTag*>(tag); }
    }
    return nullptr;
}

IntTag* CompoundTag::getInt(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Int) { return static_cast<IntTag*>(tag); }
    }
    return nullptr;
}

const Int64Tag* CompoundTag::getInt64(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::Int64) { return static_cast<const Int64Tag*>(tag); }
    }
    return nullptr;
}

Int64Tag* CompoundTag::getInt64(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Int64) { return static_cast<Int64Tag*>(tag); }
    }
    return nullptr;
}

const FloatTag* CompoundTag::getFloat(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::Float) { return static_cast<const FloatTag*>(tag); }
    }
    return nullptr;
}

FloatTag* CompoundTag::getFloat(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Float) { return static_cast<FloatTag*>(tag); }
    }
    return nullptr;
}

const DoubleTag* CompoundTag::getDouble(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::Double) { return static_cast<const DoubleTag*>(tag); }
    }
    return nullptr;
}

DoubleTag* CompoundTag::getDouble(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Double) { return static_cast<DoubleTag*>(tag); }
    }
    return nullptr;
}

const StringTag* CompoundTag::getString(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::String) { return static_cast<const StringTag*>(tag); }
    }
    return nullptr;
}

StringTag* CompoundTag::getString(std::string_view key) {
    if (auto* tag = get(key)) {
        if (tag->getType() == Type::String) { return static_cast<StringTag*>(tag); }
    }
    return nullptr;
}

const ByteArrayTag* CompoundTag::getByteArray(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::ByteArray) { return static_cast<const ByteArrayTag*>(tag); }
    }
    return nullptr;
}

ByteArrayTag* CompoundTag::getByteArray(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::ByteArray) { return static_cast<ByteArrayTag*>(tag); }
    }
    return nullptr;
}

const IntArrayTag* CompoundTag::getIntArray(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::IntArray) { return static_cast<const IntArrayTag*>(tag); }
    }
    return nullptr;
}

IntArrayTag* CompoundTag::getIntArray(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::IntArray) { return static_cast<IntArrayTag*>(tag); }
    }
    return nullptr;
}

const LongArrayTag* CompoundTag::getLongArray(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Type::LongArray) { return static_cast<const LongArrayTag*>(tag); }
    }
    return nullptr;
}

LongArrayTag* CompoundTag::getLongArray(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::LongArray) { return static_cast<LongArrayTag*>(tag); }
    }
    return nullptr;
}

const CompoundTag* CompoundTag::getCompound(std::string_view key) const {
    if (const auto* tag = get(key); tag) {
        if (tag->getType() == Type::Compound) { return static_cast<const CompoundTag*>(tag); }
    }
    return nullptr;
}

CompoundTag* CompoundTag::getCompound(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Compound) { return static_cast<CompoundTag*>(tag); }
    }
    return nullptr;
}

const ListTag* CompoundTag::getList(std::string_view key) const {
    if (const auto* tag = get(key); tag) {
        if (tag->getType() == Type::List) { return static_cast<const ListTag*>(tag); }
    }
    return nullptr;
}

ListTag* CompoundTag::getList(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::List) { return static_cast<ListTag*>(tag); }
    }
    return nullptr;
}

bool CompoundTag::contains(std::string_view key) const { return get(key) != nullptr; }

bool CompoundTag::contains(std::string_view key, Tag::Type type) const {
    if (const auto* tag = get(key); tag) { return tag->getType() == type; }
    return false;
}

bool CompoundTag::empty() const noexcept { return mTagMap.empty(); }

bool CompoundTag::remove(std::string_view index) {
    if (mTagMap.contains(index)) {
        mTagMap.erase(std::string(index));
        return true;
    }
    return false;
}

bool CompoundTag::rename(std::string_view index, std::string_view newName) {
    if (contains(index)) {
        mTagMap[std::string(newName)] = mTagMap.at(std::string(index));
        remove(index);
        return true;
    }
    return false;
}

void CompoundTag::clear() noexcept { mTagMap.clear(); }

CompoundTag::iterator CompoundTag::begin() noexcept { return mTagMap.begin(); }
CompoundTag::iterator CompoundTag::end() noexcept { return mTagMap.end(); }

CompoundTag::const_iterator CompoundTag::begin() const noexcept { return cbegin(); }
CompoundTag::const_iterator CompoundTag::end() const noexcept { return cend(); }

CompoundTag::reverse_iterator CompoundTag::rbegin() noexcept { return mTagMap.rbegin(); }
CompoundTag::reverse_iterator CompoundTag::rend() noexcept { return mTagMap.rend(); }

CompoundTag::const_iterator CompoundTag::cbegin() const noexcept { return mTagMap.cbegin(); }
CompoundTag::const_iterator CompoundTag::cend() const noexcept { return mTagMap.cend(); }

CompoundTag::const_reverse_iterator CompoundTag::crbegin() const noexcept { return mTagMap.crbegin(); }
CompoundTag::const_reverse_iterator CompoundTag::crend() const noexcept { return mTagMap.crend(); }

CompoundTag::iterator CompoundTag::erase(const_iterator where) noexcept { return mTagMap.erase(where); }
CompoundTag::iterator CompoundTag::erase(const_iterator first, const_iterator last) noexcept {
    return mTagMap.erase(first, last);
}

CompoundTag::TagMap&       CompoundTag::items() noexcept { return mTagMap; }
CompoundTag::TagMap const& CompoundTag::items() const noexcept { return mTagMap; }

void CompoundTag::serialize(bstream::BinaryStream& stream) const {
    stream.writeByte(static_cast<std::byte>(Type::Compound));
    stream.writeString("");
    write(stream);
}

void CompoundTag::serialize(io::BytesDataOutput& stream) const {
    stream.writeByte(static_cast<uint8_t>(Type::Compound));
    stream.writeString("");
    write(stream);
}

void CompoundTag::deserialize(bstream::ReadOnlyBinaryStream& stream) {
    auto tagType = static_cast<Type>(stream.getByte());
    (void)stream.getStringView();
    if (tagType == Type::Compound) { load(stream); }
}

void CompoundTag::deserialize(io::BytesDataInput& stream) {
    auto tagType = static_cast<Type>(stream.getByte());
    (void)stream.getStringView();
    if (tagType == Type::Compound) { load(stream); }
}

std::optional<CompoundTag> CompoundTag::fromBinaryNbt(std::string_view binaryData, bool isLittleEndian) noexcept try {
    io::BytesDataInput stream(binaryData, false, isLittleEndian);
    CompoundTag        result;
    result.deserialize(stream);
    return result;
} catch (...) { return std::nullopt; }

std::optional<CompoundTag>
CompoundTag::fromBinaryNbtWithHeader(std::string_view binaryData, bool isLittleEndian) noexcept try {
    io::BytesDataInput stream(binaryData, false, isLittleEndian);
    stream.ignoreBytes(sizeof(int));
    return fromBinaryNbt(stream.getLongStringView(), isLittleEndian);
} catch (...) { return std::nullopt; }

std::string CompoundTag::toBinaryNbt(bool isLittleEndian) const noexcept {
    io::BytesDataOutput stream(isLittleEndian);
    serialize(stream);
    return stream.getAndReleaseData();
}

std::string CompoundTag::toBinaryNbtWithHeader(bool isLittleEndian, std::optional<int> storageVersion) const noexcept {
    io::BytesDataOutput stream(isLittleEndian);
    int                 storage_version = 0;
    if (storageVersion.has_value()) {
        storage_version = *storageVersion;
    } else if (contains("StorageVersion")) {
        auto& version = at("StorageVersion");
        if (version.getType() == Type::Int) { storage_version = version; }
    }
    stream.writeInt(storage_version);
    stream.writeLongString(toBinaryNbt(isLittleEndian));
    return stream.getAndReleaseData();
}

std::optional<CompoundTag> CompoundTag::fromNetworkNbt(std::string_view binaryData) noexcept try {
    bstream::ReadOnlyBinaryStream stream(binaryData, false);
    CompoundTag                   result;
    result.deserialize(stream);
    return result;
} catch (...) { return std::nullopt; }

std::string CompoundTag::toNetworkNbt() const noexcept {
    bstream::BinaryStream stream;
    serialize(stream);
    return stream.getAndReleaseData();
}

CompoundTagVariant&       CompoundTag::at(std::string_view index) { return mTagMap.at(std::string(index)); }
CompoundTagVariant const& CompoundTag::at(std::string_view index) const { return mTagMap.at(std::string(index)); }

CompoundTagVariant&       CompoundTag::operator[](std::string_view index) { return mTagMap[std::string(index)]; }
CompoundTagVariant const& CompoundTag::operator[](std::string_view index) const {
    return mTagMap.at(std::string(index));
}

size_t CompoundTag::size() const noexcept { return mTagMap.size(); }

std::optional<CompoundTag> CompoundTag::fromSnbt(std::string_view snbt, std::optional<size_t> parsedLength) noexcept {
    return CompoundTagVariant::parse(snbt, parsedLength)
        .and_then([](CompoundTagVariant&& val) -> std::optional<CompoundTag> {
            if (val.hold<CompoundTag>()) {
                return std::move(val.as<CompoundTag>());
            } else {
                return std::nullopt;
            }
        });
}

int CompoundTag::readHeaderVersion(std::string_view content, bool isLittleEndian) noexcept {
    io::BytesDataInput stream(content, isLittleEndian);
    return stream.getInt();
}

} // namespace nbt