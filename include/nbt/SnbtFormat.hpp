#pragma once
#include <cstdint>
#include <type_traits>

namespace bedrock_protocol {

enum class SnbtFormat : uint8_t {
    Minimize        = 0,
    PrettyFilePrint = 1 << 0,
    ArrayLineFeed   = 1 << 1,
    ForceAscii      = 1 << 2,
    ForceQuote      = 1 << 3,
    CommentMarks    = 1 << 4,
    Classic         = ForceQuote | ArrayLineFeed,
    Jsonify         = ForceQuote | CommentMarks,
    AlwaysLineFeed  = PrettyFilePrint | ArrayLineFeed
};

[[nodiscard]] constexpr SnbtFormat operator|(const SnbtFormat l, const SnbtFormat r) noexcept {
    return static_cast<SnbtFormat>(
        static_cast<std::underlying_type_t<SnbtFormat>>(l) | static_cast<std::underlying_type_t<SnbtFormat>>(r)
    );
}

[[nodiscard]] constexpr SnbtFormat operator&(const SnbtFormat l, const SnbtFormat r) noexcept {
    return static_cast<SnbtFormat>(
        static_cast<std::underlying_type_t<SnbtFormat>>(l) & static_cast<std::underlying_type_t<SnbtFormat>>(r)
    );
}

} // namespace bedrock_protocol