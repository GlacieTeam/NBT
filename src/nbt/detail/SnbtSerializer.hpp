#pragma once
#include "nbt/SnbtFormat.hpp"
#include <string>

namespace nbt {

class CompoundTag;
class EndTag;
class ByteTag;
class ShortTag;
class IntTag;
class Int64Tag;
class FloatTag;
class DoubleTag;
class ByteArrayTag;
class StringTag;
class ListTag;
class IntArrayTag;
class LongArrayTag;

namespace detail {

std::string TypedToSnbt(ByteTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(ShortTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(IntTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(Int64Tag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(FloatTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(DoubleTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(ByteArrayTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(StringTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(ListTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(CompoundTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(IntArrayTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(LongArrayTag const&, uint8_t, SnbtFormat);
std::string TypedToSnbt(EndTag const&, uint8_t, SnbtFormat);

} // namespace detail

} // namespace nbt
