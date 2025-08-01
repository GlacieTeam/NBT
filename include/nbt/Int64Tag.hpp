#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class Int64Tag : public Tag {
protected:
    int64_t mData;

public:
    Int64Tag() = default;
    Int64Tag(int64_t data);

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    int64_t&       data();
    int64_t const& data() const;
};

} // namespace bedrock_protocol
