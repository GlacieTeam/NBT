// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/FloatTag.hpp>

namespace bedrock_protocol {

bool FloatTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Float) && (mStorage == static_cast<const FloatTag&>(other).mStorage);
}

std::unique_ptr<Tag> FloatTag::copy() const { return std::make_unique<FloatTag>(mStorage); }

std::size_t FloatTag::hash() const { return std::hash<float>{}(mStorage); }

void FloatTag::write(BytesDataOutput& stream) const { stream.writeFloat(mStorage); }

void FloatTag::load(BytesDataInput& stream) { mStorage = stream.getFloat(); }

void FloatTag::write(BinaryStream& stream) const { stream.writeFloat(mStorage); }

void FloatTag::load(ReadOnlyBinaryStream& stream) { mStorage = stream.getFloat(); }

} // namespace bedrock_protocol