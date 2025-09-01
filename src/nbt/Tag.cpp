// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/Tag.hpp"
#include "nbt/CompoundTag.hpp"
#include "nbt/CompoundTagVariant.hpp"
#include "nbt/detail/SnbtSerializer.hpp"
#include <nlohmann/json.hpp>

namespace nbt {

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
    case Type::LongArray:
        return std::make_unique<LongArrayTag>();
    default:
        return std::make_unique<EndTag>();
    }
}

bool Tag::operator==(Tag const& other) const { return equals(other); }

std::string Tag::toSnbt(SnbtFormat snbtFormat, uint8_t indent) const noexcept {
    switch (getType()) {
    case Type::Byte:
        return detail::TypedToSnbt(as<ByteTag>(), indent, snbtFormat);
    case Type::Short:
        return detail::TypedToSnbt(as<ShortTag>(), indent, snbtFormat);
    case Type::Int:
        return detail::TypedToSnbt(as<IntTag>(), indent, snbtFormat);
    case Type::Int64:
        return detail::TypedToSnbt(as<Int64Tag>(), indent, snbtFormat);
    case Type::Float:
        return detail::TypedToSnbt(as<FloatTag>(), indent, snbtFormat);
    case Type::Double:
        return detail::TypedToSnbt(as<DoubleTag>(), indent, snbtFormat);
    case Type::ByteArray:
        return detail::TypedToSnbt(as<ByteArrayTag>(), indent, snbtFormat);
    case Type::String:
        return detail::TypedToSnbt(as<StringTag>(), indent, snbtFormat);
    case Type::List:
        return detail::TypedToSnbt(as<ListTag>(), indent, snbtFormat);
    case Type::Compound:
        return detail::TypedToSnbt(as<CompoundTag>(), indent, snbtFormat);
    case Type::IntArray:
        return detail::TypedToSnbt(as<IntArrayTag>(), indent, snbtFormat);
    default:
        return detail::TypedToSnbt(as<EndTag>(), indent, snbtFormat);
    }
}

std::string Tag::toJson(uint8_t indent) const noexcept {
    try {
        return nlohmann::ordered_json::parse(toSnbt(SnbtFormat::Jsonify), nullptr, true, true).dump(indent);
    } catch (...) { return {}; }
}

Tag& Tag::operator[](size_t index) {
    if (getType() == Type::List) { return as<ListTag>().operator[](index); }
    throw std::runtime_error("tag not hold an array");
}
Tag const& Tag::operator[](size_t index) const {
    if (getType() == Type::List) { return as<ListTag>().operator[](index); }
    throw std::runtime_error("tag not hold an array");
}

CompoundTagVariant& Tag::operator[](std::string_view index) {
    if (getType() == Type::Compound) { return as<CompoundTag>().operator[](index); }
    throw std::runtime_error("tag not hold an object");
}
CompoundTagVariant const& Tag::operator[](std::string_view index) const {
    if (getType() == Type::Compound) { return as<CompoundTag>().operator[](index); }
    throw std::runtime_error("tag not hold an object");
}

Tag::operator std::string&() {
    if (getType() == Type::String) { return as<StringTag>().storage(); }
    throw std::runtime_error("tag can not convert to a string");
}
Tag::operator std::string const&() const {
    if (getType() == Type::String) { return as<StringTag>().storage(); }
    throw std::runtime_error("tag can not convert to a string");
}

} // namespace nbt
