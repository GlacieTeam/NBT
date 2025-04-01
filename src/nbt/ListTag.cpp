#include <nbt/CompoundTagVariant.hpp>
#include <nbt/ListTag.hpp>

namespace bedrock_protocol {

ListTag::ListTag(std::vector<CompoundTagVariant> const& data) : mData(std::move(data)) {
    if (!mData.empty()) { mType = mData.front()->getType(); }
}

Tag::Type ListTag::getType() const { return Tag::Type::List; }

bool ListTag::equals(const Tag& other) const {
    return (other.getType() == Tag::Type::List) && (mData == static_cast<const ListTag&>(other).mData);
}

std::unique_ptr<Tag> ListTag::copy() const { return copyList(); }

std::size_t ListTag::hash() const {
    std::size_t hash = 0;
    for (auto& value : mData) {
        std::size_t element_hash  = value.get()->hash();
        hash                     ^= element_hash + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
    return hash;
}

std::unique_ptr<ListTag> ListTag::copyList() const {
    auto copy   = std::make_unique<ListTag>();
    copy->mType = mType;
    for (const auto& data : mData) { copy->mData.push_back(data.get()->copy()); }
    return copy;
}

void ListTag::write(BytesDataOutput& stream) const {
    stream.writeByte((uint8_t)mType);
    stream.writeInt((int)mData.size());
    for (const auto& data : mData) { data->write(stream); }
}

void ListTag::load(BytesDataInput& stream) {
    mType     = Tag::Type(stream.getByte());
    auto size = stream.getInt();
    mData.clear();
    for (int i = 0; i < size; ++i) {
        auto tag = newTag(mType);
        if (tag) {
            tag->load(stream);
            mData.push_back(std::move(tag));
        }
    }
}

void ListTag::write(BinaryStream& stream) const {
    stream.writeByte((uint8_t)mType);
    stream.writeVarInt((int)mData.size());
    for (const auto& data : mData) { data->write(stream); }
}

void ListTag::load(ReadOnlyBinaryStream& stream) {
    mType     = Tag::Type(stream.getByte());
    auto size = stream.getVarInt();
    mData.clear();
    for (int i = 0; i < size; ++i) {
        auto tag = newTag(mType);
        if (tag) {
            tag->load(stream);
            mData.push_back(std::move(tag));
        }
    }
}

void ListTag::add(std::unique_ptr<Tag>&& tag) {
    if (mData.empty()) { mType = tag->getType(); }
    mData.push_back(std::move(tag));
}

void ListTag::forEachCompoundTag(std::function<void(CompoundTag const& tag)> func) {
    if (mType == Tag::Type::Compound) {
        for (auto& nbt : mData) { func(nbt); }
    }
}

size_t ListTag::size() const { return mData.size(); }

Tag&       ListTag::operator[](size_t index) { return mData[index].as<Tag>(); }
Tag const& ListTag::operator[](size_t index) const { return mData[index].as<Tag>(); }

ListTag::iterator ListTag::begin() noexcept { return mData.begin(); }
ListTag::iterator ListTag::end() noexcept { return mData.end(); }

ListTag::reverse_iterator ListTag::rbegin() noexcept { return mData.rbegin(); }
ListTag::reverse_iterator ListTag::rend() noexcept { return mData.rend(); }

ListTag::const_iterator ListTag::begin() const noexcept { return cbegin(); }
ListTag::const_iterator ListTag::end() const noexcept { return cend(); }

ListTag::const_iterator ListTag::cbegin() const noexcept { return mData.cbegin(); }
ListTag::const_iterator ListTag::cend() const noexcept { return mData.cend(); }

ListTag::const_reverse_iterator ListTag::crbegin() const noexcept { return mData.crbegin(); }
ListTag::const_reverse_iterator ListTag::crend() const noexcept { return mData.crend(); }

} // namespace bedrock_protocol