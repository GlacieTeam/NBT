#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class StringTag : public Tag {
public:
    std::string mData;

public:
    StringTag() = default;
    StringTag(std::string const& data);

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
