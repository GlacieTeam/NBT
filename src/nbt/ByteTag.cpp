// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/ByteTag.hpp>

namespace bedrock_protocol {

ByteTag::ByteTag(uint8_t data) : mData(data) {}

Tag::Type ByteTag::getType() const { return Tag::Type::Byte; }

bool ByteTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Byte) && (mData == static_cast<const ByteTag&>(other).mData);
}

std::unique_ptr<Tag> ByteTag::copy() const { return std::make_unique<ByteTag>(mData); }

std::size_t ByteTag::hash() const { return mData; }

void ByteTag::write(BytesDataOutput& stream) const { stream.writeByte(mData); }

void ByteTag::load(BytesDataInput& stream) { mData = stream.getByte(); }

void ByteTag::write(BinaryStream& stream) const { stream.writeUnsignedChar(mData); }

void ByteTag::load(ReadOnlyBinaryStream& stream) { mData = stream.getUnsignedChar(); }

uint8_t&       ByteTag::data() { return mData; }
uint8_t const& ByteTag::data() const { return mData; }

} // namespace bedrock_protocol