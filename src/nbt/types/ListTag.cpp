// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/types/ListTag.hpp"
#include "nbt/types/CompoundTagVariant.hpp"
#include <algorithm>

namespace nbt {

ListTag::ListTag(std::initializer_list<CompoundTagVariant> tags) : mStorage(tags) {
    if (!mStorage.empty()) { mType = mStorage.front()->getType(); }
}

ListTag::ListTag(std::vector<nbt::CompoundTagVariant> const& data) : mStorage(data) {
    if (!mStorage.empty()) { mType = mStorage.front()->getType(); }
}

ListTag::ListTag(std::vector<CompoundTagVariant>&& tags) : mStorage(std::move(tags)) {
    if (!mStorage.empty()) { mType = mStorage.front()->getType(); }
}

ListTag::ListTag(std::vector<std::unique_ptr<Tag>> const& tags) {
    for (auto& tag : tags) { mStorage.emplace_back(*tag); }
    if (!mStorage.empty()) { mType = mStorage.front()->getType(); }
}

ListTag::ListTag(std::vector<std::unique_ptr<Tag>>&& tags) {
    for (auto&& tag : tags) { mStorage.push_back(std::move(tag)); }
    if (!mStorage.empty()) { mType = mStorage.front()->getType(); }
}

bool ListTag::equals(const Tag& other) const {
    return (other.getType() == Type::List) && (mStorage == static_cast<const ListTag&>(other).mStorage);
}

Tag::Type ListTag::getType() const { return Type::List; }

std::unique_ptr<Tag> ListTag::copy() const { return clone(); }

std::size_t ListTag::hash() const {
    std::size_t hash = 0;
    for (auto& value : mStorage) {
        std::size_t element_hash  = value.get()->hash();
        hash                     ^= element_hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

std::unique_ptr<ListTag> ListTag::clone() const { return std::make_unique<ListTag>(mStorage); }

void ListTag::write(io::BytesDataOutput& stream) const {
    stream.writeByte((uint8_t)mType);
    stream.writeInt(static_cast<int>(mStorage.size()));
    for (const auto& data : mStorage) { data->write(stream); }
}

void ListTag::load(io::BytesDataInput& stream) {
    mType     = static_cast<Type>(stream.getByte());
    auto size = stream.getInt();
    for (int i = 0; i < size; i++) {
        if (auto tag = Tag::newTag(mType)) {
            tag->load(stream);
            mStorage.emplace_back(std::move(tag));
        }
    }
}

void ListTag::write(bstream::BinaryStream& stream) const {
    stream.writeByte(static_cast<std::byte>(mType));
    stream.writeVarInt(static_cast<int>(mStorage.size()));
    for (const auto& data : mStorage) { data->write(stream); }
}

void ListTag::load(bstream::ReadOnlyBinaryStream& stream) {
    mType     = static_cast<Type>(stream.getByte());
    auto size = stream.getVarInt();
    for (int i = 0; i < size; i++) {
        if (auto tag = Tag::newTag(mType)) {
            tag->load(stream);
            mStorage.emplace_back(std::move(tag));
        }
    }
}

void ListTag::merge(ListTag const& other) {
    if (other.empty()) { return; }
    if (mType == other.mType) {
        for (auto const& val : other.mStorage) {
            bool isEqual = false;
            for (const auto& tag : mStorage) {
                if (tag->equals(*val)) {
                    isEqual = true;
                    break;
                }
            }
            if (!isEqual) { mStorage.emplace_back(val); }
        }
    } else {
        mType = other.mType;
        mStorage.clear();
        for (const auto& data : other.mStorage) { mStorage.emplace_back(data); }
    }
}

void ListTag::push_back(std::unique_ptr<Tag>&& tag) {
    if (mStorage.empty()) { mType = tag->getType(); }
    mStorage.emplace_back(std::move(tag));
}

void ListTag::push_back(Tag const& tag) {
    if (mStorage.empty()) { mType = tag.getType(); }
    mStorage.emplace_back(tag.copy());
}

void ListTag::push_back(CompoundTagVariant const& val) {
    if (mStorage.empty()) { mType = val.getType(); }
    mStorage.push_back(val);
}

void ListTag::push_back(CompoundTagVariant&& val) {
    if (mStorage.empty()) { mType = val.getType(); }
    mStorage.push_back(std::move(val));
}

void ListTag::reserve(size_t size) { mStorage.reserve(size); }

bool ListTag::remove(size_t index) {
    if (index < mStorage.size()) {
        mStorage.erase(mStorage.begin() + static_cast<decltype(mStorage)::difference_type>(index));
        return true;
    }
    return false;
}

bool ListTag::remove(size_t startIndex, size_t endIndex) {
    if (startIndex < endIndex && endIndex < mStorage.size()) {
        mStorage.erase(
            mStorage.begin() + static_cast<decltype(mStorage)::difference_type>(startIndex),
            mStorage.begin() + static_cast<decltype(mStorage)::difference_type>(endIndex)
        );
        return true;
    }
    return false;
}

void ListTag::clear() noexcept { mStorage.clear(); }

ListTag::TagList&       ListTag::storage() noexcept { return mStorage; }
ListTag::TagList const& ListTag::storage() const noexcept { return mStorage; }

size_t ListTag::size() const noexcept { return mStorage.size(); }
bool   ListTag::empty() const { return mStorage.empty(); }

Tag::Type ListTag::getElementType() const { return mType; }

CompoundTagVariant&       ListTag::operator[](size_t index) noexcept { return mStorage[index]; }
CompoundTagVariant const& ListTag::operator[](size_t index) const noexcept { return mStorage[index]; }

CompoundTagVariant&       ListTag::at(size_t index) { return mStorage.at(index); }
CompoundTagVariant const& ListTag::at(size_t index) const { return mStorage.at(index); }

ListTag::iterator ListTag::begin() noexcept { return mStorage.begin(); }
ListTag::iterator ListTag::end() noexcept { return mStorage.end(); }

ListTag::reverse_iterator ListTag::rbegin() noexcept { return mStorage.rbegin(); }
ListTag::reverse_iterator ListTag::rend() noexcept { return mStorage.rend(); }

ListTag::const_iterator ListTag::begin() const noexcept { return cbegin(); }
ListTag::const_iterator ListTag::end() const noexcept { return cend(); }

ListTag::const_iterator ListTag::cbegin() const noexcept { return mStorage.cbegin(); }
ListTag::const_iterator ListTag::cend() const noexcept { return mStorage.cend(); }

ListTag::const_reverse_iterator ListTag::crbegin() const noexcept { return mStorage.crbegin(); }
ListTag::const_reverse_iterator ListTag::crend() const noexcept { return mStorage.crend(); }

ListTag::iterator ListTag::erase(const_iterator where) noexcept { return mStorage.erase(where); }
ListTag::iterator ListTag::erase(const_iterator first, const_iterator last) noexcept {
    return mStorage.erase(first, last);
}

bool ListTag::set(size_t index, Tag const& tag) {
    if (index < mStorage.size()) {
        mStorage[index] = tag.copy();
        return true;
    }
    return false;
}

bool ListTag::set(size_t index, std::unique_ptr<Tag>&& tag) {
    if (index < mStorage.size()) {
        mStorage[index] = std::move(tag);
        return true;
    }
    return false;
}

bool ListTag::set(size_t index, CompoundTagVariant const& tag) {
    if (index < mStorage.size()) {
        mStorage[index] = tag;
        return true;
    }
    return false;
}

bool ListTag::set(size_t index, CompoundTagVariant&& tag) {
    if (index < mStorage.size()) {
        mStorage[index] = std::move(tag);
        return true;
    }
    return false;
}

} // namespace nbt