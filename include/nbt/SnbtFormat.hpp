#pragma once
#include <utility>

namespace nbt {

enum class SnbtFormat : uint8_t {
    Minimize                  = 0,
    CompoundLineFeed          = 1 << 0,
    ListArrayLineFeed         = 1 << 1,
    BinaryArrayLineFeed       = 1 << 2,
    ForceLineFeedIgnoreIndent = 1 << 3,
    ForceAscii                = 1 << 4,
    ForceQuote                = 1 << 5,
    ForceUppercase            = 1 << 6,
    CommentMarks              = 1 << 7,
    ArrayLineFeed             = ListArrayLineFeed | BinaryArrayLineFeed,
    AlwaysLineFeed            = CompoundLineFeed | ArrayLineFeed,
    PrettyFilePrint           = CompoundLineFeed | ListArrayLineFeed,
    Classic                   = PrettyFilePrint | ForceQuote,
    Jsonify                   = AlwaysLineFeed | ForceQuote | CommentMarks,
    Default                   = PrettyFilePrint,
};

[[nodiscard]] constexpr SnbtFormat operator|(const SnbtFormat lhs, const SnbtFormat rhs) noexcept {
    return static_cast<SnbtFormat>(std::to_underlying(lhs) | std::to_underlying(rhs));
}

[[nodiscard]] constexpr SnbtFormat operator&(const SnbtFormat lhs, const SnbtFormat rhs) noexcept {
    return static_cast<SnbtFormat>(std::to_underlying(lhs) & std::to_underlying(rhs));
}

} // namespace nbt