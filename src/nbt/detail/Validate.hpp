#pragma once
#include "nbt/CompoundTagVariant.hpp"

namespace nbt::detail {

bool validateCompoundTag(io::BytesDataInput& stream, size_t streamSize);

bool validateCompoundTag(bstream::ReadOnlyBinaryStream& stream, size_t streamSize);

} // namespace nbt::detail
