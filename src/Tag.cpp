#include <CompoundTag.hpp>
#include <CompoundTagVariant.hpp>
#include <Tag.hpp>

namespace bedrock_protocol {

std::unique_ptr<Tag> Tag::newTag(Type type) {
    switch (type) {
    case Type::Byte:
        return std::make_unique<ByteTag>();
    case Type::Short:
        return std::make_unique<ShortTag>();
    case Type::Int:
        return std::make_unique<IntTag>();
    case Type::Int64:
        return std::make_unique<Int64Tag>();
    case Type::Float:
        return std::make_unique<FloatTag>();
    case Type::Double:
        return std::make_unique<DoubleTag>();
    case Type::ByteArray:
        return std::make_unique<ByteArrayTag>();
    case Type::String:
        return std::make_unique<StringTag>();
    case Type::List:
        return std::make_unique<ListTag>();
    case Type::Compound:
        return std::make_unique<CompoundTag>();
    case Type::IntArray:
        return std::make_unique<IntArrayTag>();
    default:
        return std::make_unique<EndTag>();
    }
}

bool Tag::operator==(Tag const& other) const { return equals(other); }

} // namespace bedrock_protocol
