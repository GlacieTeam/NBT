#pragma once
#include <map>
#include <nbt/Tag.hpp>
#include <vector>

namespace bedrock_protocol {

class CompoundTagVariant;
class EndTag;
class ByteTag;
class ShortTag;
class IntTag;
class Int64Tag;
class FloatTag;
class DoubleTag;
class ByteArrayTag;
class StringTag;
class ListTag;
class IntArrayTag;

class CompoundTag : public Tag {
public:
    using TagMap = std::map<std::string, CompoundTagVariant, std::less<>>;
    TagMap mTagMap;

public:
    using iterator               = TagMap::iterator;
    using const_iterator         = TagMap::const_iterator;
    using reverse_iterator       = TagMap::reverse_iterator;
    using const_reverse_iterator = TagMap::const_reverse_iterator;

public:
    CompoundTag() = default;
    CompoundTag(std::initializer_list<TagMap::value_type> tagPairs) : mTagMap(tagPairs) {}

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

public:
    [[nodiscard]] CompoundTagVariant&       operator[](std::string_view index);
    [[nodiscard]] CompoundTagVariant const& operator[](std::string_view index) const;

    [[nodiscard]] CompoundTagVariant&       at(std::string_view index);
    [[nodiscard]] CompoundTagVariant const& at(std::string_view index) const;

    [[nodiscard]] iterator begin() noexcept;
    [[nodiscard]] iterator end() noexcept;

    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] const_iterator end() const noexcept;

    [[nodiscard]] reverse_iterator rbegin() noexcept;
    [[nodiscard]] reverse_iterator rend() noexcept;

    [[nodiscard]] const_iterator cbegin() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] const_reverse_iterator crend() const noexcept;

public:
    void put(std::string_view key, Tag&& tag);
    void put(std::string_view key, std::unique_ptr<Tag> tag);

    void putByte(std::string_view key, uint8_t value);
    void putShort(std::string_view key, int16_t value);
    void putInt(std::string_view key, int32_t value);
    void putInt64(std::string_view key, int64_t value);
    void putFloat(std::string_view key, float value);
    void putDouble(std::string_view key, double value);
    void putString(std::string_view key, std::string_view value);
    void putByteArray(std::string_view key, std::vector<uint8_t> const& value);
    void putIntArray(std::string_view key, std::vector<int> const& value);
    void putCompound(std::string_view key, CompoundTag&& value);
    void putCompound(std::string_view key, std::unique_ptr<CompoundTag> value);
    void putList(std::string_view key, ListTag&& value);
    void putList(std::string_view key, std::unique_ptr<ListTag> value);

    [[nodiscard]] const Tag* get(std::string_view key) const;
    [[nodiscard]] Tag*       get(std::string_view key);

    [[nodiscard]] const ByteTag* getByte(std::string_view key) const;
    [[nodiscard]] ByteTag*       getByte(std::string_view key);

    [[nodiscard]] const ShortTag* getShort(std::string_view key) const;
    [[nodiscard]] ShortTag*       getShort(std::string_view key);

    [[nodiscard]] const IntTag* getInt(std::string_view key) const;
    [[nodiscard]] IntTag*       getInt(std::string_view key);

    [[nodiscard]] const Int64Tag* getInt64(std::string_view key) const;
    [[nodiscard]] Int64Tag*       getInt64(std::string_view key);

    [[nodiscard]] const FloatTag* getFloat(std::string_view key) const;
    [[nodiscard]] FloatTag*       getFloat(std::string_view key);

    [[nodiscard]] const DoubleTag* getDouble(std::string_view key) const;
    [[nodiscard]] DoubleTag*       getDouble(std::string_view key);

    [[nodiscard]] const StringTag* getString(std::string_view key) const;
    [[nodiscard]] StringTag*       getString(std::string_view key);

    [[nodiscard]] const ByteArrayTag* getByteArray(std::string_view key) const;
    [[nodiscard]] ByteArrayTag*       getByteArray(std::string_view key);

    [[nodiscard]] const IntArrayTag* getIntArray(std::string_view key) const;
    [[nodiscard]] IntArrayTag*       getIntArray(std::string_view key);

    [[nodiscard]] const CompoundTag* getCompound(std::string_view key) const;
    [[nodiscard]] CompoundTag*       getCompound(std::string_view key);

    [[nodiscard]] const ListTag* getList(std::string_view key) const;
    [[nodiscard]] ListTag*       getList(std::string_view key);

    [[nodiscard]] bool contains(std::string_view key) const;
    [[nodiscard]] bool contains(std::string_view key, Type type) const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] size_t size() const;

    bool remove(std::string_view index);

    std::unique_ptr<CompoundTag> clone() const;


public:
    void serialize(BinaryStream& stream) const;
    void serialize(BytesDataOutput& stream) const;

    void deserialize(ReadOnlyBinaryStream& stream);
    void deserialize(BytesDataInput& stream);

public:
    static CompoundTag fromNetworkNbt(std::string_view binaryData);
    static CompoundTag fromBinaryNbt(std::string_view binaryData, bool isLittleEndian = true);

    std::string toNetworkNbt() const;
    std::string toBinaryNbt(bool isLittleEndian = true) const;
};

} // namespace bedrock_protocol
