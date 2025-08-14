#include "nbt-c/NBT.h"
#include "nbt/CompoundTagVariant.hpp"

namespace {

inline bedrock_protocol::Tag* toTag(void* handle) { return reinterpret_cast<bedrock_protocol::Tag*>(handle); }

} // namespace

extern "C" {

void nbtio_buffer_destroy(nbtio_buffer* buffer) {
    if (buffer && buffer->data) {
        delete[] buffer->data;
        buffer->data = nullptr;
        buffer->size = 0;
    }
}

// Any Tag
TagType nbt_any_tag_get_type(void* handle) {
    if (!handle) { return TagType::Tag_End; }
    return static_cast<TagType>(toTag(handle)->getType());
}

bool nbt_any_tag_equals(void* handle, void* other) {
    if (!handle) { return false; }
    return toTag(handle)->equals(*toTag(other));
}

void* nbt_any_tag_copy(void* handle) {
    if (!handle) { return nullptr; }
    return toTag(handle)->copy().release();
}

size_t nbt_any_tag_hash(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->hash();
}

void nbt_any_tag_write(void* handle, void* stream) {
    if (handle) { toTag(handle)->write(*reinterpret_cast<bedrock_protocol::BinaryStream*>(stream)); }
}

void nbt_any_tag_load(void* handle, void* stream) {
    if (handle) { return toTag(handle)->load(*reinterpret_cast<bedrock_protocol::ReadOnlyBinaryStream*>(stream)); }
}

void nbt_any_tag_destroy(void* handle) {
    if (!handle) { return; }
    switch (nbt_any_tag_get_type(handle)) {
    case Tag_Byte: {
        delete reinterpret_cast<bedrock_protocol::ByteTag*>(handle);
        break;
    }
    case Tag_Short: {
        delete reinterpret_cast<bedrock_protocol::ShortTag*>(handle);
        break;
    }
    case Tag_Int: {
        delete reinterpret_cast<bedrock_protocol::IntTag*>(handle);
        break;
    }
    case Tag_Int64: {
        delete reinterpret_cast<bedrock_protocol::Int64Tag*>(handle);
        break;
    }
    case Tag_Float: {
        delete reinterpret_cast<bedrock_protocol::FloatTag*>(handle);
        break;
    }
    case Tag_Double: {
        delete reinterpret_cast<bedrock_protocol::DoubleTag*>(handle);
        break;
    }
    case Tag_ByteArray: {
        delete reinterpret_cast<bedrock_protocol::ByteArrayTag*>(handle);
        break;
    }
    case Tag_String: {
        delete reinterpret_cast<bedrock_protocol::StringTag*>(handle);
        break;
    }
    case Tag_List: {
        delete reinterpret_cast<bedrock_protocol::ListTag*>(handle);
        break;
    }
    case Tag_Compound: {
        delete reinterpret_cast<bedrock_protocol::CompoundTag*>(handle);
        break;
    }
    case Tag_IntArray: {
        delete reinterpret_cast<bedrock_protocol::IntArrayTag*>(handle);
        break;
    }
    default:
        break;
    }
}

// EndTag
void* nbt_end_tag_create() { return new bedrock_protocol::EndTag(); }

// ByteTag
void* nbt_byte_tag_create(uint8_t value) { return new bedrock_protocol::ByteTag(value); }

void nbt_byte_tag_set_value(void* handle, uint8_t value) {
    if (handle) { toTag(handle)->as<bedrock_protocol::ByteTag>() = value; }
}

uint8_t nbt_byte_tag_get_value(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::ByteTag>();
}

// ShortTag
void* nbt_short_tag_create(short value) { return new bedrock_protocol::ShortTag(value); }

void nbt_short_tag_set_value(void* handle, short value) {
    if (handle) { toTag(handle)->as<bedrock_protocol::ShortTag>() = value; }
}

short nbt_short_tag_get_value(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::ShortTag>();
}

// IntTag
void* nbt_int_tag_create(int value) { return new bedrock_protocol::IntTag(value); }

void nbt_int_tag_set_value(void* handle, int value) {
    if (handle) { toTag(handle)->as<bedrock_protocol::IntTag>() = value; }
}

int nbt_int_tag_get_value(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::IntTag>();
}

// Int64Tag
void* nbt_int64_tag_create(int64_t value) { return new bedrock_protocol::Int64Tag(value); }

void nbt_int64_tag_set_value(void* handle, int64_t value) {
    if (handle) { toTag(handle)->as<bedrock_protocol::Int64Tag>() = value; }
}

int64_t nbt_int64_tag_get_value(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::Int64Tag>();
}

// FloatTag
void* nbt_float_tag_create(float value) { return new bedrock_protocol::FloatTag(value); }

void nbt_float_tag_set_value(void* handle, float value) {
    if (handle) { toTag(handle)->as<bedrock_protocol::FloatTag>() = value; }
}

float nbt_float_tag_get_value(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::FloatTag>();
}

// DoubleTag
void* nbt_double_tag_create(double value) { return new bedrock_protocol::DoubleTag(value); }

void nbt_double_tag_set_value(void* handle, double value) {
    if (handle) { toTag(handle)->as<bedrock_protocol::DoubleTag>() = value; }
}

double nbt_double_tag_get_value(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::DoubleTag>();
}

// ByteArrayTag
void* nbt_byte_array_tag_create(const uint8_t* data, size_t size) {
    if (data) { return new bedrock_protocol::ByteArrayTag(data, size); }
    return new bedrock_protocol::ByteArrayTag();
}

void nbt_byte_array_tag_set_value(void* handle, const uint8_t* data, size_t size) {
    if (handle) { toTag(handle)->as<bedrock_protocol::ByteArrayTag>().reinit(data, size); }
}

nbtio_buffer nbt_byte_array_tag_get_value(void* handle) {
    if (handle) {
        std::vector<uint8_t> value = toTag(handle)->as<bedrock_protocol::ByteArrayTag>();
        uint8_t*             data  = new uint8_t[value.size()];
        std::memcpy(data, value.data(), value.size());
        nbtio_buffer result;
        result.data = data;
        result.size = value.size();
        return result;
    }
    return nbtio_buffer();
}

// StringTag
void* nbt_string_tag_create(const char* data, size_t size) {
    if (data) { return new bedrock_protocol::StringTag(data, size); }
    return new bedrock_protocol::StringTag();
}

void nbt_string_tag_set_value(void* handle, const char* data, size_t size) {
    if (handle && data) { toTag(handle)->as<bedrock_protocol::StringTag>() = std::string(data, size); }
}

nbtio_buffer nbt_string_tag_get(void* handle) {
    if (handle) {
        std::string value = toTag(handle)->as<bedrock_protocol::StringTag>();
        uint8_t*    data  = new uint8_t[value.size()];
        std::memcpy(data, value.data(), value.size());
        nbtio_buffer result;
        result.data = data;
        result.size = value.size();
        return result;
    }
    return nbtio_buffer();
}

// ListTag
void* nbt_list_tag_create() { return new bedrock_protocol::ListTag(); }

size_t nbt_list_tag_size(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::ListTag>().size();
}

void nbt_list_tag_add_tag(void* handle, void* tag) {
    if (handle && tag) { toTag(handle)->as<bedrock_protocol::ListTag>().push_back(toTag(tag)->copy()); }
}

void* nbt_list_tag_get_tag(void* handle, size_t index) {
    if (handle) {
        auto& tag = toTag(handle)->as<bedrock_protocol::ListTag>();
        if (index < tag.size()) { return tag[index].copy().release(); }
    }
    return nullptr;
}

bool nbt_list_tag_remove_tag(void* handle, size_t index) {
    if (!handle) { return false; }
    return toTag(handle)->as<bedrock_protocol::ListTag>().remove(index);
}

void nbt_list_tag_clear(void* handle) {
    if (handle) { return toTag(handle)->as<bedrock_protocol::ListTag>().clear(); }
}

bool nbt_list_tag_set_tag(void* handle, size_t index, void* tag) {
    if (handle) {
        auto& listTag = toTag(handle)->as<bedrock_protocol::ListTag>();
        if (index < listTag.size()) { return listTag.set(index, *toTag(tag)); }
    }
    return false;
}

// CompoundTag
void* nbt_compound_tag_create() { return new bedrock_protocol::CompoundTag(); }

size_t nbt_compound_tag_size(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::CompoundTag>().size();
}

void nbt_compound_tag_set_tag(void* handle, const char* key_data, size_t key_size, void* tag) {
    if (!handle) { return; }
    std::string key(key_data, key_size);
    toTag(handle)->as<bedrock_protocol::CompoundTag>().put(key, toTag(tag)->copy());
}

void* nbt_compound_tag_get_tag(void* handle, const char* key_data, size_t key_size) {
    if (handle) {
        std::string key(key_data, key_size);
        return toTag(handle)->as<bedrock_protocol::CompoundTag>().get(key)->copy().release();
    }
    return nullptr;
}

bool nbt_compound_tag_remove_tag(void* handle, const char* key_data, size_t key_size) {
    if (!handle) { return false; }
    std::string key(key_data, key_size);
    return toTag(handle)->as<bedrock_protocol::CompoundTag>().remove(key);
}

void nbt_compound_tag_clear(void* handle) {
    if (handle) { toTag(handle)->as<bedrock_protocol::CompoundTag>().clear(); }
}

nbtio_buffer nbt_compound_to_binary_nbt(void* handle, bool little_endian) {
    if (handle) {
        std::string value = toTag(handle)->as<bedrock_protocol::CompoundTag>().toBinaryNbt(little_endian);
        uint8_t*    data  = new uint8_t[value.size()];
        std::memcpy(data, value.data(), value.size());
        nbtio_buffer result;
        result.data = data;
        result.size = value.size();
        return result;
    }
    return nbtio_buffer();
}

nbtio_buffer nbt_compound_to_network_nbt(void* handle) {
    if (handle) {
        std::string value = toTag(handle)->as<bedrock_protocol::CompoundTag>().toNetworkNbt();
        uint8_t*    data  = new uint8_t[value.size()];
        std::memcpy(data, value.data(), value.size());
        nbtio_buffer result;
        result.data = data;
        result.size = value.size();
        return result;
    }
    return nbtio_buffer();
}

nbtio_buffer nbt_compound_to_snbt(void* handle, Snbt_Format format, uint8_t indent) {
    if (handle) {
        std::string value = toTag(handle)->as<bedrock_protocol::CompoundTag>().toSnbt(
            static_cast<bedrock_protocol::SnbtFormat>(format),
            indent
        );
        uint8_t* data = new uint8_t[value.size()];
        std::memcpy(data, value.data(), value.size());
        nbtio_buffer result;
        result.data = data;
        result.size = value.size();
        return result;
    }
    return nbtio_buffer();
}

void* nbt_compound_from_binary_nbt(const uint8_t* data, size_t size, bool little_endian) {
    std::string_view content(reinterpret_cast<const char*>(data), size);
    auto             result = bedrock_protocol::CompoundTag::fromBinaryNbt(content, little_endian);
    return new bedrock_protocol::CompoundTag(result);
}

void* nbt_compound_from_network_nbt(const uint8_t* data, size_t size) {
    std::string_view content(reinterpret_cast<const char*>(data), size);
    auto             result = bedrock_protocol::CompoundTag::fromNetworkNbt(content);
    return new bedrock_protocol::CompoundTag(result);
}

void* nbt_compound_from_snbt(const uint8_t* data, size_t size) {
    std::string_view content(reinterpret_cast<const char*>(data), size);
    if (auto result = bedrock_protocol::CompoundTag::fromSnbt(content)) {
        return new bedrock_protocol::CompoundTag(*result);
    }
    return nullptr;
}

// IntArrayTag
void* nbt_int_array_tag_create() { return new bedrock_protocol::IntArrayTag(); }

size_t nbt_int_array_tag_size(void* handle) {
    if (!handle) { return 0; }
    return toTag(handle)->as<bedrock_protocol::IntArrayTag>().size();
}

void nbt_int_array_tag_add_value(void* handle, int value) {
    toTag(handle)->as<bedrock_protocol::IntArrayTag>().push_back(value);
}

int nbt_int_array_tag_get_value(void* handle, size_t index) {
    if (handle) {
        auto& tag = toTag(handle)->as<bedrock_protocol::IntArrayTag>();
        if (index < tag.size()) { return tag[index]; }
    }
    return 0;
}

bool nbt_int_array_tag_remove_value(void* handle, size_t index) {
    if (!handle) { return false; }
    return toTag(handle)->as<bedrock_protocol::IntArrayTag>().remove(index);
}

void nbt_int_array_tag_clear(void* handle) {
    if (handle) { return toTag(handle)->as<bedrock_protocol::ListTag>().clear(); }
}

bool nbt_int_array_tag_set_tag(void* handle, size_t index, void* tag) {
    if (handle) {
        auto& intArrayTag = toTag(handle)->as<bedrock_protocol::ListTag>();
        if (index < intArrayTag.size()) { return intArrayTag.set(index, *toTag(tag)); }
    }
    return false;
}
}