#pragma once
#include <optional>
#include <string_view>

namespace bedrock_protocol {

class CompoundTagVariant;

namespace detail {

std::optional<CompoundTagVariant> parseSnbtValue(std::string_view& s) noexcept;
std::optional<CompoundTagVariant> parseSnbtValueNonSkip(std::string_view& s);

} // namespace detail

} // namespace bedrock_protocol
