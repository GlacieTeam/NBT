#pragma once
#include <Tag.hpp>
#include <vector>

namespace bedrock_protocol {

class CompoundTagVariant;

class ListTag : public Tag {
public:
    std::vector<CompoundTagVariant> mData;
    Type                            mType{Type::End};

public:
    ListTag() = default;
    ListTag(std::vector<CompoundTagVariant> const& data);

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    std::unique_ptr<ListTag> copyList() const;
};

} // namespace bedrock_protocol