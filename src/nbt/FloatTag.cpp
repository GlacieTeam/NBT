// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/FloatTag.hpp>

namespace bedrock_protocol {

FloatTag::FloatTag(float data) : mData(data) {}

Tag::Type FloatTag::getType() const { return Tag::Type::Float; }

bool FloatTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Float) && (mData == static_cast<const FloatTag&>(other).mData);
}

std::unique_ptr<Tag> FloatTag::copy() const { return std::make_unique<FloatTag>(mData); }

std::size_t FloatTag::hash() const { return std::hash<float>{}(mData); }

void FloatTag::write(BytesDataOutput& stream) const { stream.writeFloat(mData); }

void FloatTag::load(BytesDataInput& stream) { mData = stream.getFloat(); }

void FloatTag::write(BinaryStream& stream) const { stream.writeFloat(mData); }

void FloatTag::load(ReadOnlyBinaryStream& stream) { mData = stream.getFloat(); }

float&       FloatTag::data() { return mData; }
float const& FloatTag::data() const { return mData; }

} // namespace bedrock_protocol