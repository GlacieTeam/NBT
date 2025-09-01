#pragma once
#include <cstdint>

namespace nbt::literals {

constexpr uint8_t operator""_b(unsigned long long value) noexcept { return static_cast<uint8_t>(value); }

constexpr short operator""_s(unsigned long long value) noexcept { return static_cast<short>(value); }

constexpr int64_t operator""_l(unsigned long long value) noexcept { return static_cast<int64_t>(value); }

} // namespace nbt::literals
