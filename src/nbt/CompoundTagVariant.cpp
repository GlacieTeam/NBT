// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/CompoundTagVariant.hpp"
#include "nbt/CompoundTag.hpp"
#include "nbt/detail/SnbtDeserializer.hpp"
#include "nbt/detail/SnbtSerializer.hpp"
#include <nlohmann/json.hpp>

namespace bedrock_protocol {

CompoundTagVariant::CompoundTagVariant(Tag&& tag) { emplace(std::move(tag)); }

CompoundTagVariant::CompoundTagVariant(std::unique_ptr<Tag> tag) { emplace(std::move(*tag)); }

Tag& CompoundTagVariant::emplace(Tag&& tag) {
    switch (tag.getType()) {
    case Tag::Type::Byte:
        return emplace<ByteTag>(static_cast<ByteTag&&>(tag));
    case Tag::Type::Short:
        return emplace<ShortTag>(static_cast<ShortTag&&>(tag));
    case Tag::Type::Int:
        return emplace<IntTag>(static_cast<IntTag&&>(tag));
    case Tag::Type::Int64:
        return emplace<Int64Tag>(static_cast<Int64Tag&&>(tag));
    case Tag::Type::Float:
        return emplace<FloatTag>(static_cast<FloatTag&&>(tag));
    case Tag::Type::Double:
        return emplace<DoubleTag>(static_cast<DoubleTag&&>(tag));
    case Tag::Type::ByteArray:
        return emplace<ByteArrayTag>(static_cast<ByteArrayTag&&>(tag));
    case Tag::Type::String:
        return emplace<StringTag>(static_cast<StringTag&&>(tag));
    case Tag::Type::List:
        return emplace<ListTag>(static_cast<ListTag&&>(tag));
    case Tag::Type::Compound:
        return emplace<CompoundTag>(static_cast<CompoundTag&&>(tag));
    case Tag::Type::IntArray:
        return emplace<IntArrayTag>(static_cast<IntArrayTag&&>(tag));
    default:
        return emplace<EndTag>();
    }
}

const Tag* CompoundTagVariant::operator->() const { return get(); }

Tag* CompoundTagVariant::operator->() { return get(); }

const Tag* CompoundTagVariant::get() const {
    return std::visit([](auto&& arg) -> const Tag* { return &arg; }, mStorage);
}

Tag* CompoundTagVariant::get() {
    return std::visit([](auto&& arg) -> Tag* { return &arg; }, mStorage);
}

const Tag& CompoundTagVariant::operator*() const { return *get(); }

Tag& CompoundTagVariant::operator*() { return *get(); }

bool CompoundTagVariant::operator==(CompoundTagVariant const& other) const { return get()->equals(*other.get()); }

std::string CompoundTagVariant::toSnbt(SnbtFormat snbtFormat, uint8_t indent) const noexcept {
    return std::visit([&](auto& tag) { return detail::TypedToSnbt(tag, indent, snbtFormat); }, mStorage);
}

std::string CompoundTagVariant::toJson(uint8_t indent) const noexcept {
    try {
        return nlohmann::ordered_json::parse(toSnbt(SnbtFormat::Jsonify), nullptr, true, true).dump(indent);
    } catch (...) { return {}; }
}

void CompoundTagVariant::merge(CompoundTagVariant const& other, bool mergeList) {
    if (getType() == other.getType()) {
        if (is_object()) {
            return as<CompoundTag>().merge(other.as<CompoundTag>(), mergeList);
        } else if (is_array() && mergeList) {
            return as<ListTag>().merge(other.as<ListTag>());
        }
    }
    mStorage = other.mStorage;
}

std::optional<CompoundTagVariant>
CompoundTagVariant::parse(std::string_view snbt, std::optional<size_t> parsedLength) noexcept {
    auto begin{snbt.begin()};
    auto result = detail::parseSnbtValue(snbt);
    if (parsedLength) { *parsedLength = snbt.begin() - begin; }
    return result;
}

} // namespace bedrock_protocol
