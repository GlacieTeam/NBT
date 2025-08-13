// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/ByteArrayTag.hpp>

namespace bedrock_protocol {

ByteArrayTag::ByteArrayTag(std::vector<uint8_t> arr) : mStorage(std::move(arr)) {}

ByteArrayTag::ByteArrayTag(std::initializer_list<uint8_t> val) : mStorage(val) {}

ByteArrayTag::operator std::vector<uint8_t> const&() const { return mStorage; }
ByteArrayTag::operator std::vector<uint8_t>&() { return mStorage; }

bool ByteArrayTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::ByteArray) && (mStorage == static_cast<const ByteArrayTag&>(other).mStorage);
}

std::unique_ptr<Tag> ByteArrayTag::copy() const { return std::make_unique<ByteArrayTag>(mStorage); }

std::size_t ByteArrayTag::hash() const {
    std::size_t hash = 0;
    for (uint8_t value : mStorage) {
        std::size_t element_hash  = std::hash<uint8_t>{}(value);
        hash                     ^= element_hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

void ByteArrayTag::write(BytesDataOutput& stream) const {
    stream.writeInt((int)mStorage.size());
    stream.writeBytes(mStorage.data(), mStorage.size());
}

void ByteArrayTag::load(BytesDataInput& stream) {
    auto size = stream.getInt();
    mStorage.resize(size);
    stream.getBytes(mStorage.data(), size);
}

void ByteArrayTag::write(BinaryStream& stream) const {
    stream.writeVarInt((int)mStorage.size());
    for (auto& data : mStorage) { stream.writeUnsignedChar(data); }
}

void ByteArrayTag::load(ReadOnlyBinaryStream& stream) {
    auto size = stream.getVarInt();
    for (auto i = 0; i < size; i++) { mStorage.emplace_back(stream.getUnsignedChar()); }
}

std::vector<uint8_t>&       ByteArrayTag::storage() { return mStorage; }
std::vector<uint8_t> const& ByteArrayTag::storage() const { return mStorage; }

size_t ByteArrayTag::size() const { return mStorage.size(); }

} // namespace bedrock_protocol