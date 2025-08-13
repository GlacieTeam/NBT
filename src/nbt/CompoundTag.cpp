// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <algorithm>
#include <nbt/ByteArrayTag.hpp>
#include <nbt/ByteTag.hpp>
#include <nbt/CompoundTag.hpp>
#include <nbt/CompoundTagVariant.hpp>
#include <nbt/DoubleTag.hpp>
#include <nbt/EndTag.hpp>
#include <nbt/FloatTag.hpp>
#include <nbt/Int64Tag.hpp>
#include <nbt/IntArrayTag.hpp>
#include <nbt/IntTag.hpp>
#include <nbt/ListTag.hpp>
#include <nbt/ShortTag.hpp>
#include <nbt/StringTag.hpp>

namespace bedrock_protocol {

bool CompoundTag::equals(Tag const& other) const {
    if (other.getType() != Tag::Type::Compound) { return false; }
    const auto& other_tag = static_cast<const CompoundTag&>(other);
    return std::all_of(mTagMap.begin(), mTagMap.end(), [&](const auto& kv) {
        const auto* tag = other_tag.get(kv.first);
        return tag && kv.second.get()->equals(*tag);
    });
}

std::unique_ptr<Tag> CompoundTag::copy() const { return clone(); }

std::size_t CompoundTag::hash() const {
    size_t hash      = 0x811c9dc5;
    size_t fnv_prime = 0x01000193;
    auto   binary    = toBinaryNbt();
    for (uint8_t byte : binary) {
        hash ^= byte;
        hash  = hash * fnv_prime;
    }
    return hash;
}

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
        if (type != Tag::Type::End) {
            stream.writeString(key);
            tag->write(stream);
        }
    }
    stream.writeByte((uint8_t)Type::End);
}

void CompoundTag::load(BytesDataInput& stream) {
    mTagMap.clear();
    auto type = Tag::Type::End;
    do {
        type = Tag::Type(stream.getByte());
        if (type != Tag::Type::End) {
            auto key = stream.getString();
            auto tag = Tag::newTag(type);
            tag->load(stream);
            put(key, std::move(tag));
        }
    } while (type != Tag::Type::End);
}

void CompoundTag::write(BinaryStream& stream) const {
    for (const auto& [key, tag] : mTagMap) {
        auto type = tag->getType();
        stream.writeUnsignedChar((uint8_t)type);
        if (type != Tag::Type::End) {
            stream.writeString(key);
            tag->write(stream);
        }
    }
    stream.writeUnsignedChar((uint8_t)Type::End);
}

void CompoundTag::load(ReadOnlyBinaryStream& stream) {
    mTagMap.clear();
    auto type = Tag::Type::End;
    do {
        type = Tag::Type(stream.getUnsignedChar());
        if (type != Tag::Type::End) {
            auto key = stream.getString();
            auto tag = Tag::newTag(type);
            tag->load(stream);
            put(key, std::move(tag));
        }
    } while (type != Tag::Type::End);
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
        if (tag->getType() == Tag::Type::Byte) { return static_cast<const ByteTag*>(tag); }
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
        if (tag->getType() == Tag::Type::Short) { return static_cast<const ShortTag*>(tag); }
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
        if (tag->getType() == Tag::Type::Int) { return static_cast<const IntTag*>(tag); }
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
        if (tag->getType() == Tag::Type::Int64) { return static_cast<const Int64Tag*>(tag); }
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
        if (tag->getType() == Tag::Type::Float) { return static_cast<const FloatTag*>(tag); }
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
        if (tag->getType() == Tag::Type::Double) { return static_cast<const DoubleTag*>(tag); }
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
        if (tag->getType() == Tag::Type::String) { return static_cast<const StringTag*>(tag); }
    }
    return nullptr;
}

StringTag* CompoundTag::getString(std::string_view key) {
    if (auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::String) { return static_cast<StringTag*>(tag); }
    }
    return nullptr;
}

const ByteArrayTag* CompoundTag::getByteArray(std::string_view key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::ByteArray) { return static_cast<const ByteArrayTag*>(tag); }
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
        if (tag->getType() == Tag::Type::IntArray) { return static_cast<const IntArrayTag*>(tag); }
    }
    return nullptr;
}

IntArrayTag* CompoundTag::getIntArray(std::string_view key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::IntArray) { return static_cast<IntArrayTag*>(tag); }
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

bool CompoundTag::isEmpty() const { return mTagMap.empty(); }

bool CompoundTag::remove(std::string_view name) {
    auto it = mTagMap.find(name);
    if (it == mTagMap.end()) { return false; }
    mTagMap.erase(it);
    return true;
}

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

CompoundTag::TagMap&       CompoundTag::items() { return mTagMap; }
CompoundTag::TagMap const& CompoundTag::items() const { return mTagMap; }

void CompoundTag::serialize(BinaryStream& stream) const {
    stream.writeUnsignedChar((uint8_t)Tag::Type::Compound);
    stream.writeString("");
    write(stream);
}

void CompoundTag::serialize(BytesDataOutput& stream) const {
    stream.writeByte((uint8_t)Tag::Type::Compound);
    stream.writeString("");
    write(stream);
}

void CompoundTag::deserialize(ReadOnlyBinaryStream& stream) {
    (void)stream.getByte();
    (void)stream.getString();
    load(stream);
}

void CompoundTag::deserialize(BytesDataInput& stream) {
    (void)stream.getByte();
    (void)stream.getString();
    load(stream);
}

CompoundTag CompoundTag::fromBinaryNbt(std::string_view binaryData, bool isLittleEndian) {
    BytesDataInput stream(binaryData, false, isLittleEndian);
    CompoundTag    result;
    result.deserialize(stream);
    return result;
}

std::string CompoundTag::toBinaryNbt(bool isLittleEndian) const {
    BytesDataOutput stream(isLittleEndian);
    serialize(stream);
    return stream.getAndReleaseData();
}

CompoundTag CompoundTag::fromNetworkNbt(std::string_view binaryData) {
    ReadOnlyBinaryStream stream(binaryData, false);
    CompoundTag          result;
    result.deserialize(stream);
    return result;
}

std::string CompoundTag::toNetworkNbt() const {
    BinaryStream stream;
    serialize(stream);
    return stream.getAndReleaseData();
}

CompoundTagVariant&       CompoundTag::at(std::string_view index) { return mTagMap[std::string(index)]; }
CompoundTagVariant const& CompoundTag::at(std::string_view index) const {
    if (contains(index)) { return mTagMap.at(std::string(index)); }
    throw std::out_of_range("invalid nbt key");
}

CompoundTagVariant&       CompoundTag::operator[](std::string_view index) { return at(index); }
CompoundTagVariant const& CompoundTag::operator[](std::string_view index) const { return at(index); }

size_t CompoundTag::size() const { return mTagMap.size(); }

} // namespace bedrock_protocol