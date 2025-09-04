#pragma once
#include <string>

namespace nbt::string_utils {

constexpr std::string& replaceAll(std::string& str, std::string_view oldValue, std::string_view newValue) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos += newValue.length()) {
        if ((pos = str.find(oldValue, pos)) != std::string::npos) str.replace(pos, oldValue.length(), newValue);
        else break;
    }
    return str;
}

[[nodiscard]] inline std::string
replaceAll(std::string const& str, std::string_view oldValue, std::string_view newValue) {
    std::string ret = str;
    replaceAll(ret, oldValue, newValue);
    return ret;
}

[[nodiscard]] constexpr size_t doHash(std::string_view x) {
    size_t           hash  = 0xcbf29ce484222325;
    constexpr size_t prime = 0x100000001b3;
    for (char c : x) {
        hash ^= static_cast<size_t>(c);
        hash *= prime;
    }
    return hash;
}

} // namespace nbt::string_utils
