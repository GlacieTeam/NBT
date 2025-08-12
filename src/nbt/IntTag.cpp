// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/IntTag.hpp>

namespace bedrock_protocol {

Tag::Type IntTag::getType() const { return Tag::Type::Int; }

bool IntTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Int) && (mData == static_cast<const IntTag&>(other).mData);
}

std::unique_ptr<Tag> IntTag::copy() const { return std::make_unique<IntTag>(mData); }

std::size_t IntTag::hash() const { return mData; }

void IntTag::write(BytesDataOutput& stream) const { stream.writeInt(mData); }

void IntTag::load(BytesDataInput& stream) { mData = stream.getInt(); }

void IntTag::write(BinaryStream& stream) const { stream.writeVarInt(mData); }

void IntTag::load(ReadOnlyBinaryStream& stream) { mData = stream.getVarInt(); }

int&       IntTag::data() { return mData; }
int const& IntTag::data() const { return mData; }

} // namespace bedrock_protocol