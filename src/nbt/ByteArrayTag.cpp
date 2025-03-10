#include <nbt/ByteArrayTag.hpp>

namespace bedrock_protocol {

ByteArrayTag::ByteArrayTag(std::vector<uint8_t> const& data) : mData(data) {}

Tag::Type ByteArrayTag::getType() const { return Tag::Type::ByteArray; }

bool ByteArrayTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::ByteArray) && (mData == static_cast<const ByteArrayTag&>(other).mData);
}

std::unique_ptr<Tag> ByteArrayTag::copy() const { return std::make_unique<ByteArrayTag>(mData); }

std::size_t ByteArrayTag::hash() const {
    std::size_t hash = 0;
    for (uint8_t value : mData) {
        std::size_t element_hash  = std::hash<uint8_t>{}(value);
        hash                     ^= element_hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

void ByteArrayTag::write(BytesDataOutput& stream) const {
    stream.writeInt((int)mData.size());
    stream.writeBytes(mData.data(), mData.size());
}

void ByteArrayTag::load(BytesDataInput& stream) {
    auto size = stream.getInt();
    mData.resize(size);
    stream.getBytes(mData.data(), size);
}

void ByteArrayTag::write(BinaryStream& stream) const {
    stream.writeVarInt((int)mData.size());
    for (auto& data : mData) { stream.writeByte(data); }
}

void ByteArrayTag::load(ReadOnlyBinaryStream& stream) {
    auto size = stream.getVarInt();
    for (auto i = 0; i < size; i++) { mData.emplace_back(stream.getByte()); }
}

} // namespace bedrock_protocol