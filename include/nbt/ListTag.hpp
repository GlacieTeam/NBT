// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <functional>
#include <nbt/Tag.hpp>
#include <vector>

namespace bedrock_protocol {

class CompoundTag;
class CompoundTagVariant;

class ListTag : public Tag {
public:
    using TagList = std::vector<std::unique_ptr<Tag>>;

private:
    TagList mStorage;
    Type    mType{Type::End};

public:
    using iterator               = TagList::iterator;
    using const_iterator         = TagList::const_iterator;
    using reverse_iterator       = TagList::reverse_iterator;
    using const_reverse_iterator = TagList::const_reverse_iterator;

public:
    [[nodiscard]] ListTag() = default;
    [[nodiscard]] ListTag(TagList&& data);
    [[nodiscard]] ListTag(ListTag const& other);
    [[nodiscard]] ListTag(ListTag&& other);
    [[nodiscard]] ListTag(std::initializer_list<CompoundTagVariant> tags);
    [[nodiscard]] ListTag(std::vector<CompoundTagVariant> tags);

    ListTag& operator=(ListTag const& other);
    ListTag& operator=(ListTag&& other);

    [[nodiscard]] constexpr Type getType() const override { return Type::List; }

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

public:
    [[nodiscard]] std::unique_ptr<ListTag> copyList() const;

    [[nodiscard]] size_t size() const noexcept;
    [[nodiscard]] bool   empty() const;

    [[nodiscard]] Type const& getElementType() const;
    [[nodiscard]] Type&       getElementType();

    void push_back(std::unique_ptr<Tag>&& tag);
    void push_back(Tag const& tag);
    void push_back(CompoundTagVariant val);

    void reserve(size_t size);

    bool remove(size_t index);
    bool remove(size_t startIndex, size_t endIndex);
    void clear() noexcept;

    [[nodiscard]] TagList&       storage() noexcept;
    [[nodiscard]] TagList const& storage() const noexcept;

    void forEachCompoundTag(std::function<void(CompoundTag const& tag)> func);

    [[nodiscard]] Tag&       operator[](size_t index) noexcept;
    [[nodiscard]] Tag const& operator[](size_t index) const noexcept;

    [[nodiscard]] Tag&       at(size_t index);
    [[nodiscard]] Tag const& at(size_t index) const;

    [[nodiscard]] iterator begin() noexcept;
    [[nodiscard]] iterator end() noexcept;

    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] const_iterator end() const noexcept;

    [[nodiscard]] const_iterator cbegin() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;

    [[nodiscard]] reverse_iterator rbegin() noexcept;
    [[nodiscard]] reverse_iterator rend() noexcept;

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] const_reverse_iterator crend() const noexcept;

    iterator erase(const_iterator where) noexcept;
    iterator erase(const_iterator first, const_iterator last) noexcept;

    bool set(size_t index, Tag const& tag);
};

} // namespace bedrock_protocol