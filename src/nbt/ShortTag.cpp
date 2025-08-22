// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/ShortTag.hpp"

namespace bedrock_protocol {

bool ShortTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Short) && (mStorage == static_cast<const ShortTag&>(other).mStorage);
}

Tag::Type ShortTag::getType() const { return Type::Short; }

std::unique_ptr<Tag> ShortTag::copy() const { return std::make_unique<ShortTag>(mStorage); }

std::size_t ShortTag::hash() const { return mStorage; }

void ShortTag::write(BytesDataOutput& stream) const { stream.writeShort(mStorage); }

void ShortTag::load(BytesDataInput& stream) { mStorage = stream.getShort(); }

void ShortTag::write(BinaryStream& stream) const { stream.writeSignedShort(mStorage); }

void ShortTag::load(ReadOnlyBinaryStream& stream) { mStorage = stream.getSignedShort(); }

} // namespace bedrock_protocol