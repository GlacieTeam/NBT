// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/StringTag.hpp>

namespace bedrock_protocol {

StringTag::StringTag(std::string_view str) : mData(str) {}

StringTag::StringTag(std::string str) : mData(std::move(str)) {}

Tag::Type StringTag::getType() const { return Tag::Type::String; }

bool StringTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::String) && (mData == static_cast<const StringTag&>(other).mData);
}

std::unique_ptr<Tag> StringTag::copy() const { return std::make_unique<StringTag>(mData); }

std::size_t StringTag::hash() const { return std::hash<std::string>{}(mData); }

void StringTag::write(BytesDataOutput& stream) const { stream.writeString(mData); }

void StringTag::load(BytesDataInput& stream) { mData = stream.getString(); }

void StringTag::write(BinaryStream& stream) const { stream.writeString(mData); }

void StringTag::load(ReadOnlyBinaryStream& stream) { stream.getString(mData); }

std::string&       StringTag::data() { return mData; }
std::string const& StringTag::data() const { return mData; }
std::string_view   StringTag::view() const { return mData; }

} // namespace bedrock_protocol