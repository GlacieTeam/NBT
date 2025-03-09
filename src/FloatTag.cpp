#include <FloatTag.hpp>

namespace bedrock_protocol {

FloatTag::FloatTag(float data) : mData(data) {}

Tag::Type FloatTag::getType() const { return Tag::Type::Float; }

bool FloatTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Float) && (mData == static_cast<const FloatTag&>(other).mData);
}

std::unique_ptr<Tag> FloatTag::copy() const { return std::make_unique<FloatTag>(mData); }

std::size_t FloatTag::hash() const { return std::hash<float>{}(mData); }

void FloatTag::write(BytesDataOutput& stream) const { stream.writeFloat(mData); }

void FloatTag::load(BytesDataInput& stream) { mData = stream.getFloat(); }

void FloatTag::write(BinaryStream& stream) const { stream.writeFloat(mData); }

void FloatTag::load(ReadOnlyBinaryStream& stream) { mData = stream.getFloat(); }

} // namespace bedrock_protocol