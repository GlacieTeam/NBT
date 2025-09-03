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
    for (uint8_t byte : binary) {
        hash ^= byte;
        hash  = hash * prime;
    }
    return hash;
}

Tag::Type CompoundTag::getType() const { return Type::Compound; }

std::unique_ptr<CompoundTag> CompoundTag::clone() const {
    auto new_tag = std::make_unique<CompoundTag>();
    for (const auto& [key, value] : mTagMap) {
        new_tag->mTagMap[std::string(key)].emplace(std::move(*value.get()->copy()));
    }
    return new_tag;
}

void CompoundTag::write(BytesDataOutput& stream) const {
    for (const auto& [key, tag] : mTagMap) {
        auto type = tag->getType();
        stream.writeByte((uint8_t)type);
        if (type != Type::End) {
            stream.writeString(key);
            tag->write(stream);
        }
    }
    stream.writeByte((uint8_t)Type::End);
}

void CompoundTag::load(BytesDataInput& stream) {
    mTagMap.clear();
    while (true) {
        const Type type = static_cast<Type>(stream.getByte());
        if (type == Type::End) { break; }
        auto key    = stream.getStringView();
        auto tagPtr = Tag::newTag(type);
        tagPtr->load(stream);
        mTagMap.emplace(std::move(key), std::move(tagPtr));
    }
}

void CompoundTag::write(bstream::BinaryStream& stream) const {
    for (const auto& [key, tag] : mTagMap) {
        auto type = tag->getType();
        stream.writeUnsignedChar((uint8_t)type);
        if (type != Type::End) {
            stream.writeString(key);
            tag->write(stream);
        }
    }
    stream.writeUnsignedChar((uint8_t)Type::End);
}

void CompoundTag::load(bstream::ReadOnlyBinaryStream& stream) {
    mTagMap.clear();
    while (true) {
        const Type type = static_cast<Type>(stream.getByte());
        if (type == Type::End) { break; }
        std::string key;
        stream.getString(key);
        auto tagPtr = Tag::newTag(type);
        tagPtr->load(stream);
        mTagMap.emplace(std::move(key), std::move(tagPtr));
    }
}

void CompoundTag::merge(CompoundTag const& other, bool mergeList) {
    for (auto const& [key, val] : other) {
        if (mTagMap.contains(key)) {
            if (mTagMap[key].getType() == Type::Compound && val.getType() == Type::Compound) {
                mTagMap[key].as<CompoundTag>().merge(val.as<CompoundTag>());
                continue;
            } else if (mTagMap[key].getType() == Type::List && val.getType() == Type::List && mergeList) {
                mTagMap[key].as<ListTag>().merge(val.as<ListTag>());
                continue;
            }
        }
        mTagMap[key] = val;
    }
}

void CompoundTag::put(std::string_view key, Tag&& tag) { mTagMap[std::string(key)].emplace(std::forward<Tag>(tag)); }

void CompoundTag::put(std::string_view key, std::unique_ptr<Tag> tag) {
    if (tag) { mTagMap[std::string(key)].emplace(std::move(*tag)); }
}

void CompoundTag::putByte(std::string_view key, uint8_t value) { mTagMap[std::string(key)].emplace(ByteTag(value)); }

void CompoundTag::putShort(std::string_view key, int16_t value) { mTagMap[std::string(key)].emplace(ShortTag(value)); }

void CompoundTag::putInt(std::string_view key, int32_t value) { mTagMap[std::string(key)].emplace(IntTag(value)); }

void CompoundTag::putInt64(std::string_view key, int64_t value) { mTagMap[std::string(key)].emplace(Int64Tag(value)); }

void CompoundTag::putFloat(std::string_view key, float value) { mTagMap[std::string(key)].emplace(FloatTag(value)); }

void CompoundTag::putDouble(std::string_view key, double value) { mTagMap[std::string(key)].emplace(DoubleTag(value)); }

void CompoundTag::putString(std::string_view key, std::string_view value) {
    mTagMap[std::string(key)].emplace(StringTag(value));
}

void CompoundTag::putByteArray(std::string_view key, std::vector<uint8_t> const& value) {
    mTagMap[std::string(key)].emplace(ByteArrayTag(value));
}

void CompoundTag::putIntArray(std::string_view key, std::vector<int> const& value) {
    mTagMap[std::string(key)].emplace(IntArrayTag(value));
}

void CompoundTag::putLongArray(std::string_view key, std::vector<int64_t> const& value) {
    mTagMap[std::string(key)].emplace(LongArrayTag(value));
}

void CompoundTag::putCompound(std::string_view key, CompoundTag&& value) {
    mTagMap[std::string(key)].emplace(std::move(value));
}

void CompoundTag::putCompound(std::string_view key, std::unique_ptr<CompoundTag> value) {
    if (value) { mTagMap[std::string(key)].emplace(std::move(*value)); }
}

void CompoundTag::putList(std::string_view key, ListTag&& value) {
    mTagMap[std::string(key)].emplace(std::move(value));
}

void CompoundTag::putList(std::string_view key, std::unique_ptr<ListTag> value) {
    if (value) { mTagMap[std::string(key)].emplace(std::move(*value)); }
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

void CompoundTag::serialize(BytesDataOutput& stream) const {
    stream.writeByte(static_cast<uint8_t>(Type::Compound));
    stream.writeString("");
    write(stream);
}

void CompoundTag::deserialize(bstream::ReadOnlyBinaryStream& stream) {
    auto tagType = static_cast<Type>(stream.getByte());
    (void)stream.getString();
    if (tagType == Type::Compound) { load(stream); }
}

void CompoundTag::deserialize(BytesDataInput& stream) {
    auto tagType = static_cast<Type>(stream.getByte());
    (void)stream.getStringView();
    if (tagType == Type::Compound) { load(stream); }
}

std::optional<CompoundTag> CompoundTag::fromBinaryNbt(std::string_view binaryData, bool isLittleEndian) noexcept try {
    BytesDataInput stream(binaryData, false, isLittleEndian);
    CompoundTag    result;
    result.deserialize(stream);
    return result;
} catch (...) { return std::nullopt; }

std::optional<CompoundTag>
CompoundTag::fromBinaryNbtWithHeader(std::string_view binaryData, bool isLittleEndian) noexcept try {
    BytesDataInput stream(binaryData, false, isLittleEndian);
    (void)stream.getInt();
    auto content = stream.getLongString();
    return fromBinaryNbt(content, isLittleEndian);
} catch (...) { return std::nullopt; }

std::string CompoundTag::toBinaryNbt(bool isLittleEndian) const noexcept {
    BytesDataOutput stream(isLittleEndian);
    serialize(stream);
    return stream.getAndReleaseData();
}

std::string CompoundTag::toBinaryNbtWithHeader(bool isLittleEndian) const noexcept {
    BytesDataOutput stream(isLittleEndian);
    int             storage_version = 0;
    if (contains("StorageVersion")) {
        auto version = at("StorageVersion");
        if (version.getType() == Type::Int) { storage_version = version; }
    }
    stream.writeInt(storage_version);
    auto binary = toBinaryNbt(isLittleEndian);
    stream.writeLongString(binary);
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


} // namespace nbt