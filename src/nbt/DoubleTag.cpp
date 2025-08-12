// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/DoubleTag.hpp>

namespace bedrock_protocol {

Tag::Type DoubleTag::getType() const { return Tag::Type::Double; }

bool DoubleTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Double) && (mData == static_cast<const DoubleTag&>(other).mData);
}

std::unique_ptr<Tag> DoubleTag::copy() const { return std::make_unique<DoubleTag>(mData); }

std::size_t DoubleTag::hash() const { return std::hash<double>{}(mData); }

void DoubleTag::write(BytesDataOutput& stream) const { stream.writeDouble(mData); }

void DoubleTag::load(BytesDataInput& stream) { mData = stream.getDouble(); }

void DoubleTag::write(BinaryStream& stream) const { stream.writeDouble(mData); }

void DoubleTag::load(ReadOnlyBinaryStream& stream) { mData = stream.getDouble(); }

double&       DoubleTag::data() { return mData; }
double const& DoubleTag::data() const { return mData; }

} // namespace bedrock_protocol