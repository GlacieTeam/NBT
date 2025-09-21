#include "nbt/ByteArrayTag.hpp"
#include "nbt/ByteTag.hpp"
#include "nbt/CompoundTag.hpp"
#include "nbt/CompoundTagVariant.hpp"
#include "nbt/DoubleTag.hpp"
#include "nbt/EndTag.hpp"
#include "nbt/FloatTag.hpp"
#include "nbt/Int64Tag.hpp"
#include "nbt/IntArrayTag.hpp"
#include "nbt/IntTag.hpp"
#include "nbt/ListTag.hpp"
#include "nbt/ShortTag.hpp"
#include "nbt/StringTag.hpp"
#include "nbt/detail/Base64.hpp"
#include "nbt/detail/StringUtils.hpp"
#include <format>
#include <limits>
#include <nlohmann/json.hpp>

namespace nbt {

namespace {
static constexpr std::string_view BASE64_TAG = " /*BASE64*/";

template <std::integral T>
std::string toString(T value) {
    return std::format("{}", value);
}

template <std::floating_point T>
std::string toString(T value) {
    if (std::round(value) == value) { return std::format("{:.1f}", value); }
    return std::format("{}", value);
}

bool isMinimize(SnbtFormat format) {
    return !(
        static_cast<bool>(format & SnbtFormat::CompoundLineFeed)
        || static_cast<bool>(format & SnbtFormat::ListArrayLineFeed)
        || static_cast<bool>(format & SnbtFormat::BinaryArrayLineFeed)
    );
}

std::string toDumpString(std::string const& str, SnbtFormat format, bool key) {
    std::string res;
    bool        base64 = false;
    if (str.empty()) {
        res = "\"\"";
    } else {
        bool isTrivial = true;
        if (!static_cast<bool>(format & SnbtFormat::ForceQuote)) {
            if (!key && (str[0] == '-' || isdigit(str[0]))) {
                isTrivial = false;
            } else {
                for (auto c : str) {
                    if (!(isalnum(c) || c == '-' || c == '+' || c == '_' || c == '.')) {
                        isTrivial = false;
                        break;
                    }
                }
            }
        } else {
            isTrivial = false;
        }
        if (isTrivial) {
            res = str;
        } else {
            try {
                res = nlohmann::json(std::move(str))
                          .dump(
                              -1,
                              ' ',
                              (bool)(format & SnbtFormat::ForceAscii),
                              nlohmann::json::error_handler_t::strict
                          );
            } catch (...) {
                base64 = true;
                res    = '\"' + base64_utils::encode(str) + '\"';
            }
        }
    }
    if (base64) { res += BASE64_TAG; }
    return res;
}

} // namespace

namespace detail {

template <typename T>
constexpr std::string makeSnbtTagValue(T value, SnbtFormat format, char mark) {
    bool upper = static_cast<bool>(format & SnbtFormat::ForceUppercase);
    if (static_cast<bool>(format & SnbtFormat::CommentMarks)) {
        return std::format("{0} /*{1:c}*/", value, upper ? std::toupper(mark) : mark);
    }
    return std::format("{0}{1:c}", value, upper ? std::toupper(mark) : mark);
}

std::string TypedToSnbt(EndTag const&, uint8_t, SnbtFormat) { return "null"; }

std::string TypedToSnbt(ByteTag const& self, uint8_t, SnbtFormat format) {
    return makeSnbtTagValue(self.storage(), format, 'b');
}

std::string TypedToSnbt(ShortTag const& self, uint8_t, SnbtFormat format) {
    return makeSnbtTagValue(self.storage(), format, 's');
}

std::string TypedToSnbt(IntTag const& self, uint8_t, SnbtFormat) { return toString(self.storage()); }

std::string TypedToSnbt(Int64Tag const& self, uint8_t, SnbtFormat format) {
    return makeSnbtTagValue(self.storage(), format, 'l');
}

std::string TypedToSnbt(FloatTag const& self, uint8_t, SnbtFormat format) {
    return makeSnbtTagValue(self.storage(), format, 'f');
}

std::string TypedToSnbt(DoubleTag const& self, uint8_t, SnbtFormat) { return toString(self.storage()); }

std::string TypedToSnbt(StringTag const& self, uint8_t, SnbtFormat format) {
    return toDumpString(self.storage(), format, false);
}

std::string TypedToSnbt(ListTag const& self, uint8_t indent, SnbtFormat format) {
    std::string res;

    static constexpr std::string_view lbracket{"["}, rbracket{"]"};

    res += lbracket;

    size_t      i = self.size();
    std::string indentSpace(indent, ' ');

    bool isMinimized = isMinimize(format);
    bool isNewLine   = static_cast<bool>(format & SnbtFormat::ListArrayLineFeed)
                  && (static_cast<bool>(format & SnbtFormat::ForceLineFeedIgnoreIndent) || (indent > 0));

    if (isNewLine && self.size() > 0) { res += '\n'; }
    for (auto& tag : self) {
        i--;
        if (isNewLine) { res += indentSpace; }
        auto key = tag->toSnbt(format, indent);

        if (isNewLine) { string_utils::replaceAll(key, "\n", "\n" + indentSpace); }
        res += key;

        if (i > 0) {
            res += ',';
            if (!isMinimized && !isNewLine) { res += ' '; }
        }
        if (isNewLine) { res += '\n'; }
    }

    res += rbracket;

    return res;
}

std::string TypedToSnbt(CompoundTag const& self, uint8_t indent, SnbtFormat format) {

    std::string res;

    static constexpr std::string_view lbracket{"{"}, rbracket{"}"};

    res += lbracket;

    size_t      i = self.size();
    std::string indentSpace(indent, ' ');

    bool isMinimized = isMinimize(format);
    bool isNewLine   = static_cast<bool>(format & SnbtFormat::CompoundLineFeed)
                  && (static_cast<bool>(format & SnbtFormat::ForceLineFeedIgnoreIndent) || (indent > 0));

    if (isNewLine && self.size() > 0) { res += '\n'; }

    for (auto& [k, v] : self.items()) {
        i--;
        if (isNewLine) { res += indentSpace; }
        auto keyStr = toDumpString(k, format, true);

        res += keyStr;

        res += ':';

        if (!isMinimized) { res += ' '; }
        auto key = v.toSnbt(format, indent);

        if (isNewLine) { string_utils::replaceAll(key, "\n", "\n" + indentSpace); }
        res += key;

        if (i > 0) {
            res += ',';
            if (!isMinimized && !isNewLine) { res += ' '; }
        }
        if (isNewLine) { res += '\n'; }
    }

    res += rbracket;

    return res;
}

std::string TypedToSnbt(ByteArrayTag const& self, uint8_t indent, SnbtFormat format) {

    std::string res;

    std::string lbracket{"[B;"}, rbracket{"]"};
    if (static_cast<bool>(format & SnbtFormat::CommentMarks)) { lbracket = "[ /*B;*/"; }

    res += lbracket;

    size_t      i = self.size();
    std::string indentSpace(indent, ' ');

    bool isMinimized = isMinimize(format);
    bool isNewLine   = static_cast<bool>(format & SnbtFormat::BinaryArrayLineFeed)
                  && (static_cast<bool>(format & SnbtFormat::ForceLineFeedIgnoreIndent) || (indent > 0));

    if (isNewLine && self.size() > 0) { res += '\n'; }

    for (auto& tag : self.storage()) {
        i--;
        if (isNewLine) { res += indentSpace; }
        res += makeSnbtTagValue(tag, format, 'b');

        if (i > 0) {
            res += ',';
            if (!isMinimized && !isNewLine) { res += ' '; }
        }
        if (isNewLine) { res += '\n'; }
    }

    res += rbracket;

    return res;
}

std::string TypedToSnbt(IntArrayTag const& self, uint8_t indent, SnbtFormat format) {

    std::string res;

    std::string lbracket{"[I;"}, rbracket{"]"};
    if (static_cast<bool>(format & SnbtFormat::CommentMarks)) { lbracket = "[ /*I;*/"; }

    res += lbracket;

    size_t      i = self.size();
    std::string indentSpace(indent, ' ');

    bool isMinimized = isMinimize(format);
    bool isNewLine   = static_cast<bool>(format & SnbtFormat::BinaryArrayLineFeed)
                  && (static_cast<bool>(format & SnbtFormat::ForceLineFeedIgnoreIndent) || (indent > 0));

    if (isNewLine && self.size() > 0) { res += '\n'; }

    for (auto& tag : self.storage()) {
        i--;
        if (isNewLine) { res += indentSpace; }

        res += toString(tag);

        if (i > 0) {
            res += ',';
            if (!isMinimized && !isNewLine) { res += ' '; }
        }
        if (isNewLine) { res += '\n'; }
    }

    res += rbracket;

    return res;
}

std::string TypedToSnbt(LongArrayTag const& self, uint8_t indent, SnbtFormat format) {

    std::string res;

    std::string lbracket{"[L;"}, rbracket{"]"};
    if (static_cast<bool>(format & SnbtFormat::CommentMarks)) { lbracket = "[ /*L;*/"; }

    res += lbracket;

    size_t      i = self.size();
    std::string indentSpace(indent, ' ');

    bool isMinimized = isMinimize(format);
    bool isNewLine   = static_cast<bool>(format & SnbtFormat::BinaryArrayLineFeed)
                  && (static_cast<bool>(format & SnbtFormat::ForceLineFeedIgnoreIndent) || (indent > 0));

    if (isNewLine && self.size() > 0) { res += '\n'; }

    for (auto& tag : self.storage()) {
        i--;
        if (isNewLine) { res += indentSpace; }
        res += makeSnbtTagValue(tag, format, 'l');

        if (i > 0) {
            res += ',';
            if (!isMinimized && !isNewLine) { res += ' '; }
        }
        if (isNewLine) { res += '\n'; }
    }

    res += rbracket;

    return res;
}

} // namespace detail

} // namespace nbt