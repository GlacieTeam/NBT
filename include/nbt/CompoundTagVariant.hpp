#pragma once
#include <nbt/CompoundTag.hpp>
#include <nbt/Tag.hpp>
#include <utility>
#include <variant>

namespace bedrock_protocol {

class CompoundTagVariant {
public:
    using TagVariant = std::variant<
        EndTag,
        ByteTag,
        ShortTag,
        IntTag,
        Int64Tag,
        FloatTag,
        DoubleTag,
        ByteArrayTag,
        StringTag,
        ListTag,
        CompoundTag,
        IntArrayTag>;

public:
    TagVariant mStorage;

public:
    template <bool Const>
    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = ptrdiff_t;
        using value_type        = Tag;
        using reference         = std::conditional_t<Const, value_type const, value_type>&;
        using pointer           = std::add_pointer_t<reference>;

        std::conditional_t<
            Const,
            std::variant<CompoundTagVariant const*, CompoundTag::const_iterator, ListTag::const_iterator>,
            std::variant<CompoundTagVariant*, CompoundTag::iterator, ListTag::iterator>>
            iter;

        static Iterator makeBegin(auto& var) noexcept {
            Iterator res;
            switch (var.index()) {
            case Tag::Type::List:
                res.iter.emplace<2>(var.as<ListTag>().begin());
                break;
            case Tag::Type::Compound:
                res.iter.emplace<1>(var.as<CompoundTag>().begin());
                break;
            case Tag::Type::End:
                res.iter.emplace<0>(std::addressof(var) + 1);
                break;
            default:
                res.iter.emplace<0>(std::addressof(var));
            }
            return res;
        }

        static Iterator makeEnd(auto& var) noexcept {
            Iterator res;
            switch (var.index()) {
            case Tag::Type::List:
                res.iter.emplace<2>(var.as<ListTag>().end());
                break;
            case Tag::Type::Compound:
                res.iter.emplace<1>(var.as<CompoundTag>().end());
                break;
            default:
                res.iter.emplace<0>(std::addressof(var) + 1);
            }
            return res;
        }

        [[nodiscard]] reference operator*() const noexcept {
            switch (iter.index()) {
            case 0:
                return std::get<0>(iter)->get();
            case 1:
                return std::get<1>(iter)->second.get();
            case 2:
                return *std::get<2>(iter)->get();
            default:
                std::unreachable();
            }
            std::unreachable();
        }

        [[nodiscard]] pointer operator->() const noexcept { return std::addressof(**this); }

        Iterator& operator++() noexcept {
            std::visit([](auto& val) { ++val; }, iter);
            return *this;
        }

        Iterator operator++(int) noexcept {
            Iterator tmp = *this;
            ++*this;
            return tmp;
        }

        Iterator& operator--() noexcept {
            std::visit([](auto& val) { --val; }, iter);
            return *this;
        }

        Iterator operator--(int) noexcept {
            Iterator tmp = *this;
            --*this;
            return tmp;
        }

        [[nodiscard]] bool operator==(Iterator const& r) const noexcept { return this->iter == r.iter; }
    };

public:
    [[nodiscard]] CompoundTagVariant(Tag&& tag);
    [[nodiscard]] CompoundTagVariant(std::unique_ptr<Tag> tag);

    [[nodiscard]] constexpr CompoundTagVariant()                          = default;
    [[nodiscard]] constexpr CompoundTagVariant(CompoundTagVariant&&)      = default;
    [[nodiscard]] constexpr CompoundTagVariant(CompoundTagVariant const&) = default;

    CompoundTagVariant& operator=(CompoundTagVariant&&)      = default;
    CompoundTagVariant& operator=(CompoundTagVariant const&) = default;

    template <class T>
        requires(requires(T o) { mStorage = std::move(o); })
    constexpr CompoundTagVariant& operator=(T other) {
        mStorage = std::move(other);
        return *this;
    }

public:
    template <typename T, typename... Args>
    T& emplace(Args&&... args) {
        mStorage = T(std::forward<Args>(args)...);
        return std::get<T>(mStorage);
    }

    Tag& emplace(Tag&& tag);

    template <std::derived_from<Tag> T>
    [[nodiscard]] T& as() {
        return *(T*)get();
    }

    template <std::derived_from<Tag> T>
    [[nodiscard]] T const& as() const {
        return *(T*)get();
    }

    [[nodiscard]] const Tag* operator->() const;
    [[nodiscard]] Tag*       operator->();

    [[nodiscard]] const Tag* get() const;
    [[nodiscard]] Tag*       get();

    [[nodiscard]] const Tag& operator*() const;
    [[nodiscard]] Tag&       operator*();

    [[nodiscard]] bool operator==(CompoundTagVariant const& other) const;

public:
    using iterator       = Iterator<false>;
    using const_iterator = Iterator<true>;

    [[nodiscard]] constexpr CompoundTagVariant(std::nullptr_t) {}

    [[nodiscard]] CompoundTagVariant(TagVariant tag) : mStorage(std::move(tag)) {}

    template <class T, class... Args>
    [[nodiscard]] constexpr CompoundTagVariant(std::in_place_type_t<T>, Args&&... args)
    : mStorage(std::in_place_type<T>, std::forward<Args>(args)...) {}

    [[nodiscard]] constexpr CompoundTagVariant(std::initializer_list<CompoundTag::TagMap::value_type> tagPairs)
    : mStorage(std::in_place_type<CompoundTag>, tagPairs) {}

    [[nodiscard]] CompoundTagVariant(Tag const& tag) : CompoundTagVariant(tag.copy()) {}
    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr CompoundTagVariant(T tag) : mStorage(std::move(tag)) {}
    template <std::integral T>
    [[nodiscard]] constexpr CompoundTagVariant(T integer) {
        constexpr size_t size = sizeof(T);
        if constexpr (size == 1) {
            mStorage = ByteTag{integer};
        } else if constexpr (size == 2) {
            mStorage = ShortTag{integer};
        } else if constexpr (size == 4) {
            mStorage = IntTag{integer};
        } else {
            mStorage = Int64Tag{integer};
        }
    }
    [[nodiscard]] constexpr CompoundTagVariant(std::byte b) : mStorage(ByteTag{b}) {}

    [[nodiscard]] constexpr CompoundTagVariant(float f) : mStorage(FloatTag{f}) {}

    [[nodiscard]] constexpr CompoundTagVariant(double d) : mStorage(DoubleTag{d}) {}

    [[nodiscard]] constexpr CompoundTagVariant(std::string s) : mStorage(std::in_place_type<StringTag>, std::move(s)) {}

    [[nodiscard]] constexpr CompoundTagVariant(std::string_view s)
    : mStorage(std::in_place_type<StringTag>, std::string(s)) {}

    template <size_t N>
    [[nodiscard]] constexpr CompoundTagVariant(char const (&str)[N])
    : CompoundTagVariant(std::string_view{str, N - 1}) {}

    [[nodiscard]] iterator       begin() noexcept { return iterator::makeBegin(*this); }
    [[nodiscard]] const_iterator begin() const noexcept { return cbegin(); }
    [[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator::makeBegin(*this); }

    [[nodiscard]] iterator       end() noexcept { return iterator::makeEnd(*this); }
    [[nodiscard]] const_iterator end() const noexcept { return cend(); }
    [[nodiscard]] const_iterator cend() const noexcept { return const_iterator::makeEnd(*this); }

    [[nodiscard]] constexpr Tag::Type index() const noexcept { return Tag::Type(mStorage.index()); }
    [[nodiscard]] constexpr Tag::Type getId() const noexcept { return index(); }

    template <std::derived_from<Tag> T>
    [[nodiscard]] constexpr bool hold() const noexcept {
        return std::holds_alternative<T>(mStorage);
    }
    [[nodiscard]] constexpr bool hold(Tag::Type type) const noexcept { return getId() == type; }

    [[nodiscard]] constexpr bool is_array() const noexcept { return hold(Tag::Type::List); }
    [[nodiscard]] constexpr bool is_binary() const noexcept {
        return hold(Tag::Type::ByteArray) || hold(Tag::Type::IntArray);
    }
    [[nodiscard]] constexpr bool is_boolean() const noexcept { return hold(Tag::Type::Byte); }
    [[nodiscard]] constexpr bool is_null() const noexcept { return hold(Tag::Type::End); }
    [[nodiscard]] constexpr bool is_number_float() const noexcept {
        return hold(Tag::Type::Float) || hold(Tag::Type::Double);
    }
    [[nodiscard]] constexpr bool is_number_integer() const noexcept {
        return hold(Tag::Type::Byte) || hold(Tag::Type::Short) || hold(Tag::Type::Int) || hold(Tag::Type::Int64);
    }
    [[nodiscard]] constexpr bool is_object() const noexcept { return hold(Tag::Type::Compound); }
    [[nodiscard]] constexpr bool is_string() const noexcept { return hold(Tag::Type::String); }
    [[nodiscard]] constexpr bool is_number() const noexcept { return is_number_float() || is_number_integer(); }
    [[nodiscard]] constexpr bool is_primitive() const noexcept {
        return is_null() || is_string() || is_number() || is_binary();
    }
    [[nodiscard]] constexpr bool is_structured() const noexcept { return is_array() || is_object(); }

    [[nodiscard]] CompoundTag::TagMap const& items() const { return as<CompoundTag>().mTagMap; }
    [[nodiscard]] CompoundTag::TagMap&       items() { return as<CompoundTag>().mTagMap; }

    [[nodiscard]] bool contains(std::string_view key) const noexcept {
        if (is_object()) { return as<CompoundTag>().contains(std::string(key)); }
        return false;
    }

    [[nodiscard]] bool contains(std::string_view key, Tag::Type type) const noexcept {
        if (is_object()) { return as<CompoundTag>().contains(std::string(key), type); }
        return false;
    }

    [[nodiscard]] constexpr size_t size() const noexcept {
        switch (index()) {
        case Tag::Type::Byte:
        case Tag::Type::Short:
        case Tag::Type::Int:
        case Tag::Type::Int64:
        case Tag::Type::Float:
        case Tag::Type::Double:
        case Tag::Type::String:
            return 1;
        case Tag::Type::List:
            return as<ListTag>().size();
        case Tag::Type::Compound:
            return as<CompoundTag>().size();
        case Tag::Type::IntArray:
            return as<IntArrayTag>().size();
        case Tag::Type::ByteArray:
            return as<ByteArrayTag>().size();
        case Tag::Type::End:
            return 0;
        default:
            std::unreachable();
        }
    }

    [[nodiscard]] Tag& operator[](size_t index) {
        if (hold(Tag::Type::List)) {
            return as<ListTag>()[index];
        } else {
            throw std::runtime_error("tag not hold an array");
        }
    }

    [[nodiscard]] Tag const& operator[](size_t index) const {
        if (hold(Tag::Type::List)) {
            return as<ListTag>()[index];
        } else {
            throw std::runtime_error("tag not hold an array");
        }
    }

    [[nodiscard]] CompoundTagVariant& operator[](std::string_view index) {
        if (is_null()) { mStorage = CompoundTag{}; }
        if (!hold(Tag::Type::Compound)) { throw std::runtime_error("tag not hold an object"); }
        return as<CompoundTag>()[index];
    }

    [[nodiscard]] CompoundTagVariant const& operator[](std::string_view index) const {
        if (!hold(Tag::Type::Compound)) { throw std::runtime_error("tag not hold an object"); }
        return as<CompoundTag>()[index];
    }

    template <size_t N>
    [[nodiscard]] CompoundTagVariant& operator[](char const (&index)[N]) {
        return operator[](std::string_view{index, N - 1});
    }

    template <size_t N>
    [[nodiscard]] CompoundTagVariant const& operator[](char const (&index)[N]) const {
        return operator[](std::string_view{index, N - 1});
    }

    [[nodiscard]] std::unique_ptr<Tag> toUniqueCopy() const& {
        return std::visit(
            [](auto& val) -> std::unique_ptr<Tag> { return std::make_unique<std::decay_t<decltype(val)>>(val); },
            mStorage
        );
    }

    [[nodiscard]] std::unique_ptr<Tag> toUnique() && {
        return std::visit(
            [](auto&& val) -> std::unique_ptr<Tag> {
                return std::make_unique<std::decay_t<decltype(val)>>(std::move(val));
            },
            mStorage
        );
    }

    void push_back(CompoundTagVariant val) {
        if (is_null()) { mStorage = ListTag{}; }
        if (!hold(Tag::Type::List)) { throw std::runtime_error("tag not hold an array"); }
        as<ListTag>().add(std::move(val).toUnique());
    }

    template <class T>
    [[nodiscard]] constexpr operator T() const {
        if (is_number()) {
            return std::visit(
                [](auto& val) -> T {
                    if constexpr (std::is_convertible_v<std::decay_t<decltype(val)>, T>) {
                        return (T)val;
                    } else {
                        return {};
                    }
                },
                mStorage
            );
        } else {
            throw std::runtime_error("tag not hold an number");
        }
    }

    [[nodiscard]] operator uint8_t const&() const { return as<ByteTag>().data(); }
    [[nodiscard]] operator uint8_t&() { return as<ByteTag>().data(); }
    [[nodiscard]] operator uint8_t&&() && { return std::move(as<ByteTag>().data()); }

    [[nodiscard]] operator short const&() const { return as<ShortTag>().data(); }
    [[nodiscard]] operator short&() { return as<ShortTag>().data(); }
    [[nodiscard]] operator short&&() && { return std::move(as<ShortTag>().data()); }

    [[nodiscard]] operator int const&() const { return as<IntTag>().data(); }
    [[nodiscard]] operator int&() { return as<IntTag>().data(); }
    [[nodiscard]] operator int&&() && { return std::move(as<IntTag>().data()); }

    [[nodiscard]] operator int64_t const&() const { return as<Int64Tag>().data(); }
    [[nodiscard]] operator int64_t&() { return as<Int64Tag>().data(); }
    [[nodiscard]] operator int64_t&&() && { return std::move(as<Int64Tag>().data()); }

    [[nodiscard]] operator float const&() const { return as<FloatTag>().data(); }
    [[nodiscard]] operator float&() { return as<FloatTag>().data(); }
    [[nodiscard]] operator float&&() && { return std::move(as<FloatTag>().data()); }

    [[nodiscard]] operator double const&() const { return as<DoubleTag>().data(); }
    [[nodiscard]] operator double&() { return as<DoubleTag>().data(); }
    [[nodiscard]] operator double&&() && { return std::move(as<DoubleTag>().data()); }

    [[nodiscard]] operator std::string const&() const { return as<StringTag>().data(); }
    [[nodiscard]] operator std::string&() { return as<StringTag>().data(); }
    [[nodiscard]] operator std::string&&() && { return std::move(as<StringTag>().data()); }
    [[nodiscard]] operator std::string_view() const { return as<StringTag>().data(); }

    [[nodiscard]] operator std::vector<uint8_t> const&() const { return as<ByteArrayTag>().data(); }
    [[nodiscard]] operator std::vector<uint8_t>&() { return as<ByteArrayTag>().data(); }
    [[nodiscard]] operator std::vector<uint8_t>&&() && { return std::move(as<ByteArrayTag>().data()); }

    [[nodiscard]] operator std::vector<int> const&() const { return as<IntArrayTag>().data(); }
    [[nodiscard]] operator std::vector<int>&() { return as<IntArrayTag>().data(); }
    [[nodiscard]] operator std::vector<int>&&() && { return std::move(as<IntArrayTag>().data()); }

    [[nodiscard]] operator CompoundTag const&() const { return as<CompoundTag>(); }
    [[nodiscard]] operator CompoundTag&() { return as<CompoundTag>(); }
    [[nodiscard]] operator CompoundTag&&() && { return std::move(as<CompoundTag>()); }

    [[nodiscard]] operator ListTag const&() const { return as<ListTag>(); }
    [[nodiscard]] operator ListTag&() { return as<ListTag>(); }
    [[nodiscard]] operator ListTag&&() && { return std::move(as<ListTag>()); }

    static CompoundTagVariant object(std::initializer_list<CompoundTag::TagMap::value_type> init = {}) {
        return CompoundTagVariant{std::in_place_type<CompoundTag>, init};
    }

    static CompoundTagVariant array(std::initializer_list<CompoundTagVariant> init = {}) {
        return CompoundTagVariant{std::in_place_type<ListTag>, init};
    }
};

} // namespace bedrock_protocol
