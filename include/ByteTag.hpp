#pragma once
#include <Tag.hpp>

namespace bedrock_protocol {

class ByteTag : public Tag {
public:
    uint8_t mData;

public:
    ByteTag() = default;
    ByteTag(uint8_t data);

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;
};

} // namespace bedrock_protocol
