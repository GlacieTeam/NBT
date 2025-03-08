#pragma once
#include <Tag.hpp>
#include <vector>

namespace bedrock_protocol {

class IntArrayTag : public Tag {
public:
    std::vector<int> mData;

public:
    IntArrayTag() = default;
    IntArrayTag(std::vector<int> const& data);

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;
};

} // namespace bedrock_protocol
