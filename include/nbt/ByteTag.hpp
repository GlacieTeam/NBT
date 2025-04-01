#pragma once
#include <nbt/Tag.hpp>

namespace bedrock_protocol {

class ByteTag : public Tag {
protected:
    uint8_t mData;

public:
    [[nodiscard]] ByteTag() = default;
    [[nodiscard]] ByteTag(uint8_t data);

    template <std::integral T>
    constexpr ByteTag& operator=(T value) {
        data = (uint8_t)value;
        return *this;
    }

    template <std::integral T>
    [[nodiscard]] constexpr operator T() const {
        return (T)data;
    }

    [[nodiscard]] constexpr operator std::byte() const { return (std::byte)mData; }

    template <std::integral T>
    [[nodiscard]] constexpr explicit ByteTag(T value = 0) : mData((uint8_t)value) {}

    [[nodiscard]] constexpr explicit ByteTag(std::byte b) : mData(std::to_integer<uint8_t>(b)) {}

    [[nodiscard]] ByteTag operator-() const { return ByteTag{-mData}; }

    Type getType() const override;

    bool equals(Tag const& other) const override;

    std::unique_ptr<Tag> copy() const override;

    std::size_t hash() const override;

    void write(BytesDataOutput& stream) const override;

    void load(BytesDataInput& stream) override;

    void write(BinaryStream& stream) const override;

    void load(ReadOnlyBinaryStream& stream) override;

    uint8_t&       data();
    uint8_t const& data() const;
};

} // namespace bedrock_protocol
