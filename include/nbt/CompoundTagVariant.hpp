// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <nbt/ByteArrayTag.hpp>
#include <nbt/ByteTag.hpp>
#include <nbt/CompoundTag.hpp>
#include <nbt/DoubleTag.hpp>
#include <nbt/EndTag.hpp>
#include <nbt/FloatTag.hpp>
#include <nbt/Int64Tag.hpp>
#include <nbt/IntArrayTag.hpp>
#include <nbt/IntTag.hpp>
#include <nbt/ListTag.hpp>
#include <nbt/ShortTag.hpp>
#include <nbt/StringTag.hpp>
#include <nbt/Tag.hpp>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>

namespace bedrock_protocol {

class CompoundTagVariant {
public:
    using TagVariant = std::variant<
        EndTag,
        ByteTag,
        ShortTag,
        IntTag,
        Int64Tag,
        FloatTag,
        DoubleTag,
        ByteArrayTag,
        StringTag,
        ListTag,
        CompoundTag,
        IntArrayTag>;

public:
    TagVariant mStorage;

public:
    template <bool Const>
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;
        using value_type        = Tag;
        using reference         = std::conditional_t<Const, value_type const, value_type>&;
        using pointer           = std::add_pointer_t<reference>;

        std::conditional_t<
            Const,
            std::variant<CompoundTagVariant const*, CompoundTag::const_iterator, ListTag::const_iterator>,
            std::variant<CompoundTagVariant*, CompoundTag::iterator, ListTag::iterator>>
            iter;

        [[nodiscard]] static Iterator makeBegin(auto& var) noexcept {
            Iterator res;
            switch (var.index()) {
            case Tag::Type::List:
                res.iter.template emplace<2>(var.template as<ListTag>().begin());
                break;
            case Tag::Type::Compound:
                res.iter.template emplace<1>(var.template as<CompoundTag>().begin());
                break;
            case Tag::Type::End:
                res.iter.template emplace<0>(std::addressof(var) + 1);
                break;
            default:
                res.iter.template emplace<0>(std::addressof(var));
            }
            return res;
        }

        [[nodiscard]] static Iterator makeEnd(auto& var) noexcept {
            Iterator res;
            switch (var.index()) {
            case Tag::Type::List:
                res.iter.template emplace<2>(var.template as<ListTag>().end());
                break;
            case Tag::Type::Compound:
                res.iter.template emplace<1>(var.template as<CompoundTag>().end());
                break;
            default:
                res.iter.template emplace<0>(std::addressof(var) + 1);
            }
            return res;
        }

        [[nodiscard]] reference operator*() const noexcept {
            switch (iter.index()) {
            case 0:
                return *std::get<0>(iter)->get();
            case 1:
                return *std::get<1>(iter)->second.get();
            case 2:
                return *std::get<2>(iter)->get();
            default:
                std::unreachable();
            }
            std::unreachable();
        }

        [[nodiscard]] pointer operator->() const noexcept { return std::addressof(**this); }

        Iterator& operator++() noexcept {
            std::visit([](auto& val) { ++val; }, iter);
            return *this;
        }

        Iterator operator++(int) noexcept {
            Iterator tmp = *this;
            ++*this;
            return tmp;
        }

        Iterator& operator--() noexcept {
            std::visit([](auto& val) { --val; }, iter);
            return *this;
        }

        Iterator operator--(int) noexcept {
            Iterator tmp = *this;
            --*this;
            return tmp;
        }

        [[nodiscard]] bool operator==(Iterator const& r) const noexcept { return this->iter == r.iter; }
    };

public:
    [[nodiscard]] CompoundTagVariant(Tag&& tag);
    [[nodiscard]] CompoundTagVariant(std::unique_ptr<Tag> tag);

    [[nodiscard]] constexpr CompoundTagVariant()                          = default;
    [[nodiscard]] constexpr CompoundTagVariant(CompoundTagVariant&&)      = default;
    [[nodiscard]] constexpr CompoundTagVariant(CompoundTagVariant const&) = default;

    CompoundTagVariant& operator=(CompoundTagVariant&&)      = default;
    CompoundTagVariant& operator=(CompoundTagVariant const&) = default;

    template <class T>
        requires(requires(T o) { mStorage = std::move(o); })
    constexpr CompoundTagVariant& operator=(T other) {
        mStorage = std::move(other);
        return *this;
    }

public:
    template <typename T, typename... Args>
    T& emplace(Args&&... args) {
        mStorage = T(std::forward<Args>(args)...);
        return std::get<T>(mStorage);
    }

    Tag& emplace(Tag&& tag);

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr T& as() noexcept {
        return static_cast<T&>(*get());
    }

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr T const& as() const noexcept {
        return static_cast<T const&>(*get());
    }

    [[nodiscard]] const Tag* operator->() const;
    [[nodiscard]] Tag*       operator->();

    [[nodiscard]] const Tag* get() const;
    [[nodiscard]] Tag*       get();

    [[nodiscard]] const Tag& operator*() const;
    [[nodiscard]] Tag&       operator*();

    [[nodiscard]] bool operator==(CompoundTagVariant const& other) const;

public:
    using iterator       = Iterator<false>;
    using const_iterator = Iterator<true>;

    [[nodiscard]] constexpr CompoundTagVariant(std::nullptr_t) {}

    [[nodiscard]] CompoundTagVariant(TagVariant tag) : mStorage(std::move(tag)) {}

    template <class T, class... Args>
    [[nodiscard]] constexpr CompoundTagVariant(std::in_place_type_t<T>, Args&&... args)
    : mStorage(std::in_place_type<T>, std::forward<Args>(args)...) {}

    [[nodiscard]] constexpr CompoundTagVariant(std::initializer_list<CompoundTag::TagMap::value_type> tagPairs)
    : mStorage(std::in_place_type<CompoundTag>, tagPairs) {}

    [[nodiscard]] CompoundTagVariant(Tag const& tag) : CompoundTagVariant(tag.copy()) {}
    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr CompoundTagVariant(T tag) : mStorage(std::move(tag)) {}
    template <std::integral T>
    [[nodiscard]] constexpr CompoundTagVariant(T integer) {
        constexpr size_t size = sizeof(T);
        if constexpr (size == 1) {
            mStorage = ByteTag{integer};
        } else if constexpr (size == 2) {
            mStorage = ShortTag{integer};
        } else if constexpr (size == 4) {
            mStorage = IntTag{integer};
        } else {
            mStorage = Int64Tag{integer};
        }
    }
    [[nodiscard]] CompoundTagVariant(std::byte b) : mStorage(ByteTag{b}) {}

    [[nodiscard]] CompoundTagVariant(float f) : mStorage(FloatTag{f}) {}

    [[nodiscard]] CompoundTagVariant(double d) : mStorage(DoubleTag{d}) {}

    [[nodiscard]] constexpr CompoundTagVariant(std::string s) : mStorage(std::in_place_type<StringTag>, std::move(s)) {}

    [[nodiscard]] constexpr CompoundTagVariant(std::string_view s)
    : mStorage(std::in_place_type<StringTag>, std::string(s)) {}

    template <size_t N>
    [[nodiscard]] constexpr CompoundTagVariant(char const (&str)[N])
    : CompoundTagVariant(std::string_view{str, N - 1}) {}

    [[nodiscard]] iterator       begin() noexcept { return iterator::makeBegin(*this); }
    [[nodiscard]] const_iterator begin() const noexcept { return cbegin(); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator::makeBegin(*this); }

    [[nodiscard]] iterator       end() noexcept { return iterator::makeEnd(*this); }
    [[nodiscard]] const_iterator end() const noexcept { return cend(); }
    [[nodiscard]] const_iterator cend() const noexcept { return const_iterator::makeEnd(*this); }

    [[nodiscard]] constexpr Tag::Type index() const noexcept { return Tag::Type(mStorage.index()); }
    [[nodiscard]] constexpr Tag::Type getType() const noexcept { return index(); }

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr bool hold() const noexcept {
        return std::holds_alternative<T>(mStorage);
    }
    [[nodiscard]] constexpr bool hold(Tag::Type type) const noexcept { return getType() == type; }

    [[nodiscard]] constexpr bool is_array() const noexcept { return hold(Tag::Type::List); }
    [[nodiscard]] constexpr bool is_binary() const noexcept {
        return hold(Tag::Type::ByteArray) || hold(Tag::Type::IntArray);
    }
    [[nodiscard]] constexpr bool is_boolean() const noexcept { return hold(Tag::Type::Byte); }
    [[nodiscard]] constexpr bool is_null() const noexcept { return hold(Tag::Type::End); }
    [[nodiscard]] constexpr bool is_number_float() const noexcept {
        return hold(Tag::Type::Float) || hold(Tag::Type::Double);
    }
    [[nodiscard]] constexpr bool is_number_integer() const noexcept {
        return hold(Tag::Type::Byte) || hold(Tag::Type::Short) || hold(Tag::Type::Int) || hold(Tag::Type::Int64);
    }
    [[nodiscard]] constexpr bool is_object() const noexcept { return hold(Tag::Type::Compound); }
    [[nodiscard]] constexpr bool is_string() const noexcept { return hold(Tag::Type::String); }
    [[nodiscard]] constexpr bool is_number() const noexcept { return is_number_float() || is_number_integer(); }
    [[nodiscard]] constexpr bool is_primitive() const noexcept {
        return is_null() || is_string() || is_number() || is_binary();
    }
    [[nodiscard]] constexpr bool is_structured() const noexcept { return is_array() || is_object(); }

    [[nodiscard]] CompoundTag::TagMap const& items() const { return as<CompoundTag>().items(); }
    [[nodiscard]] CompoundTag::TagMap&       items() { return as<CompoundTag>().items(); }

    [[nodiscard]] bool contains(std::string_view key) const noexcept {
        if (is_object()) { return as<CompoundTag>().contains(std::string(key)); }
        return false;
    }

    [[nodiscard]] bool contains(std::string_view key, Tag::Type type) const noexcept {
        if (is_object()) { return as<CompoundTag>().contains(std::string(key), type); }
        return false;
    }

    [[nodiscard]] constexpr size_t size() const noexcept {
        switch (index()) {
        case Tag::Type::Byte:
        case Tag::Type::Short:
        case Tag::Type::Int:
        case Tag::Type::Int64:
        case Tag::Type::Float:
        case Tag::Type::Double:
        case Tag::Type::String:
            return 1;
        case Tag::Type::List:
            return as<ListTag>().size();
        case Tag::Type::Compound:
            return as<CompoundTag>().size();
        case Tag::Type::IntArray:
            return as<IntArrayTag>().size();
        case Tag::Type::ByteArray:
            return as<ByteArrayTag>().size();
        case Tag::Type::End:
            return 0;
        default:
            std::unreachable();
        }
    }

    [[nodiscard]] Tag& operator[](size_t index) {
        if (hold(Tag::Type::List)) {
            return as<ListTag>()[index];
        } else {
            throw std::runtime_error("tag not hold an array");
        }
    }

    [[nodiscard]] Tag const& operator[](size_t index) const {
        if (hold(Tag::Type::List)) {
            return as<ListTag>()[index];
        } else {
            throw std::runtime_error("tag not hold an array");
        }
    }

    [[nodiscard]] CompoundTagVariant& operator[](std::string_view index) {
        if (is_null()) { mStorage = CompoundTag{}; }
        if (!hold(Tag::Type::Compound)) { throw std::runtime_error("tag not hold an object"); }
        return as<CompoundTag>()[index];
    }

    [[nodiscard]] CompoundTagVariant const& operator[](std::string_view index) const {
        if (!hold(Tag::Type::Compound)) { throw std::runtime_error("tag not hold an object"); }
        return as<CompoundTag>()[index];
    }

    template <size_t N>
    [[nodiscard]] CompoundTagVariant& operator[](char const (&index)[N]) {
        return operator[](std::string_view{index, N - 1});
    }

    template <size_t N>
    [[nodiscard]] CompoundTagVariant const& operator[](char const (&index)[N]) const {
        return operator[](std::string_view{index, N - 1});
    }

    [[nodiscard]] std::unique_ptr<Tag> toUniqueCopy() const& {
        return std::visit(
            [](auto& val) -> std::unique_ptr<Tag> { return std::make_unique<std::decay_t<decltype(val)>>(val); },
            mStorage
        );
    }

    [[nodiscard]] std::unique_ptr<Tag> toUnique() && {
        return std::visit(
            [](auto&& val) -> std::unique_ptr<Tag> {
                return std::make_unique<std::decay_t<decltype(val)>>(std::move(val));
            },
            mStorage
        );
    }

    bool remove(std::string_view index) {
        if (is_object()) { as<CompoundTag>().remove(index); }
        throw std::runtime_error("tag not hold an object");
    }

    bool remove(size_t index) {
        if (is_object()) { as<ListTag>().remove(index); }
        throw std::runtime_error("tag not hold an array");
    }

    bool rename(std::string_view index, std::string_view newName) {
        if (is_object()) { as<CompoundTag>().rename(index, newName); }
        throw std::runtime_error("tag not hold an object");
    }

    void push_back(CompoundTagVariant val) {
        if (is_null()) { mStorage = ListTag{}; }
        if (!hold(Tag::Type::List)) { throw std::runtime_error("tag not hold an array"); }
        as<ListTag>().push_back(std::move(val).toUnique());
    }

    void push_back(Tag const& val) {
        if (is_null()) { mStorage = ListTag{}; }
        if (!hold(Tag::Type::List)) { throw std::runtime_error("tag not hold an array"); }
        as<ListTag>().push_back(val);
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    [[nodiscard]] constexpr operator T() const {
        return std::visit(
            [](auto& val) -> T {
                if constexpr (std::is_convertible_v<std::decay_t<decltype(val)>, T>) {
                    return static_cast<T>(val);
                } else {
                    throw std::runtime_error("tag can not convert to a number");
                }
            },
            mStorage
        );
    }

    [[nodiscard]] operator std::string const&() const {
        if (!is_string()) { throw std::runtime_error("tag can not convert to a string"); }
        return as<StringTag>().storage();
    }
    [[nodiscard]] operator std::string&() {
        if (!is_string()) { throw std::runtime_error("tag can not convert to a string"); }
        return as<StringTag>().storage();
    }

    [[nodiscard]] operator std::vector<uint8_t> const&() const {
        if (!is_binary()) { throw std::runtime_error("tag can not convert to a byte array"); }
        return as<ByteArrayTag>().storage();
    }
    [[nodiscard]] operator std::vector<uint8_t>&() {
        if (!is_binary()) { throw std::runtime_error("tag can not convert to a byte array"); }
        return as<ByteArrayTag>().storage();
    }

    [[nodiscard]] operator std::vector<int> const&() const {
        if (!is_binary()) { throw std::runtime_error("tag can not convert to a int array"); }
        return as<IntArrayTag>().storage();
    }
    [[nodiscard]] operator std::vector<int>&() {
        if (!is_binary()) { throw std::runtime_error("tag can not convert to a int array"); }
        return as<IntArrayTag>().storage();
    }

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr operator T&() noexcept {
        return as<T&>();
    }

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr operator T const&() const noexcept {
        return as<T const&>();
    }

    [[nodiscard]] std::string
    toSnbt(SnbtFormat snbtFormat = SnbtFormat::PrettyFilePrint, uint8_t indent = 4) const noexcept;

    [[nodiscard]] std::string toJson(uint8_t indent = 4) const noexcept;

    void merge(CompoundTagVariant const& other, bool mergeList = false);

public:
    [[nodiscard]] static CompoundTagVariant object(std::initializer_list<CompoundTag::TagMap::value_type> init = {}) {
        return CompoundTagVariant{std::in_place_type<CompoundTag>, init};
    }

    [[nodiscard]] static CompoundTagVariant array(std::initializer_list<CompoundTagVariant> init = {}) {
        return CompoundTagVariant{std::in_place_type<ListTag>, init};
    }

    [[nodiscard]] static std::optional<CompoundTagVariant>
    parse(std::string_view snbt, std::optional<size_t> parsedLength = {}) noexcept;
};

} // namespace bedrock_protocol
