#include <CompoundTagVariant.hpp>
#include <ListTag.hpp>

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

} // namespace bedrock_protocol