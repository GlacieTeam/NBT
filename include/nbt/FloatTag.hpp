#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class FloatTag : public Tag {
protected:
    float mData;

public:
    FloatTag() = default;
    FloatTag(float data);

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    float&       data();
    float const& data() const;
};

} // namespace bedrock_protocol
