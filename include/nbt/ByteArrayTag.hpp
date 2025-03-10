#pragma once
#include <nbt/Tag.hpp>
#include <vector>

namespace bedrock_protocol {

class ByteArrayTag : public Tag {
public:
    std::vector<uint8_t> mData;

public:
    ByteArrayTag() = default;
    ByteArrayTag(std::vector<uint8_t> const& data);

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
