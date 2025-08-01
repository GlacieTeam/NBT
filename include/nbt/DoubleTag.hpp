#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class DoubleTag : public Tag {
protected:
    double mData;

public:
    DoubleTag() = default;
    DoubleTag(double data);

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    double&       data();
    double const& data() const;
};

} // namespace bedrock_protocol
