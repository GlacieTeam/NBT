#include <nbt/EndTag.hpp>

namespace bedrock_protocol {

Tag::Type EndTag::getType() const { return Tag::Type::End; }

bool EndTag::equals(const Tag& other) const { return other.getType() == Tag::Type::End; }

std::unique_ptr<Tag> EndTag::copy() const { return std::make_unique<EndTag>(); }

std::size_t EndTag::hash() const { return 0; }

void EndTag::write(BytesDataOutput&) const {}

void EndTag::load(BytesDataInput&) {}

void EndTag::write(BinaryStream&) const {}

void EndTag::load(ReadOnlyBinaryStream&) {}

} // namespace bedrock_protocol
