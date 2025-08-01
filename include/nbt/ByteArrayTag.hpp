#pragma once
#include <nbt/Tag.hpp>
#include <vector>

namespace bedrock_protocol {

class ByteArrayTag : public Tag {
protected:
    std::vector<uint8_t> mData;

public:
    [[nodiscard]] constexpr ByteArrayTag() = default;

    [[nodiscard]] constexpr ByteArrayTag(std::vector<uint8_t> arr) : mData(std::move(arr)) {}

    [[nodiscard]] constexpr ByteArrayTag(std::initializer_list<uint8_t> val) : mData(val) {}

    [[nodiscard]] Type getType() const override;

    [[nodiscard]] bool equals(Tag const& other) const override;

    [[nodiscard]] std::unique_ptr<Tag> copy() const override;

    [[nodiscard]] std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

public:
    std::vector<uint8_t>&       data();
    std::vector<uint8_t> const& data() const;

    size_t size() const;
};

} // namespace bedrock_protocol
