// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/Int64Tag.hpp"

namespace nbt {

bool Int64Tag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Int64) && (mStorage == static_cast<const Int64Tag&>(other).mStorage);
}

Int64Tag Int64Tag::operator+() const noexcept { return Int64Tag{+mStorage}; }

Tag::Type Int64Tag::getType() const { return Type::Int64; }

std::unique_ptr<Tag> Int64Tag::copy() const { return std::make_unique<Int64Tag>(mStorage); }

std::size_t Int64Tag::hash() const { return mStorage; }

void Int64Tag::write(BytesDataOutput& stream) const { stream.writeInt64(mStorage); }

void Int64Tag::load(BytesDataInput& stream) { mStorage = stream.getInt64(); }

void Int64Tag::write(bstream::BinaryStream& stream) const { stream.writeVarInt64(mStorage); }

void Int64Tag::load(bstream::ReadOnlyBinaryStream& stream) { mStorage = stream.getVarInt64(); }

} // namespace nbt