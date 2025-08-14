// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/IntTag.hpp"

namespace bedrock_protocol {

bool IntTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Int) && (mStorage == static_cast<const IntTag&>(other).mStorage);
}

std::unique_ptr<Tag> IntTag::copy() const { return std::make_unique<IntTag>(mStorage); }

std::size_t IntTag::hash() const { return mStorage; }

void IntTag::write(BytesDataOutput& stream) const { stream.writeInt(mStorage); }

void IntTag::load(BytesDataInput& stream) { mStorage = stream.getInt(); }

void IntTag::write(BinaryStream& stream) const { stream.writeVarInt(mStorage); }

void IntTag::load(ReadOnlyBinaryStream& stream) { mStorage = stream.getVarInt(); }

} // namespace bedrock_protocol