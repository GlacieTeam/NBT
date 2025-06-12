#pragma once
#include <functional>
#include <nbt/Tag.hpp>
#include <vector>

namespace bedrock_protocol {

class CompoundTag;

class ListTag : public Tag {
public:
    std::vector<std::unique_ptr<Tag>> mData;
    Type                              mType{Type::End};

public:
    using iterator               = std::vector<std::unique_ptr<Tag>>::iterator;
    using const_iterator         = std::vector<std::unique_ptr<Tag>>::const_iterator;
    using reverse_iterator       = std::vector<std::unique_ptr<Tag>>::reverse_iterator;
    using const_reverse_iterator = std::vector<std::unique_ptr<Tag>>::const_reverse_iterator;

public:
    [[nodiscard]] ListTag() = default;
    [[nodiscard]] ListTag(std::vector<std::unique_ptr<Tag>>&& data);
    [[nodiscard]] ListTag(ListTag const& other);
    [[nodiscard]] ListTag(ListTag&& other);
    [[nodiscard]] ListTag(std::initializer_list<CompoundTagVariant> tags);
    [[nodiscard]] ListTag(std::vector<CompoundTagVariant> tags);

    ListTag& operator=(ListTag const& other);
    ListTag& operator=(ListTag&& other);

    [[nodiscard]] Type getType() const override;

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

public:
    [[nodiscard]] std::unique_ptr<ListTag> copyList() const;

    [[nodiscard]] size_t size() const;

    void add(std::unique_ptr<Tag>&& tag);

    void forEachCompoundTag(std::function<void(CompoundTag const& tag)> func);

    [[nodiscard]] Tag&       operator[](size_t index);
    [[nodiscard]] Tag const& operator[](size_t index) const;

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
};

} // namespace bedrock_protocol