#include <nbt/ShortTag.hpp>

namespace bedrock_protocol {

ShortTag::ShortTag(short data) : mData(data) {}

Tag::Type ShortTag::getType() const { return Tag::Type::Short; }

bool ShortTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Short) && (mData == static_cast<const ShortTag&>(other).mData);
}

std::unique_ptr<Tag> ShortTag::copy() const { return std::make_unique<ShortTag>(mData); }

std::size_t ShortTag::hash() const { return mData; }

void ShortTag::write(BytesDataOutput& stream) const { stream.writeShort(mData); }

void ShortTag::load(BytesDataInput& stream) { mData = stream.getShort(); }

void ShortTag::write(BinaryStream& stream) const { stream.writeSignedShort(mData); }

void ShortTag::load(ReadOnlyBinaryStream& stream) { mData = stream.getSignedShort(); }

} // namespace bedrock_protocol