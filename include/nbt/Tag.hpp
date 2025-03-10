#pragma once
#include <binarystream/BinaryStream.hpp>
#include <nbt/io/BytesDataOutput.hpp>

namespace bedrock_protocol {

class Tag {
public:
    enum class Type : uint8_t {
        End         = 0,
        Byte        = 1,
        Short       = 2,
        Int         = 3,
        Int64       = 4,
        Float       = 5,
        Double      = 6,
        ByteArray   = 7,
        String      = 8,
        List        = 9,
        Compound    = 10,
        IntArray    = 11,
        NumTagTypes = 12,
    };

    virtual ~Tag() = default;

    virtual Type getType() const = 0;

    virtual bool equals(Tag const& other) const = 0;

    virtual std::unique_ptr<Tag> copy() const = 0;

    virtual std::size_t hash() const = 0;

    virtual void write(BytesDataOutput& stream) const = 0;

    virtual void load(BytesDataInput& stream) = 0;

    virtual void write(BinaryStream& stream) const = 0;

    virtual void load(ReadOnlyBinaryStream& stream) = 0;

public:
    static std::unique_ptr<Tag> newTag(Type type);

    bool operator==(Tag const& other) const;
};

} // namespace bedrock_protocol
