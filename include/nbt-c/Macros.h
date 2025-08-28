#pragma once

#ifdef _NBT_EXPORT
#ifdef _WIN32
#ifdef _NBT_DLL
#define NBT_API __declspec(dllimport)
#else
#define NBT_API __declspec(dllexport)
#endif
#else
#define NBT_API __attribute__((visibility("default"), used))
#endif
#else
#define NBT_API
#endif