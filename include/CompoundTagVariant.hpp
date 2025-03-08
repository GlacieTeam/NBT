#pragma once
#include <CompoundTag.hpp>
#include <Tag.hpp>
#include <variant>

namespace bedrock_protocol {

class CompoundTagVariant {
public:
    using TagVariant = std::variant<
        EndTag,
        ByteTag,
        ShortTag,
        IntTag,
        Int64Tag,
        FloatTag,
        DoubleTag,
        ByteArrayTag,
        StringTag,
        ListTag,
        CompoundTag,
        IntArrayTag>;

public:
    TagVariant mStorage;

public:
    CompoundTagVariant() = default;
    CompoundTagVariant(Tag&& tag);
    CompoundTagVariant(std::unique_ptr<Tag> tag);

    template <typename T, typename... Args>
    T& emplace(Args&&... args) {
        mStorage = T(std::forward<Args>(args)...);
        return std::get<T>(mStorage);
    }

    Tag& emplace(Tag&& tag);

    const Tag* operator->() const;
    Tag*       operator->();

    const Tag* get() const;
    Tag*       get();

    const Tag& operator*() const;
    Tag&       operator*();

    bool operator==(CompoundTagVariant const& other) const;
};

} // namespace bedrock_protocol
