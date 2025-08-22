// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/DoubleTag.hpp"

namespace bedrock_protocol {

bool DoubleTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Double) && (mStorage == static_cast<const DoubleTag&>(other).mStorage);
}

Tag::Type DoubleTag::getType() const { return Type::Double; }

std::unique_ptr<Tag> DoubleTag::copy() const { return std::make_unique<DoubleTag>(mStorage); }

std::size_t DoubleTag::hash() const { return std::hash<double>{}(mStorage); }

void DoubleTag::write(BytesDataOutput& stream) const { stream.writeDouble(mStorage); }

void DoubleTag::load(BytesDataInput& stream) { mStorage = stream.getDouble(); }

void DoubleTag::write(BinaryStream& stream) const { stream.writeDouble(mStorage); }

void DoubleTag::load(ReadOnlyBinaryStream& stream) { mStorage = stream.getDouble(); }

} // namespace bedrock_protocol