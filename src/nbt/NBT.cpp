// Copyright © 2025 GlacieTeam.All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/NBT.hpp"
#include <sstream>
#include <zstr/zstr.hpp>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace nbt {

std::string readFileMMap(std::filesystem::path const& path) {
    std::string content;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hFile = CreateFileA(
        path.string().c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) { return content; }
    DWORD size = GetFileSize(hFile, NULL);
    if (size == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return content;
    }
    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!hMapping) {
        CloseHandle(hFile);
        return content;
    }
    LPVOID mapped = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (!mapped) {
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return content;
    }
    content.assign(static_cast<char*>(mapped), size);
    UnmapViewOfFile(mapped);
    CloseHandle(hMapping);
    CloseHandle(hFile);
#else
    int fd = open(path.string().c_str(), O_RDONLY);
    if (fd == -1) { return content; }
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        close(fd);
        return content;
    }
    void* mapped = mmap(nullptr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        close(fd);
        return content;
    }
    content.assign(static_cast<char*>(mapped), sb.st_size);
    munmap(mapped, sb.st_size);
    close(fd);
#endif
    return content;
}

std::optional<CompoundTag> parseFromFile(std::filesystem::path const& path, NbtFileFormat format) {
    if (std::filesystem::exists(path)) {
        auto        content = readFileMMap(path);
        const auto& b0      = static_cast<uint8_t const&>(content[0]);
        const auto& b1      = static_cast<uint8_t const&>(content[1]);
        if (content.size() > 2
                && ((b0 == 0x1F && b1 == 0x8B) // gzip header
                    || (b0 == 0x78 && (b1 == 0x01 || b1 == 0x9C || b1 == 0xDA))// zlib header
                    )) {
            std::istringstream stream(content);
            zstr::istream      decompressor(stream);
            content.assign(std::istreambuf_iterator<char>(decompressor), std::istreambuf_iterator<char>());
        }
        switch (format) {
        case NbtFileFormat::LittleEndianBinary: {
            return CompoundTag::fromBinaryNbt(content, true);
        }
        case NbtFileFormat::LittleEndianBinaryWithHeader: {
            return CompoundTag::fromBinaryNbtWithHeader(content, true);
        }
        case NbtFileFormat::BigEndianBinary: {
            return CompoundTag::fromBinaryNbt(content, false);
        }
        case NbtFileFormat::BigEndianBinaryWithHeader: {
            return CompoundTag::fromBinaryNbtWithHeader(content, false);
        }
        case NbtFileFormat::BedrockNetwork: {
            return CompoundTag::fromNetworkNbt(content);
        }
        default:
            return std::nullopt;
        }
    }
    return std::nullopt;
}

bool saveToFile(
    CompoundTag const&           nbt,
    std::filesystem::path const& path,
    NbtFileFormat                format,
    CompressionType              compressionType,
    CompressionLevel             compressionLevel
) {
    std::string content;
    switch (format) {
    case NbtFileFormat::LittleEndianBinary: {
        content = nbt.toBinaryNbt(true);
        break;
    }
    case NbtFileFormat::LittleEndianBinaryWithHeader: {
        content = nbt.toBinaryNbtWithHeader(true);
        break;
    }
    case NbtFileFormat::BigEndianBinary: {
        content = nbt.toBinaryNbt(false);
        break;
    }
    case NbtFileFormat::BigEndianBinaryWithHeader: {
        content = nbt.toBinaryNbtWithHeader(false);
        break;
    }
    case NbtFileFormat::BedrockNetwork: {
        content = nbt.toNetworkNbt();
        break;
    }
    default:
        return false;
    }
    if (!std::filesystem::exists(path.parent_path())) { std::filesystem::create_directories(path.parent_path()); }
    switch (compressionType) {
    case CompressionType::Zlib: {
        zstr::ofstream fWrite(path, std::ios::out | std::ios::binary, static_cast<int>(compressionLevel), 15);
        if (fWrite.is_open()) {
            fWrite.write(content.data(), static_cast<std::streamsize>(content.size()));
            fWrite.close();
            return true;
        }
        return false;
    }
    case CompressionType::Gzip: {
        zstr::ofstream fWrite(path, std::ios::out | std::ios::binary, static_cast<int>(compressionLevel), 31);
        if (fWrite.is_open()) {
            fWrite.write(content.data(), static_cast<std::streamsize>(content.size()));
            fWrite.close();
            return true;
        }
        return false;
    }
    default:
        std::ofstream fWrite(path, std::ios::out | std::ios::binary);
        if (fWrite.is_open()) {
            fWrite.write(content.data(), static_cast<std::streamsize>(content.size()));
            fWrite.close();
            return true;
        }
        return false;
    }
    return false;
}

std::optional<CompoundTag> parseSnbtFromFile(std::filesystem::path const& path) {
    std::ifstream fRead(path, std::ios::ate);
    if (fRead.is_open()) {
        std::string content;
        auto        size = fRead.tellg();
        fRead.seekg(0);
        content.resize(size);
        fRead.read(content.data(), size);
        return CompoundTag::fromSnbt(content);
    }
    return std::nullopt;
}

bool saveSnbtToFile(CompoundTag const& nbt, std::filesystem::path const& path, SnbtFormat format, uint8_t indent) {
    std::ofstream fWrite;
    if (!std::filesystem::exists(path.parent_path())) { std::filesystem::create_directories(path.parent_path()); }
    fWrite.open(path, std::ios_base::out);
    if (!fWrite.is_open()) { return false; }
    fWrite << nbt.toSnbt(format, indent);
    fWrite.close();
    return true;
}

} // namespace nbt