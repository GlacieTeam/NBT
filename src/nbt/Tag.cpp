// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/Tag.hpp"
#include "nbt/CompoundTag.hpp"
#include "nbt/CompoundTagVariant.hpp"

namespace bedrock_protocol {

std::unique_ptr<Tag> Tag::newTag(Type type) {
    switch (type) {
    case Type::Byte:
        return std::make_unique<ByteTag>();
    case Type::Short:
        return std::make_unique<ShortTag>();
    case Type::Int:
        return std::make_unique<IntTag>();
    case Type::Int64:
        return std::make_unique<Int64Tag>();
    case Type::Float:
        return std::make_unique<FloatTag>();
    case Type::Double:
        return std::make_unique<DoubleTag>();
    case Type::ByteArray:
        return std::make_unique<ByteArrayTag>();
    case Type::String:
        return std::make_unique<StringTag>();
    case Type::List:
        return std::make_unique<ListTag>();
    case Type::Compound:
        return std::make_unique<CompoundTag>();
    case Type::IntArray:
        return std::make_unique<IntArrayTag>();
    default:
        return std::make_unique<EndTag>();
    }
}

bool Tag::operator==(Tag const& other) const { return equals(other); }

} // namespace bedrock_protocol
