#include <algorithm>
#include <nbt/CompoundTag.hpp>
#include <nbt/CompoundTagVariant.hpp>

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

void CompoundTag::write(BinaryStream& stream) const {
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

void CompoundTag::load(ReadOnlyBinaryStream& stream) {
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

void CompoundTag::serialize(BinaryStream& stream) const {
    stream.writeByte((uint8_t)Tag::Type::Compound);
    stream.writeString("");
    write(stream);
}

void CompoundTag::serialize(BytesDataOutput& stream) const {
    stream.writeByte((uint8_t)Tag::Type::Compound);
    stream.writeString("");
    write(stream);
}

void CompoundTag::deserialize(ReadOnlyBinaryStream& stream) {
    stream.getByte();
    stream.getString();
    load(stream);
}

void CompoundTag::deserialize(BytesDataInput& stream) {
    stream.getByte();
    stream.getString();
    load(stream);
}

CompoundTag CompoundTag::fromBinaryNbt(std::string_view binaryData) {
    BytesDataInput stream(binaryData, false);
    CompoundTag    result;
    result.deserialize(stream);
    return result;
}

std::string CompoundTag::toBinaryNbt() const {
    BytesDataOutput stream;
    serialize(stream);
    return stream.getAndReleaseData();
}

CompoundTag CompoundTag::fromNetworkNbt(std::string_view binaryData) {
    ReadOnlyBinaryStream stream(binaryData, false);
    CompoundTag          result;
    result.deserialize(stream);
    return result;
}

std::string CompoundTag::toNetworkNbt() const {
    BinaryStream stream;
    serialize(stream);
    return stream.getAndReleaseData();
}

} // namespace bedrock_protocol