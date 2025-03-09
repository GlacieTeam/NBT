#pragma once
#include <Tag.hpp>

namespace bedrock_protocol {

class ShortTag : public Tag {
public:
    short mData;

public:
    ShortTag() = default;
    ShortTag(short data);

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;
};

} // namespace bedrock_protocol
