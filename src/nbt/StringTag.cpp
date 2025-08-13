// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include <nbt/StringTag.hpp>

namespace bedrock_protocol {

StringTag::StringTag(std::string_view str) : mStorage(str) {}

StringTag::StringTag(std::string str) : mStorage(std::move(str)) {}

bool StringTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::String) && (mStorage == static_cast<const StringTag&>(other).mStorage);
}

std::unique_ptr<Tag> StringTag::copy() const { return std::make_unique<StringTag>(mStorage); }

std::size_t StringTag::hash() const { return std::hash<std::string>{}(mStorage); }

void StringTag::write(BytesDataOutput& stream) const { stream.writeString(mStorage); }

void StringTag::load(BytesDataInput& stream) { mStorage = stream.getString(); }

void StringTag::write(BinaryStream& stream) const { stream.writeString(mStorage); }

void StringTag::load(ReadOnlyBinaryStream& stream) { stream.getString(mStorage); }

std::string&       StringTag::storage() { return mStorage; }
std::string const& StringTag::storage() const { return mStorage; }
std::string_view   StringTag::view() const { return mStorage; }

} // namespace bedrock_protocol