#include <nbt/IntArrayTag.hpp>

namespace bedrock_protocol {

IntArrayTag::IntArrayTag(std::vector<int> const& data) : mData(data) {}

Tag::Type IntArrayTag::getType() const { return Tag::Type::IntArray; }

bool IntArrayTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::IntArray) && (mData == static_cast<const IntArrayTag&>(other).mData);
}

std::unique_ptr<Tag> IntArrayTag::copy() const { return std::make_unique<IntArrayTag>(mData); }

std::size_t IntArrayTag::hash() const {
    std::size_t hash = 0;
    for (int value : mData) {
        std::size_t element_hash  = std::hash<int>{}(value);
        hash                     ^= element_hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

void IntArrayTag::write(BytesDataOutput& stream) const {
    stream.writeInt((int)mData.size());
    for (const int i : mData) { stream.writeInt(i); }
}

void IntArrayTag::load(BytesDataInput& stream) {
    auto size = stream.getInt();
    mData.clear();
    for (int i = 0; i < size; ++i) { mData.push_back(stream.getInt()); }
}

void IntArrayTag::write(BinaryStream& stream) const {
    stream.writeVarInt((int)mData.size());
    for (auto& data : mData) { stream.writeVarInt(data); }
}

void IntArrayTag::load(ReadOnlyBinaryStream& stream) {
    auto size = stream.getVarInt();
    for (auto i = 0; i < size; i++) { mData.emplace_back(stream.getVarInt()); }
}

std::vector<int>&       IntArrayTag::data() { return mData; }
std::vector<int> const& IntArrayTag::data() const { return mData; }

size_t IntArrayTag::size() const { return mData.size(); }

} // namespace bedrock_protocol