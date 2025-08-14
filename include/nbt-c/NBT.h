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

// Any Tag
TagType nbt_any_tag_get_type(void* handle);
bool    nbt_any_tag_equals(void* handle, void* other);
void    nbt_any_tag_copy(void* handle);
void    nbt_any_tag_hash(void* handle);
void    nbt_any_tag_write(void* handle, void* stream);
void    nbt_any_tag_load(void* handle, void* stream);
void    nbt_any_tag_destroy(void* handle);

// EndTag
void* nbt_end_tag_create();

// ByteTag
void*   nbt_byte_tag_create(uint8_t value);
void    nbt_byte_tag_set_value(void* handle, uint8_t value);
uint8_t nbt_byte_tag_get_value(void* handle);

// ShortTag
void* nbt_short_tag_create(short value);
void  nbt_short_tag_set_value(void* handle, short value);
short nbt_short_tag_get_value(void* handle);

// IntTag
void* nbt_int_tag_create(int value);
void  nbt_int_tag_set_value(void* handle, int value);
int   nbt_int_tag_get_value(void* handle);

// Int64Tag
void*   nbt_int64_tag_create(int64_t value);
void    nbt_int64_tag_set_value(void* handle, int64_t value);
int64_t nbt_int64_tag_get_value(void* handle);

// FloatTag
void* nbt_float_tag_create(float value);
void  nbt_float_tag_set_value(void* handle, float value);
float nbt_float_tag_get_value(void* handle);

// DoubleTag
void*  nbt_double_tag_create(double value);
void   nbt_double_tag_set_value(void* handle, double value);
double nbt_double_tag_get_value(void* handle);

// ByteArrayTag
void* nbt_byte_array_tag_create(const uint8_t* data, size_t size);

// StringTag
void*  nbt_string_tag_create(const char* data, size_t size);
void   nbt_string_tag_set_value(void* handle, const char* data, size_t size);
char*  nbt_string_tag_get_data(void* handle);
size_t nbt_string_tag_get_size(void* handle);

// ListTag
void*  nbt_list_tag_create();
size_t nbt_list_tag_size(void* handle);
void   nbt_list_tag_push_tag(void* handle, void* tag);
void   nbt_list_tag_remove_tag(void* handle, size_t index);
void*  nbt_list_tag_get_tag(void* handle, size_t index);

// CompoundTag
void* nbt_compound_tag_create();

// IntArrayTag


void* nbt_int_tag_create(const int* data, size_t size);


#ifdef __cplusplus
}
#endif