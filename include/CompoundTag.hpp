#pragma once
#include <ByteArrayTag.hpp>
#include <ByteTag.hpp>
#include <DoubleTag.hpp>
#include <EndTag.hpp>
#include <FloatTag.hpp>
#include <Int64Tag.hpp>
#include <IntArrayTag.hpp>
#include <IntTag.hpp>
#include <ListTag.hpp>
#include <ShortTag.hpp>
#include <StringTag.hpp>
#include <Tag.hpp>
#include <map>

namespace bedrock_protocol {

class CompoundTagVariant;


class CompoundTag : public Tag {
public:
    using TagMap = std::map<std::string, CompoundTagVariant, std::less<>>;
    TagMap mTagMap;

public:
    CompoundTag() = default;

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

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

    const Tag* get(std::string const& key) const;
    Tag*       get(std::string const& key);

    const ByteTag* getByte(std::string const& key) const;
    ByteTag*       getByte(std::string const& key);

    const ShortTag* getShort(std::string const& key) const;
    ShortTag*       getShort(std::string const& key);

    const IntTag* getInt(std::string const& key) const;
    IntTag*       getInt(std::string const& key);

    const Int64Tag* getInt64(std::string const& key) const;
    Int64Tag*       getInt64(std::string const& key);

    const FloatTag* getFloat(std::string const& key) const;
    FloatTag*       getFloat(std::string const& key);

    const DoubleTag* getDouble(std::string const& key) const;
    DoubleTag*       getDouble(std::string const& key);

    const StringTag* getString(std::string const& key) const;
    StringTag*       getString(std::string const& key);

    const ByteArrayTag* getByteArray(std::string const& key) const;
    ByteArrayTag*       getByteArray(std::string const& key);

    const IntArrayTag* getIntArray(std::string const& key) const;
    IntArrayTag*       getIntArray(std::string const& key);

    const CompoundTag* getCompound(std::string const& key) const;
    CompoundTag*       getCompound(std::string const& key);

    const ListTag* getList(std::string const& key) const;
    ListTag*       getList(std::string const& key);

    bool contains(std::string const& key) const;
    bool contains(std::string const& key, Type type) const;

    bool isEmpty() const;

    bool remove(std::string const&);

    TagMap::const_iterator begin() const;
    TagMap::const_iterator end() const;

    std::unique_ptr<CompoundTag> clone() const;

public:
    void serialize(BinaryStream& stream) const;
    void serialize(BytesDataOutput& stream) const;

    void deserialize(ReadOnlyBinaryStream& stream);
    void deserialize(BytesDataInput& stream);

public:
    static CompoundTag fromNetworkNbt(std::string_view binaryData);
    static CompoundTag fromBinaryNbt(std::string_view binaryData);

    std::string toNetworkNbt() const;
    std::string toBinaryNbt() const;
};

} // namespace bedrock_protocol
