#pragma once
#include <map>
#include <nbt/ByteArrayTag.hpp>
#include <nbt/ByteTag.hpp>
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

namespace bedrock_protocol {

class CompoundTagVariant;


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
    void put(std::string const& key, Tag&& tag);
    void put(std::string const& key, std::unique_ptr<Tag> tag);

    void putByte(std::string const& key, uint8_t value);
    void putShort(std::string const& key, int16_t value);
    void putInt(std::string const& key, int32_t value);
    void putInt64(std::string const& key, int64_t value);
    void putFloat(std::string const& key, float value);
    void putDouble(std::string const& key, double value);
    void putString(std::string const& key, std::string const& value);
    void putByteArray(std::string const& key, std::vector<uint8_t> const& value);
    void putIntArray(std::string const& key, std::vector<int> const& value);
    void putCompound(std::string const& key, CompoundTag&& value);
    void putCompound(std::string const& key, std::unique_ptr<CompoundTag> value);
    void putList(std::string const& key, ListTag&& value);
    void putList(std::string const& key, std::unique_ptr<ListTag> value);

    [[nodiscard]] const Tag* get(std::string const& key) const;
    [[nodiscard]] Tag*       get(std::string const& key);

    [[nodiscard]] const ByteTag* getByte(std::string const& key) const;
    [[nodiscard]] ByteTag*       getByte(std::string const& key);

    [[nodiscard]] const ShortTag* getShort(std::string const& key) const;
    [[nodiscard]] ShortTag*       getShort(std::string const& key);

    [[nodiscard]] const IntTag* getInt(std::string const& key) const;
    [[nodiscard]] IntTag*       getInt(std::string const& key);

    [[nodiscard]] const Int64Tag* getInt64(std::string const& key) const;
    [[nodiscard]] Int64Tag*       getInt64(std::string const& key);

    [[nodiscard]] const FloatTag* getFloat(std::string const& key) const;
    [[nodiscard]] FloatTag*       getFloat(std::string const& key);

    [[nodiscard]] const DoubleTag* getDouble(std::string const& key) const;
    [[nodiscard]] DoubleTag*       getDouble(std::string const& key);

    [[nodiscard]] const StringTag* getString(std::string const& key) const;
    [[nodiscard]] StringTag*       getString(std::string const& key);

    [[nodiscard]] const ByteArrayTag* getByteArray(std::string const& key) const;
    [[nodiscard]] ByteArrayTag*       getByteArray(std::string const& key);

    [[nodiscard]] const IntArrayTag* getIntArray(std::string const& key) const;
    [[nodiscard]] IntArrayTag*       getIntArray(std::string const& key);

    [[nodiscard]] const CompoundTag* getCompound(std::string const& key) const;
    [[nodiscard]] CompoundTag*       getCompound(std::string const& key);

    [[nodiscard]] const ListTag* getList(std::string const& key) const;
    [[nodiscard]] ListTag*       getList(std::string const& key);

    [[nodiscard]] bool contains(std::string const& key) const;
    [[nodiscard]] bool contains(std::string const& key, Type type) const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] size_t size() const;

    bool remove(std::string const&);

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
