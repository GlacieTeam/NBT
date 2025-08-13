// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/IntArrayTag.hpp>

namespace bedrock_protocol {

IntArrayTag::IntArrayTag(std::vector<int> arr) : mStorage(std::move(arr)) {}

IntArrayTag::IntArrayTag(std::initializer_list<int> val) : mStorage(val) {}

IntArrayTag::operator std::vector<int> const&() const { return mStorage; }
IntArrayTag::operator std::vector<int>&() { return mStorage; }

bool IntArrayTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::IntArray) && (mStorage == static_cast<const IntArrayTag&>(other).mStorage);
}

std::unique_ptr<Tag> IntArrayTag::copy() const { return std::make_unique<IntArrayTag>(mStorage); }

std::size_t IntArrayTag::hash() const {
    std::size_t hash = 0;
    for (int value : mStorage) {
        std::size_t element_hash  = std::hash<int>{}(value);
        hash                     ^= element_hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

void IntArrayTag::write(BytesDataOutput& stream) const {
    stream.writeInt((int)mStorage.size());
    for (const int i : mStorage) { stream.writeInt(i); }
}

void IntArrayTag::load(BytesDataInput& stream) {
    auto size = stream.getInt();
    mStorage.clear();
    for (int i = 0; i < size; ++i) { mStorage.push_back(stream.getInt()); }
}

void IntArrayTag::write(BinaryStream& stream) const {
    stream.writeVarInt((int)mStorage.size());
    for (auto& data : mStorage) { stream.writeVarInt(data); }
}

void IntArrayTag::load(ReadOnlyBinaryStream& stream) {
    auto size = stream.getVarInt();
    for (auto i = 0; i < size; i++) { mStorage.emplace_back(stream.getVarInt()); }
}

std::vector<int>&       IntArrayTag::storage() { return mStorage; }
std::vector<int> const& IntArrayTag::storage() const { return mStorage; }

size_t IntArrayTag::size() const { return mStorage.size(); }

} // namespace bedrock_protocol