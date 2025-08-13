// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/ByteTag.hpp>

namespace bedrock_protocol {

bool ByteTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Byte) && (mStorage == static_cast<const ByteTag&>(other).mStorage);
}

std::unique_ptr<Tag> ByteTag::copy() const { return std::make_unique<ByteTag>(mStorage); }

std::size_t ByteTag::hash() const { return mStorage; }

void ByteTag::write(BytesDataOutput& stream) const { stream.writeByte(mStorage); }

void ByteTag::load(BytesDataInput& stream) { mStorage = stream.getByte(); }

void ByteTag::write(BinaryStream& stream) const { stream.writeUnsignedChar(mStorage); }

void ByteTag::load(ReadOnlyBinaryStream& stream) { mStorage = stream.getUnsignedChar(); }

} // namespace bedrock_protocol