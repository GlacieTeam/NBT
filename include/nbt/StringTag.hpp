#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class StringTag : public Tag {
protected:
    std::string mData;

public:
    StringTag() = default;
    StringTag(std::string_view data);

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

public:
    std::string&       data();
    std::string const& data() const;
    std::string_view   view() const;
};

} // namespace bedrock_protocol
