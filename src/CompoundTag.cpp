#include <BinaryStream.hpp>
#include <CompoundTag.hpp>
#include <CompoundTagVariant.hpp>
#include <algorithm>

namespace bedrock_protocol {

Tag::Type CompoundTag::getType() const { return Tag::Type::Compound; }

bool CompoundTag::equals(Tag const& other) const {
    if (other.getType() != Tag::Type::Compound) { return false; }
    const auto& other_tag = static_cast<const CompoundTag&>(other);
    return std::all_of(mTagMap.begin(), mTagMap.end(), [&](const auto& kv) {
        const auto* tag = other_tag.get(kv.first);
        return tag && kv.second.get()->equals(*tag);
    });
}

std::unique_ptr<Tag> CompoundTag::copy() const { return clone(); }

std::size_t CompoundTag::hash() const {
    int hash      = 0x811c9dc5;
    int fnv_prime = 0x01000193;
    for (unsigned char byte : toBinaryNbt()) {
        hash ^= byte;
        hash  = hash * fnv_prime;
    }
    return hash;
}

std::unique_ptr<CompoundTag> CompoundTag::clone() const {
    auto new_tag = std::make_unique<CompoundTag>();
    for (const auto& [key, value] : mTagMap) { new_tag->mTagMap[key].emplace(std::move(*value.get()->copy())); }
    return new_tag;
}

void CompoundTag::write(BytesDataOutput& stream) const {
    for (const auto& [key, tag] : mTagMap) {
        auto type = tag->getType();
        stream.writeByte((uint8_t)type);
        if (type != Tag::Type::End) {
            stream.writeString(key);
            tag->write(stream);
        }
    }
    stream.writeByte((uint8_t)Type::End);
}

void CompoundTag::load(BytesDataInput& stream) {
    mTagMap.clear();
    auto type = Tag::Type::End;
    do {
        type = Tag::Type(stream.getByte());
        if (type != Tag::Type::End) {
            auto key = stream.getString();
            auto tag = Tag::newTag(type);
            tag->load(stream);
            put(key, std::move(tag));
        }
    } while (type != Tag::Type::End);
}

void CompoundTag::put(std::string const& key, Tag&& tag) { mTagMap[key].emplace(std::forward<Tag>(tag)); }

void CompoundTag::put(std::string const& key, std::unique_ptr<Tag> tag) {
    if (tag) { mTagMap[key].emplace(std::move(*tag)); }
}

void CompoundTag::putByte(std::string const& key, uint8_t value) { mTagMap[key].emplace(ByteTag(value)); }

void CompoundTag::putShort(std::string const& key, int16_t value) { mTagMap[key].emplace(ShortTag(value)); }

void CompoundTag::putInt(std::string const& key, int32_t value) { mTagMap[key].emplace(IntTag(value)); }

void CompoundTag::putInt64(std::string const& key, int64_t value) { mTagMap[key].emplace(Int64Tag(value)); }

void CompoundTag::putFloat(std::string const& key, float value) { mTagMap[key].emplace(FloatTag(value)); }

void CompoundTag::putDouble(std::string const& key, double value) { mTagMap[key].emplace(DoubleTag(value)); }

void CompoundTag::putString(std::string const& key, std::string const& value) {
    mTagMap[key].emplace(StringTag(value));
}

void CompoundTag::putByteArray(std::string const& key, std::vector<uint8_t> const& value) {
    mTagMap[key].emplace(ByteArrayTag(value));
}

void CompoundTag::putIntArray(std::string const& key, std::vector<int> const& value) {
    mTagMap[key].emplace(IntArrayTag(value));
}

void CompoundTag::putCompound(std::string const& key, CompoundTag&& value) { mTagMap[key].emplace(std::move(value)); }

void CompoundTag::putCompound(std::string const& key, std::unique_ptr<CompoundTag> value) {
    if (value) { mTagMap[key].emplace(std::move(*value)); }
}

void CompoundTag::putList(std::string const& key, ListTag&& value) { mTagMap[key].emplace(std::move(value)); }

void CompoundTag::putList(std::string const& key, std::unique_ptr<ListTag> value) {
    if (value) { mTagMap[key].emplace(std::move(*value)); }
}

const Tag* CompoundTag::get(std::string const& key) const {
    if (mTagMap.contains(key)) { return mTagMap.at(key).get(); }
    return nullptr;
}

Tag* CompoundTag::get(std::string const& key) {
    if (mTagMap.contains(key)) { return mTagMap.at(key).get(); }
    return nullptr;
}

const ByteTag* CompoundTag::getByte(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::Byte) { return static_cast<const ByteTag*>(tag); }
    }
    return nullptr;
}

ByteTag* CompoundTag::getByte(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Byte) { return static_cast<ByteTag*>(tag); }
    }
    return nullptr;
}

const ShortTag* CompoundTag::getShort(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::Short) { return static_cast<const ShortTag*>(tag); }
    }
    return nullptr;
}

ShortTag* CompoundTag::getShort(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Short) { return static_cast<ShortTag*>(tag); }
    }
    return nullptr;
}

const IntTag* CompoundTag::getInt(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::Int) { return static_cast<const IntTag*>(tag); }
    }
    return nullptr;
}

IntTag* CompoundTag::getInt(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Int) { return static_cast<IntTag*>(tag); }
    }
    return nullptr;
}

const Int64Tag* CompoundTag::getInt64(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::Int64) { return static_cast<const Int64Tag*>(tag); }
    }
    return nullptr;
}

Int64Tag* CompoundTag::getInt64(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Int64) { return static_cast<Int64Tag*>(tag); }
    }
    return nullptr;
}

const FloatTag* CompoundTag::getFloat(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::Float) { return static_cast<const FloatTag*>(tag); }
    }
    return nullptr;
}

FloatTag* CompoundTag::getFloat(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Float) { return static_cast<FloatTag*>(tag); }
    }
    return nullptr;
}

const DoubleTag* CompoundTag::getDouble(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::Double) { return static_cast<const DoubleTag*>(tag); }
    }
    return nullptr;
}

DoubleTag* CompoundTag::getDouble(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Double) { return static_cast<DoubleTag*>(tag); }
    }
    return nullptr;
}

const StringTag* CompoundTag::getString(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::String) { return static_cast<const StringTag*>(tag); }
    }
    return nullptr;
}

StringTag* CompoundTag::getString(std::string const& key) {
    if (auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::String) { return static_cast<StringTag*>(tag); }
    }
    return nullptr;
}

const ByteArrayTag* CompoundTag::getByteArray(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::ByteArray) { return static_cast<const ByteArrayTag*>(tag); }
    }
    return nullptr;
}

ByteArrayTag* CompoundTag::getByteArray(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::ByteArray) { return static_cast<ByteArrayTag*>(tag); }
    }
    return nullptr;
}

const IntArrayTag* CompoundTag::getIntArray(std::string const& key) const {
    if (const auto* tag = get(key)) {
        if (tag->getType() == Tag::Type::IntArray) { return static_cast<const IntArrayTag*>(tag); }
    }
    return nullptr;
}

IntArrayTag* CompoundTag::getIntArray(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::IntArray) { return static_cast<IntArrayTag*>(tag); }
    }
    return nullptr;
}

const CompoundTag* CompoundTag::getCompound(std::string const& key) const {
    if (const auto* tag = get(key); tag) {
        if (tag->getType() == Type::Compound) { return static_cast<const CompoundTag*>(tag); }
    }
    return nullptr;
}

CompoundTag* CompoundTag::getCompound(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::Compound) { return static_cast<CompoundTag*>(tag); }
    }
    return nullptr;
}

const ListTag* CompoundTag::getList(std::string const& key) const {
    if (const auto* tag = get(key); tag) {
        if (tag->getType() == Type::List) { return static_cast<const ListTag*>(tag); }
    }
    return nullptr;
}

ListTag* CompoundTag::getList(std::string const& key) {
    if (auto* tag = get(key); tag) {
        if (tag->getType() == Type::List) { return static_cast<ListTag*>(tag); }
    }
    return nullptr;
}

bool CompoundTag::contains(std::string const& key) const { return get(key) != nullptr; }

bool CompoundTag::contains(std::string const& key, Tag::Type type) const {
    if (const auto* tag = get(key); tag) { return tag->getType() == type; }
    return false;
}

bool CompoundTag::isEmpty() const { return mTagMap.empty(); }

bool CompoundTag::remove(std::string const& name) {
    auto it = mTagMap.find(name);
    if (it == mTagMap.end()) { return false; }
    mTagMap.erase(it);
    return true;
}

CompoundTag::TagMap::const_iterator CompoundTag::begin() const { return mTagMap.begin(); }

CompoundTag::TagMap::const_iterator CompoundTag::end() const { return mTagMap.end(); }

CompoundTag CompoundTag::fromBinaryNbt(std::string_view binaryData) {
    auto        dataBuffer = std::string(binaryData);
    auto        stream     = BytesDataInput(dataBuffer, false);
    CompoundTag result;
    stream.getByte();
    stream.getString();
    result.load(stream);
    return result;
}

std::string CompoundTag::toBinaryNbt() const {
    auto stream = BytesDataOutput();
    stream.writeByte((uint8_t)Tag::Type::Compound);
    stream.writeString("");
    write(stream);
    return stream.getAndReleaseData();
}

extern void writeCompoundData(BinaryStream& stream, CompoundTag const& nbt);

void writeTag(BinaryStream& stream, Tag const& tag) {
    switch (tag.getType()) {
    case CompoundTag::Type::Byte: {
        stream.writeByte(((ByteTag&)tag).mData);
        break;
    }
    case CompoundTag::Type::Short: {
        stream.writeSignedShort(((ShortTag&)tag).mData);
        break;
    }
    case CompoundTag::Type::Int: {
        stream.writeVarInt(((IntTag&)tag).mData);
        break;
    }
    case CompoundTag::Type::Int64: {
        stream.writeVarInt64(((Int64Tag&)tag).mData);
        break;
    }
    case CompoundTag::Type::Float: {
        stream.writeFloat(((FloatTag&)tag).mData);
        break;
    }
    case CompoundTag::Type::Double: {
        stream.writeDouble(((DoubleTag&)tag).mData);
        break;
    }
    case CompoundTag::Type::ByteArray: {
        auto& array = ((ByteArrayTag&)tag).mData;
        stream.writeVarInt((int)array.size());
        for (auto& data : array) { stream.writeByte(data); }
        break;
    }
    case CompoundTag::Type::String: {
        stream.writeString(((StringTag&)tag).mData);
        break;
    }
    case CompoundTag::Type::List: {
        auto& list = ((ListTag&)tag).mData;
        stream.writeByte((uint8_t)((ListTag&)tag).mType);
        stream.writeVarInt((int)list.size());
        for (auto& val : list) { writeTag(stream, *val); }
        break;
    }
    case CompoundTag::Type::Compound: {
        writeCompoundData(stream, (CompoundTag&)tag);
        break;
    }
    case CompoundTag::Type::IntArray: {
        auto& array = ((IntArrayTag&)tag).mData;
        stream.writeVarInt((int)array.size());
        for (auto& data : array) { stream.writeVarInt(data); }
        break;
    }
    default:
        break;
    }
}

void writeCompoundData(BinaryStream& stream, CompoundTag const& nbt) {
    for (auto& [name, tag] : nbt.mTagMap) {
        stream.writeByte((uint8_t)tag.get()->getType());
        if (tag->getType() != CompoundTag::Type::End) {
            stream.writeString(name);
            writeTag(stream, *tag.get());
        }
    }
    stream.writeByte(0);
}

extern void getCompoundData(ReadOnlyBinaryStream& stream, CompoundTag& nbt);

std::unique_ptr<Tag> getTag(ReadOnlyBinaryStream& stream, CompoundTag::Type type) {
    switch (type) {
    case CompoundTag::Type::Byte: {
        return std::make_unique<ByteTag>(stream.getByte());
    }
    case CompoundTag::Type::Short: {
        return std::make_unique<ShortTag>(stream.getSignedShort());
    }
    case CompoundTag::Type::Int: {
        return std::make_unique<IntTag>(stream.getVarInt());
    }
    case CompoundTag::Type::Int64: {
        return std::make_unique<Int64Tag>(stream.getVarInt64());
    }
    case CompoundTag::Type::Float: {
        return std::make_unique<FloatTag>(stream.getFloat());
    }
    case CompoundTag::Type::Double: {
        return std::make_unique<DoubleTag>(stream.getDouble());
    }
    case CompoundTag::Type::ByteArray: {
        auto                 size = stream.getVarInt();
        std::vector<uint8_t> data;
        for (auto i = 0; i < size; i++) { data.emplace_back(stream.getByte()); }
        return std::make_unique<ByteArrayTag>(data);
    }
    case CompoundTag::Type::String: {
        return std::make_unique<StringTag>(stream.getString());
    }
    case CompoundTag::Type::List: {
        auto                            listType = CompoundTag::Type(stream.getByte());
        auto                            size     = stream.getVarInt();
        std::vector<CompoundTagVariant> data;
        for (auto i = 0; i < size; i++) { data.emplace_back(getTag(stream, listType)); }
        return std::make_unique<ListTag>(std::move(data));
    }
    case CompoundTag::Type::Compound: {
        auto data = std::make_unique<CompoundTag>();
        getCompoundData(stream, *data);
        return data;
    }
    case CompoundTag::Type::IntArray: {
        auto             size = stream.getVarInt();
        std::vector<int> data;
        for (auto i = 0; i < size; i++) { data.emplace_back(stream.getVarInt()); }
        return std::make_unique<IntArrayTag>(data);
    }
    default:
        return std::make_unique<EndTag>();
    }
}

void getCompoundData(ReadOnlyBinaryStream& stream, CompoundTag& nbt) {
    auto type = CompoundTag::Type::End;
    do {
        type = CompoundTag::Type(stream.getByte());
        if (type != CompoundTag::Type::End) {
            auto name = stream.getString();
            auto tag  = getTag(stream, type);
            nbt.put(name, std::move(tag));
        }
    } while (type != CompoundTag::Type::End);
}

void writeCompoundTag(BinaryStream& stream, CompoundTag const& nbt) {
    stream.writeByte(10);
    stream.writeString("");
    writeCompoundData(stream, nbt);
}

CompoundTag getCompoundTag(ReadOnlyBinaryStream& stream) {
    CompoundTag nbt;
    stream.getByte();
    stream.getString();
    getCompoundData(stream, nbt);
    return std::move(nbt);
}

CompoundTag CompoundTag::fromNetworkNbt(std::string_view binaryData) {
    ReadOnlyBinaryStream stream(binaryData, false);
    return getCompoundTag(stream);
}

std::string CompoundTag::toNetworkNbt() const {
    BinaryStream stream;
    writeCompoundTag(stream, *this);
    return stream.getAndReleaseData();
}

} // namespace bedrock_protocol