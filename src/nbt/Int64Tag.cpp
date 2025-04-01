#include <nbt/Int64Tag.hpp>

namespace bedrock_protocol {

Int64Tag::Int64Tag(int64_t data) : mData(data) {}

Tag::Type Int64Tag::getType() const { return Tag::Type::Int64; }

bool Int64Tag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::Int64) && (mData == static_cast<const Int64Tag&>(other).mData);
}

std::unique_ptr<Tag> Int64Tag::copy() const { return std::make_unique<Int64Tag>(mData); }

std::size_t Int64Tag::hash() const { return mData; }

void Int64Tag::write(BytesDataOutput& stream) const { stream.writeInt64(mData); }

void Int64Tag::load(BytesDataInput& stream) { mData = stream.getInt64(); }

void Int64Tag::write(BinaryStream& stream) const { stream.writeVarInt64(mData); }

void Int64Tag::load(ReadOnlyBinaryStream& stream) { mData = stream.getVarInt64(); }

int64_t&       Int64Tag::data() { return mData; }
int64_t const& Int64Tag::data() const { return mData; }

} // namespace bedrock_protocol