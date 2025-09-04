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
    void* mapped = mmap(nullptr, static_cast<size_t>(sb.st_size), PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        close(fd);
        return content;
    }
    content.assign(static_cast<char*>(mapped), static_cast<size_t>(sb.st_size));
    munmap(mapped, static_cast<size_t>(sb.st_size));
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
            && ((b0 == 0x1F && b1 == 0x8B) || (b0 == 0x78 && (b1 == 0x01 || b1 == 0x9C || b1 == 0xDA)))) {
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
        content.resize(static_cast<size_t>(size));
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

extern bool validateCompoundTag(BytesDataInput& stream, size_t streamSize);

bool validateListTag(BytesDataInput& stream, size_t streamSize) {
    if (stream.getPosition() + sizeof(std::byte) > streamSize) { return false; }
    auto type = static_cast<Tag::Type>(stream.getByte());
    if (stream.getPosition() + sizeof(int) > streamSize) { return false; }
    auto size = static_cast<size_t>(stream.getInt());
    switch (type) {
    case Tag::Type::Byte: {
        if (stream.getPosition() + (sizeof(std::byte) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(std::byte) * size);
        break;
    }
    case Tag::Type::Short: {
        if (stream.getPosition() + (sizeof(short) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(short) * size);
        break;
    }
    case Tag::Type::Int: {
        if (stream.getPosition() + (sizeof(int) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(int) * size);
        break;
    }
    case Tag::Type::Int64: {
        if (stream.getPosition() + (sizeof(int64_t) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(int64_t) * size);
        break;
    }
    case Tag::Type::Float: {
        if (stream.getPosition() + (sizeof(float) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(float) * size);
        break;
    }
    case Tag::Type::Double: {
        if (stream.getPosition() + (sizeof(double) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(double) * size);
        break;
    }
    case Tag::Type::ByteArray: {
        for (size_t i = 0; i < size; i++) {
            if (stream.getPosition() + sizeof(int) > streamSize) { return false; }
            auto len = static_cast<size_t>(stream.getInt());
            if (stream.getPosition() + (sizeof(std::byte) * len) > streamSize) { return false; }
            stream.ignoreBytes((sizeof(std::byte) * len));
        }
        break;
    }
    case Tag::Type::String: {
        for (size_t i = 0; i < size; i++) {
            if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
            auto strSize = static_cast<size_t>(stream.getShort());
            if (stream.getPosition() + strSize > streamSize) { return false; }
            stream.ignoreBytes(strSize);
        }
        break;
    }
    case Tag::Type::List: {
        for (size_t i = 0; i < size; i++) {
            if (!validateListTag(stream, streamSize)) { return false; }
        }
        break;
    }
    case Tag::Type::Compound: {
        for (size_t i = 0; i < size; i++) {
            if (!validateCompoundTag(stream, streamSize)) { return false; }
        }
        break;
    }
    case Tag::Type::IntArray: {
        for (size_t i = 0; i < size; i++) {
            if (stream.getPosition() + sizeof(int) > streamSize) { return false; }
            auto len = static_cast<size_t>(stream.getInt());
            if (stream.getPosition() + (sizeof(int) * len) > streamSize) { return false; }
            stream.ignoreBytes((sizeof(int) * len));
        }
        break;
    }
    case Tag::Type::LongArray: {
        for (size_t i = 0; i < size; i++) {
            if (stream.getPosition() + sizeof(int) > streamSize) { return false; }
            auto len = static_cast<size_t>(stream.getInt());
            if (stream.getPosition() + (sizeof(int64_t) * len) > streamSize) { return false; }
            stream.ignoreBytes((sizeof(int64_t) * len));
        }
        break;
    }
    default:
        return false;
    }
    return true;
}

bool validateCompoundTag(BytesDataInput& stream, size_t streamSize) {
    Tag::Type type = Tag::Type::End;
    while (true) {
        if (stream.getPosition() + sizeof(std::byte) > streamSize) { return false; }
        type = static_cast<Tag::Type>(stream.getByte());
        if (type == Tag::Type::End) { return true; }
        if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
        auto strLen = static_cast<size_t>(stream.getShort());
        if (stream.getPosition() + strLen > streamSize) { return false; }
        stream.ignoreBytes(strLen);
        switch (type) {
        case Tag::Type::Byte: {
            if (stream.getPosition() + sizeof(std::byte) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(std::byte));
            break;
        }
        case Tag::Type::Short: {
            if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(short));
            break;
        }
        case Tag::Type::Int: {
            if (stream.getPosition() + sizeof(int) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(int));
            break;
        }
        case Tag::Type::Int64: {
            if (stream.getPosition() + sizeof(int64_t) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(int64_t));
            break;
        }
        case Tag::Type::Float: {
            if (stream.getPosition() + sizeof(float) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(float));
            break;
        }
        case Tag::Type::Double: {
            if (stream.getPosition() + sizeof(double) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(double));
            break;
        }
        case Tag::Type::ByteArray: {
            if (stream.getPosition() + sizeof(int) > streamSize) { return false; }
            auto size = static_cast<size_t>(stream.getInt());
            if (stream.getPosition() + (sizeof(std::byte) * size) > streamSize) { return false; }
            stream.ignoreBytes((sizeof(std::byte) * size));
            break;
        }
        case Tag::Type::String: {
            if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
            auto strSize = static_cast<size_t>(stream.getShort());
            if (stream.getPosition() + strSize > streamSize) { return false; }
            stream.ignoreBytes(strSize);
            break;
        }
        case Tag::Type::List: {
            if (!validateListTag(stream, streamSize)) { return false; }
            break;
        }
        case Tag::Type::Compound: {
            if (!validateCompoundTag(stream, streamSize)) { return false; }
            break;
        }
        case Tag::Type::IntArray: {
            if (stream.getPosition() + sizeof(int) > streamSize) { return false; }
            auto size = static_cast<size_t>(stream.getInt());
            if (stream.getPosition() + (sizeof(int) * size) > streamSize) { return false; }
            stream.ignoreBytes((sizeof(int) * size));
            break;
        }
        case Tag::Type::LongArray: {
            if (stream.getPosition() + sizeof(int) > streamSize) { return false; }
            auto size = static_cast<size_t>(stream.getInt());
            if (stream.getPosition() + (sizeof(int64_t) * size) > streamSize) { return false; }
            stream.ignoreBytes((sizeof(int64_t) * size));
            break;
        }
        default:
            return false;
        }
    }
    return true;
}

extern bool validateCompoundTag(bstream::ReadOnlyBinaryStream& stream, size_t streamSize);

bool validateListTag(bstream::ReadOnlyBinaryStream& stream, size_t streamSize) {
    if (stream.getPosition() + sizeof(std::byte) > streamSize) { return false; }
    auto type = static_cast<Tag::Type>(stream.getByte());
    auto size = static_cast<size_t>(stream.getVarInt());
    if (stream.isOverflowed()) { return false; }
    switch (type) {
    case Tag::Type::Byte: {
        if (stream.getPosition() + (sizeof(std::byte) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(std::byte) * size);
        break;
    }
    case Tag::Type::Short: {
        if (stream.getPosition() + (sizeof(short) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(short) * size);
        break;
    }
    case Tag::Type::Int: {
        (void)stream.getVarInt();
        if (stream.isOverflowed()) { return false; }
        break;
    }
    case Tag::Type::Int64: {
        (void)stream.getVarInt64();
        if (stream.isOverflowed()) { return false; }
        break;
    }
    case Tag::Type::Float: {
        if (stream.getPosition() + (sizeof(float) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(float) * size);
        break;
    }
    case Tag::Type::Double: {
        if (stream.getPosition() + (sizeof(double) * size) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(double) * size);
        break;
    }
    case Tag::Type::ByteArray: {
        for (size_t i = 0; i < size; i++) {
            auto len = static_cast<size_t>(stream.getVarInt());
            if (stream.isOverflowed() || stream.getPosition() + (sizeof(std::byte) * len) > streamSize) {
                return false;
            }
            stream.ignoreBytes((sizeof(std::byte) * len));
        }
        break;
    }
    case Tag::Type::String: {
        for (size_t i = 0; i < size; i++) {
            auto strSize = static_cast<size_t>(stream.getUnsignedVarInt());
            if (stream.isOverflowed() || stream.getPosition() + strSize > streamSize) { return false; }
            stream.ignoreBytes(strSize);
        }
        break;
    }
    case Tag::Type::List: {
        for (size_t i = 0; i < size; i++) {
            if (!validateListTag(stream, streamSize)) { return false; }
        }
        break;
    }
    case Tag::Type::Compound: {
        for (size_t i = 0; i < size; i++) {
            if (!validateCompoundTag(stream, streamSize)) { return false; }
        }
        break;
    }
    case Tag::Type::IntArray: {
        for (size_t i = 0; i < size; i++) {
            auto len = static_cast<size_t>(stream.getVarInt());
            if (stream.isOverflowed() || stream.getPosition() + (sizeof(int) * len) > streamSize) { return false; }
            for (size_t j = 0; j < len; j++) {
                (void)stream.getVarInt();
                if (stream.isOverflowed()) { return false; }
            }
        }
        break;
    }
    case Tag::Type::LongArray: {
        for (size_t i = 0; i < size; i++) {
            auto len = static_cast<size_t>(stream.getVarInt());
            if (stream.isOverflowed() || stream.getPosition() + (sizeof(int64_t) * len) > streamSize) { return false; }
            for (size_t j = 0; j < len; j++) {
                (void)stream.getVarInt64();
                if (stream.isOverflowed()) { return false; }
            }
        }
        break;
    }
    default:
        return false;
    }
    return true;
}

bool validateCompoundTag(bstream::ReadOnlyBinaryStream& stream, size_t streamSize) {
    Tag::Type type = Tag::Type::End;
    while (true) {
        if (stream.getPosition() + sizeof(std::byte) > streamSize) { return false; }
        type = static_cast<Tag::Type>(stream.getByte());
        if (type == Tag::Type::End) { return true; }
        auto strLen = stream.getUnsignedVarInt();
        if (stream.isOverflowed() || stream.getPosition() + strLen > streamSize) { return false; }
        stream.ignoreBytes(strLen);
        switch (type) {
        case Tag::Type::Byte: {
            if (stream.getPosition() + sizeof(std::byte) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(std::byte));
            break;
        }
        case Tag::Type::Short: {
            if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(short));
            break;
        }
        case Tag::Type::Int: {
            (void)stream.getUnsignedVarInt();
            if (stream.isOverflowed()) { return false; }
            break;
        }
        case Tag::Type::Int64: {
            (void)stream.getUnsignedVarInt64();
            if (stream.isOverflowed()) { return false; }
            break;
        }
        case Tag::Type::Float: {
            if (stream.getPosition() + sizeof(float) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(float));
            break;
        }
        case Tag::Type::Double: {
            if (stream.getPosition() + sizeof(double) > streamSize) { return false; }
            stream.ignoreBytes(sizeof(double));
            break;
        }
        case Tag::Type::ByteArray: {
            auto size = static_cast<size_t>(stream.getVarInt());
            if (stream.isOverflowed() || stream.getPosition() + (sizeof(std::byte) * size) > streamSize) {
                return false;
            }
            stream.ignoreBytes((sizeof(std::byte) * size));
            break;
        }
        case Tag::Type::String: {
            auto strSize = static_cast<size_t>(stream.getUnsignedVarInt());
            if (stream.isOverflowed() || stream.getPosition() + strSize > streamSize) { return false; }
            stream.ignoreBytes(strSize);
            break;
        }
        case Tag::Type::List: {
            if (!validateListTag(stream, streamSize)) { return false; }
            break;
        }
        case Tag::Type::Compound: {
            if (!validateCompoundTag(stream, streamSize)) { return false; }
            break;
        }
        case Tag::Type::IntArray: {
            auto size = static_cast<size_t>(stream.getVarInt());
            if (stream.isOverflowed() || stream.getPosition() + (sizeof(int) * size) > streamSize) { return false; }
            for (size_t i = 0; i < size; i++) {
                (void)stream.getVarInt();
                if (stream.isOverflowed()) { return false; }
            }
            break;
        }
        case Tag::Type::LongArray: {
            auto size = static_cast<size_t>(stream.getVarInt());
            if (stream.isOverflowed() || stream.getPosition() + (sizeof(int64_t) * size) > streamSize) { return false; }
            for (size_t i = 0; i < size; i++) {
                (void)stream.getVarInt64();
                if (stream.isOverflowed()) { return false; }
            }
            break;
        }
        default:
            return false;
        }
    }
    return true;
}

bool validateContent(std::string_view binary, NbtFileFormat format) {
    switch (format) {
    case NbtFileFormat::LittleEndianBinary: {
        BytesDataInput stream(binary, false, true);
        auto           streamSize = stream.size();
        if (static_cast<Tag::Type>(stream.getByte()) != Tag::Type::Compound) { return false; }
        if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
        auto strSize = static_cast<size_t>(stream.getShort());
        if (stream.getPosition() + strSize > streamSize) { return false; }
        stream.ignoreBytes(strSize);
        if (validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
        break;
    }
    case NbtFileFormat::LittleEndianBinaryWithHeader: {
        BytesDataInput stream(binary, false, true);
        auto           streamSize = stream.size();
        if (stream.getPosition() + (2 * sizeof(int)) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(int));
        auto nbtSize = static_cast<size_t>(stream.getInt());
        if (stream.getPosition() + nbtSize > streamSize) { return false; }
        if (static_cast<Tag::Type>(stream.getByte()) != Tag::Type::Compound) { return false; }
        if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
        auto strSize = static_cast<size_t>(stream.getShort());
        if (stream.getPosition() + strSize > streamSize) { return false; }
        stream.ignoreBytes(strSize);
        if (validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
        break;
    }
    case NbtFileFormat::BigEndianBinary: {
        BytesDataInput stream(binary, false, false);
        auto           streamSize = stream.size();
        if (static_cast<Tag::Type>(stream.getByte()) != Tag::Type::Compound) { return false; }
        if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
        auto strSize = static_cast<size_t>(stream.getShort());
        if (stream.getPosition() + strSize > streamSize) { return false; }
        stream.ignoreBytes(strSize);
        if (validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
        break;
    }
    case NbtFileFormat::BigEndianBinaryWithHeader: {
        BytesDataInput stream(binary, false, false);
        auto           streamSize = stream.size();
        if (stream.getPosition() + (2 * sizeof(int)) > streamSize) { return false; }
        stream.ignoreBytes(sizeof(int));
        auto nbtSize = static_cast<size_t>(stream.getInt());
        if (stream.getPosition() + nbtSize > streamSize) { return false; }
        if (static_cast<Tag::Type>(stream.getByte()) != Tag::Type::Compound) { return false; }
        if (stream.getPosition() + sizeof(short) > streamSize) { return false; }
        auto strSize = static_cast<size_t>(stream.getShort());
        if (stream.getPosition() + strSize > streamSize) { return false; }
        stream.ignoreBytes(strSize);
        if (validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
        break;
    }
    case NbtFileFormat::BedrockNetwork: {
        bstream::ReadOnlyBinaryStream stream(binary, false, false);
        auto                          streamSize = stream.size();
        if (static_cast<Tag::Type>(stream.getByte()) != Tag::Type::Compound) { return false; }
        auto strSize = static_cast<size_t>(stream.getUnsignedVarInt());
        if (stream.isOverflowed() || stream.getPosition() + strSize > streamSize) { return false; }
        stream.ignoreBytes(strSize);
        if (validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
        break;
    }
    default:
        break;
    }
    return false;
}

bool validate(std::filesystem::path const& path, NbtFileFormat format) {
    if (!std::filesystem::exists(path)) { return false; }
    auto        content = readFileMMap(path);
    const auto& b0      = static_cast<uint8_t const&>(content[0]);
    const auto& b1      = static_cast<uint8_t const&>(content[1]);
    if (content.size() > 2
        && ((b0 == 0x1F && b1 == 0x8B) || (b0 == 0x78 && (b1 == 0x01 || b1 == 0x9C || b1 == 0xDA)))) {
        std::istringstream stream(content);
        zstr::istream      decompressor(stream);
        content.assign(std::istreambuf_iterator<char>(decompressor), std::istreambuf_iterator<char>());
    }
    return validateContent(content, format);
}

} // namespace nbt