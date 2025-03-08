#include <CompoundTag.hpp>
#include <CompoundTagVariant.hpp>

namespace bedrock_protocol {

CompoundTagVariant::CompoundTagVariant(Tag&& tag) { emplace(std::move(tag)); }

CompoundTagVariant::CompoundTagVariant(std::unique_ptr<Tag> tag) { emplace(std::move(*tag)); }

Tag& CompoundTagVariant::emplace(Tag&& tag) {
    switch (tag.getType()) {
    case Tag::Type::Byte:
        return emplace<ByteTag>(static_cast<ByteTag&&>(tag));
    case Tag::Type::Short:
        return emplace<ShortTag>(static_cast<ShortTag&&>(tag));
    case Tag::Type::Int:
        return emplace<IntTag>(static_cast<IntTag&&>(tag));
    case Tag::Type::Int64:
        return emplace<Int64Tag>(static_cast<Int64Tag&&>(tag));
    case Tag::Type::Float:
        return emplace<FloatTag>(static_cast<FloatTag&&>(tag));
    case Tag::Type::Double:
        return emplace<DoubleTag>(static_cast<DoubleTag&&>(tag));
    case Tag::Type::ByteArray:
        return emplace<ByteArrayTag>(static_cast<ByteArrayTag&&>(tag));
    case Tag::Type::String:
        return emplace<StringTag>(static_cast<StringTag&&>(tag));
    case Tag::Type::List:
        return emplace<ListTag>(static_cast<ListTag&&>(tag));
    case Tag::Type::Compound:
        return emplace<CompoundTag>(static_cast<CompoundTag&&>(tag));
    case Tag::Type::IntArray:
        return emplace<IntArrayTag>(static_cast<IntArrayTag&&>(tag));
    case Tag::Type::End:
    default:
        return emplace<EndTag>();
    }
}

const Tag* CompoundTagVariant::operator->() const { return get(); }

Tag* CompoundTagVariant::operator->() { return get(); }

const Tag* CompoundTagVariant::get() const {
    return std::visit([](auto&& arg) -> const Tag* { return &arg; }, mStorage);
}

Tag* CompoundTagVariant::get() {
    return std::visit([](auto&& arg) -> Tag* { return &arg; }, mStorage);
}

const Tag& CompoundTagVariant::operator*() const { return *get(); }

Tag& CompoundTagVariant::operator*() { return *get(); }

bool CompoundTagVariant::operator==(CompoundTagVariant const& other) const { return get()->equals(*other.get()); }

} // namespace bedrock_protocol
