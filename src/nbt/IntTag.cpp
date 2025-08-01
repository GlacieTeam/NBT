#include <nbt/IntTag.hpp>

namespace bedrock_protocol {

IntTag::IntTag(int data) : mData(data) {}

Tag::Type IntTag::getType() const { return Tag::Type::Int; }

bool IntTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Int) && (mData == static_cast<const IntTag&>(other).mData);
}

std::unique_ptr<Tag> IntTag::copy() const { return std::make_unique<IntTag>(mData); }

std::size_t IntTag::hash() const { return mData; }

void IntTag::write(BytesDataOutput& stream) const { stream.writeInt(mData); }

void IntTag::load(BytesDataInput& stream) { mData = stream.getInt(); }

void IntTag::write(BinaryStream& stream) const { stream.writeVarInt(mData); }

void IntTag::load(ReadOnlyBinaryStream& stream) { mData = stream.getVarInt(); }

int&       IntTag::data() { return mData; }
int const& IntTag::data() const { return mData; }

} // namespace bedrock_protocol