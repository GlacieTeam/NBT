#include <nbt/DoubleTag.hpp>

namespace bedrock_protocol {

DoubleTag::DoubleTag(double data) : mData(data) {}

Tag::Type DoubleTag::getType() const { return Tag::Type::Double; }

bool DoubleTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Double) && (mData == static_cast<const DoubleTag&>(other).mData);
}

std::unique_ptr<Tag> DoubleTag::copy() const { return std::make_unique<DoubleTag>(mData); }

std::size_t DoubleTag::hash() const { return std::hash<double>{}(mData); }

void DoubleTag::write(BytesDataOutput& stream) const { stream.writeDouble(mData); }

void DoubleTag::load(BytesDataInput& stream) { mData = stream.getDouble(); }

void DoubleTag::write(BinaryStream& stream) const { stream.writeDouble(mData); }

void DoubleTag::load(ReadOnlyBinaryStream& stream) { mData = stream.getDouble(); }

} // namespace bedrock_protocol