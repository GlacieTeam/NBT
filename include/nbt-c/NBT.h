// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <stddef.h>
#include <stdint.h>

#ifdef NBT_EXPORT
#ifdef _WIN32
#define NBT_API __declspec(dllexport)
#else
#define NBT_API __attribute__((visibility("default"), used))
#endif
#else
#define NBT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct nbtio_buffer {
    uint8_t* data;
    size_t   size;
};
NBT_API void nbtio_buffer_destroy(nbtio_buffer* buffer);

enum TagType {
    Tag_End       = 0,
    Tag_Byte      = 1,
    Tag_Short     = 2,
    Tag_Int       = 3,
    Tag_Int64     = 4,
    Tag_Float     = 5,
    Tag_Double    = 6,
    Tag_ByteArray = 7,
    Tag_String    = 8,
    Tag_List      = 9,
    Tag_Compound  = 10,
    Tag_IntArray  = 11,
};

enum Snbt_Format {
    Minimize        = 0,
    PrettyFilePrint = 1,
    ArrayLineFeed   = 2,
    AlwaysLineFeed  = 3,
    ForceAscii      = 4,
    ForceQuote      = 8,
    CommentMarks    = 16,
    Jsonify         = 24,
};

// Any Tag
NBT_API TagType nbt_any_tag_get_type(void* handle);
NBT_API bool    nbt_any_tag_equals(void* handle, void* other);
NBT_API void*   nbt_any_tag_copy(void* handle);
NBT_API size_t  nbt_any_tag_hash(void* handle);
NBT_API void    nbt_any_tag_write(void* handle, void* stream);
NBT_API void    nbt_any_tag_load(void* handle, void* stream);
NBT_API void    nbt_any_tag_destroy(void* handle);

NBT_API nbtio_buffer nbt_any_tag_to_snbt(void* handle, Snbt_Format format, uint8_t indent);
NBT_API nbtio_buffer nbt_any_tag_to_json(void* handle, uint8_t indent);

// EndTag
NBT_API void* nbt_end_tag_create();

// ByteTag
NBT_API void*   nbt_byte_tag_create(uint8_t value);
NBT_API void    nbt_byte_tag_set_value(void* handle, uint8_t value);
NBT_API uint8_t nbt_byte_tag_get_value(void* handle);

// ShortTag
NBT_API void* nbt_short_tag_create(short value);
NBT_API void  nbt_short_tag_set_value(void* handle, short value);
NBT_API short nbt_short_tag_get_value(void* handle);

// IntTag
NBT_API void* nbt_int_tag_create(int value);
NBT_API void  nbt_int_tag_set_value(void* handle, int value);
NBT_API int   nbt_int_tag_get_value(void* handle);

// Int64Tag
NBT_API void*   nbt_int64_tag_create(int64_t value);
NBT_API void    nbt_int64_tag_set_value(void* handle, int64_t value);
NBT_API int64_t nbt_int64_tag_get_value(void* handle);

// FloatTag
NBT_API void* nbt_float_tag_create(float value);
NBT_API void  nbt_float_tag_set_value(void* handle, float value);
NBT_API float nbt_float_tag_get_value(void* handle);

// DoubleTag
NBT_API void*  nbt_double_tag_create(double value);
NBT_API void   nbt_double_tag_set_value(void* handle, double value);
NBT_API double nbt_double_tag_get_value(void* handle);

// ByteArrayTag
NBT_API void*        nbt_byte_array_tag_create(const uint8_t* data, size_t size);
NBT_API void         nbt_byte_array_tag_set_value(void* handle, const uint8_t* data, size_t size);
NBT_API nbtio_buffer nbt_byte_array_tag_get_value(void* handle);

// StringTag
NBT_API void*        nbt_string_tag_create(const char* data, size_t size);
NBT_API void         nbt_string_tag_set_value(void* handle, const char* data, size_t size);
NBT_API nbtio_buffer nbt_string_tag_get_value(void* handle);

// ListTag
NBT_API void*  nbt_list_tag_create();
NBT_API size_t nbt_list_tag_size(void* handle);
NBT_API void   nbt_list_tag_add_tag(void* handle, void* tag);
NBT_API void*  nbt_list_tag_get_tag(void* handle, size_t index);
NBT_API bool   nbt_list_tag_remove_tag(void* handle, size_t index);
NBT_API void   nbt_list_tag_clear(void* handle);
NBT_API bool   nbt_list_tag_set_tag(void* handle, size_t index, void* tag);

// CompoundTag
NBT_API void*  nbt_compound_tag_create();
NBT_API size_t nbt_compound_tag_size(void* handle);
NBT_API void   nbt_compound_tag_set_tag(void* handle, const char* key_data, size_t key_size, void* tag);
NBT_API void*  nbt_compound_tag_get_tag(void* handle, const char* key_data, size_t key_size);
NBT_API bool   nbt_compound_tag_has_tag(void* handle, const char* key_data, size_t key_size);
NBT_API bool   nbt_compound_tag_remove_tag(void* handle, const char* key_data, size_t key_size);
NBT_API void   nbt_compound_tag_clear(void* handle);

NBT_API nbtio_buffer nbt_compound_tag_to_binary_nbt(void* handle, bool little_endian);
NBT_API nbtio_buffer nbt_compound_tag_to_network_nbt(void* handle);

NBT_API void* nbt_compound_tag_from_binary_nbt(const uint8_t* data, size_t size, bool little_endian);
NBT_API void* nbt_compound_tag_from_network_nbt(const uint8_t* data, size_t size);
NBT_API void* nbt_compound_tag_from_snbt(const uint8_t* data, size_t size);

// IntArrayTag
NBT_API void*  nbt_int_array_tag_create();
NBT_API size_t nbt_int_array_tag_size(void* handle);
NBT_API void   nbt_int_array_tag_add_value(void* handle, int value);
NBT_API int    nbt_int_array_tag_get_value(void* handle, size_t index);
NBT_API bool   nbt_int_array_tag_remove_value(void* handle, size_t index);
NBT_API void   nbt_int_array_tag_clear(void* handle);
NBT_API bool   nbt_int_array_tag_set_value(void* handle, size_t index, void* tag);

#ifdef __cplusplus
}
#endif