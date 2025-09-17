// Copyright © 2025 GlacieTeam.All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/NBT.hpp"
#include "detail/Base64.hpp"
#include "nbt/Literals.hpp"
#include "nbt/detail/Validate.hpp"
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

namespace nbt::io {

constexpr std::byte operator""_byte(unsigned long long value) noexcept { return static_cast<std::byte>(value); }

void readFileMMap(std::filesystem::path const& path, std::string& content) {
#ifdef _WIN32
    HANDLE hFile = CreateFileA(
        path.string().c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) { return; }
    DWORD size = GetFileSize(hFile, NULL);
    if (size == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return;
    }
    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (!hMapping) {
        CloseHandle(hFile);
        return;
    }
    LPVOID mapped = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (!mapped) {
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }
    content.assign(static_cast<char*>(mapped), size);
    UnmapViewOfFile(mapped);
    CloseHandle(hMapping);
    CloseHandle(hFile);
#else
    int fd = open(path.string().c_str(), O_RDONLY);
    if (fd == -1) { return; }
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        close(fd);
        return;
    }
    void* mapped = mmap(nullptr, static_cast<size_t>(sb.st_size), PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        close(fd);
        return;
    }
    content.assign(static_cast<char*>(mapped), static_cast<size_t>(sb.st_size));
    munmap(mapped, static_cast<size_t>(sb.st_size));
    close(fd);
#endif
}

std::optional<NbtFileFormat> checkNbtContentFormat(std::string_view content) {
    if (validateContent(content, NbtFileFormat::LittleEndianBinaryWithHeader)) {
        return NbtFileFormat::LittleEndianBinaryWithHeader;
    } else if (validateContent(content, NbtFileFormat::LittleEndianBinary)) {
        return NbtFileFormat::LittleEndianBinary;
    } else if (validateContent(content, NbtFileFormat::BigEndianBinaryWithHeader)) {
        return NbtFileFormat::BigEndianBinaryWithHeader;
    } else if (validateContent(content, NbtFileFormat::BigEndianBinary)) {
        return NbtFileFormat::BigEndianBinary;
    } else if (validateContent(content, NbtFileFormat::BedrockNetwork)) {
        return NbtFileFormat::BedrockNetwork;
    } else {
        return std::nullopt;
    }
}

std::optional<NbtFileFormat> checkNbtFileFormat(std::filesystem::path const& path, bool fileMemoryMap) {
    if (std::filesystem::exists(path)) {
        std::string content;
        if (fileMemoryMap) {
            readFileMMap(path, content);
        } else {
            std::ifstream fRead(path, std::ios::ate | std::ios::binary);
            if (fRead.is_open()) {
                auto size = fRead.tellg();
                fRead.seekg(0);
                content.resize(static_cast<size_t>(size));
                fRead.read(content.data(), size);
            }
        }
        if (content.size() > 2) {
            const auto& b0 = static_cast<std::byte>(static_cast<uint8_t const&>(content[0]));
            const auto& b1 = static_cast<std::byte>(static_cast<uint8_t const&>(content[1]));
            if ((b0 == 0x1F_byte && b1 == 0x8B_byte)
                || (b0 == 0x78_byte && (b1 == 0x01_byte || b1 == 0x9C_byte || b1 == 0xDA_byte))) {
                std::istringstream stream(content);
                zstr::istream      decompressor(stream);
                content.assign(std::istreambuf_iterator<char>(decompressor), std::istreambuf_iterator<char>());
            }
        }
        return checkNbtContentFormat(content);
    }
    return std::nullopt;
}

std::optional<CompoundTag> _parseFromBinary(std::string& content, std::optional<NbtFileFormat> format) {
    if (content.size() > 2) {
        const auto& b0 = static_cast<std::byte>(static_cast<uint8_t const&>(content[0]));
        const auto& b1 = static_cast<std::byte>(static_cast<uint8_t const&>(content[1]));
        if ((b0 == 0x1F_byte && b1 == 0x8B_byte)
            || (b0 == 0x78_byte && (b1 == 0x01_byte || b1 == 0x9C_byte || b1 == 0xDA_byte))) {
            std::istringstream stream(content);
            zstr::istream      decompressor(stream);
            content.assign(std::istreambuf_iterator<char>(decompressor), std::istreambuf_iterator<char>());
        }
    }
    if (!format.has_value()) { format = checkNbtContentFormat(content); }
    if (!format.has_value()) { return std::nullopt; }
    switch (*format) {
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

std::optional<CompoundTag> parseFromBinary(std::string_view content, std::optional<NbtFileFormat> format) {
    std::string input(content);
    return _parseFromBinary(input, format);
}

std::optional<CompoundTag>
parseFromFile(std::filesystem::path const& path, std::optional<NbtFileFormat> format, bool fileMemoryMap) {
    if (std::filesystem::exists(path)) {
        std::string content;
        if (fileMemoryMap) {
            readFileMMap(path, content);
        } else {
            std::ifstream fRead(path, std::ios::ate | std::ios::binary);
            if (fRead.is_open()) {
                auto size = fRead.tellg();
                fRead.seekg(0);
                content.resize(static_cast<size_t>(size));
                fRead.read(content.data(), size);
            }
        }
        return _parseFromBinary(content, format);
    }
    return std::nullopt;
}

std::string saveAsBinary(
    CompoundTag const& nbt,
    NbtFileFormat      format,
    CompressionType    compressionType,
    CompressionLevel   compressionLevel,
    std::optional<int> headerVersion
) {
    std::string content;
    switch (format) {
    case NbtFileFormat::LittleEndianBinary: {
        content = nbt.toBinaryNbt(true);
        break;
    }
    case NbtFileFormat::LittleEndianBinaryWithHeader: {
        content = nbt.toBinaryNbtWithHeader(true, headerVersion);
        break;
    }
    case NbtFileFormat::BigEndianBinary: {
        content = nbt.toBinaryNbt(false);
        break;
    }
    case NbtFileFormat::BigEndianBinaryWithHeader: {
        content = nbt.toBinaryNbtWithHeader(false, headerVersion);
        break;
    }
    case NbtFileFormat::BedrockNetwork: {
        content = nbt.toNetworkNbt();
        break;
    }
    default: {
        break;
    }
    }
    static constexpr auto BUFFER_SIZE = 1048576ull;
    std::ostringstream    outstream;
    switch (compressionType) {
    case CompressionType::Zlib: {
        zstr::ostream zstream(outstream, BUFFER_SIZE, static_cast<int>(compressionLevel), 15);
        zstream << content;
        zstream.flush();
        return outstream.str();
    }
    case CompressionType::Gzip: {
        zstr::ostream zstream(outstream, BUFFER_SIZE, static_cast<int>(compressionLevel), 31);
        zstream << content;
        zstream.flush();
        return outstream.str();
    }
    default:
        return content;
    }
}

bool saveToFile(
    CompoundTag const&           nbt,
    std::filesystem::path const& path,
    NbtFileFormat                format,
    CompressionType              compressionType,
    CompressionLevel             compressionLevel,
    std::optional<int>           headerVersion
) {
    std::string content;
    switch (format) {
    case NbtFileFormat::LittleEndianBinary: {
        content = nbt.toBinaryNbt(true);
        break;
    }
    case NbtFileFormat::LittleEndianBinaryWithHeader: {
        content = nbt.toBinaryNbtWithHeader(true, headerVersion);
        break;
    }
    case NbtFileFormat::BigEndianBinary: {
        content = nbt.toBinaryNbt(false);
        break;
    }
    case NbtFileFormat::BigEndianBinaryWithHeader: {
        content = nbt.toBinaryNbtWithHeader(false, headerVersion);
        break;
    }
    case NbtFileFormat::BedrockNetwork: {
        content = nbt.toNetworkNbt();
        break;
    }
    default: {
        return false;
    }
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
        if (detail::validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
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
        if (detail::validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
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
        if (detail::validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
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
        if (detail::validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
        break;
    }
    case NbtFileFormat::BedrockNetwork: {
        bstream::ReadOnlyBinaryStream stream(binary, false, false);
        auto                          streamSize = stream.size();
        if (static_cast<Tag::Type>(stream.getByte()) != Tag::Type::Compound) { return false; }
        auto strSize = static_cast<size_t>(stream.getUnsignedVarInt());
        if (stream.isOverflowed() || stream.getPosition() + strSize > streamSize) { return false; }
        stream.ignoreBytes(strSize);
        if (detail::validateCompoundTag(stream, streamSize) && !stream.hasDataLeft()) { return true; }
        break;
    }
    default:
        break;
    }
    return false;
}

bool validateFile(std::filesystem::path const& path, NbtFileFormat format, bool fileMemoryMap) {
    if (std::filesystem::exists(path)) {
        std::string content;
        if (fileMemoryMap) {
            readFileMMap(path, content);
        } else {
            std::ifstream fRead(path, std::ios::ate | std::ios::binary);
            if (fRead.is_open()) {
                auto size = fRead.tellg();
                fRead.seekg(0);
                content.resize(static_cast<size_t>(size));
                fRead.read(content.data(), size);
            }
        }
        if (content.size() > 2) {
            const auto& b0 = static_cast<std::byte>(static_cast<uint8_t const&>(content[0]));
            const auto& b1 = static_cast<std::byte>(static_cast<uint8_t const&>(content[1]));
            if ((b0 == 0x1F_byte && b1 == 0x8B_byte)
                || (b0 == 0x78_byte && (b1 == 0x01_byte || b1 == 0x9C_byte || b1 == 0xDA_byte))) {
                std::istringstream stream(content);
                zstr::istream      decompressor(stream);
                content.assign(std::istreambuf_iterator<char>(decompressor), std::istreambuf_iterator<char>());
            }
        }
        return validateContent(content, format);
    }
    return false;
}

std::string encodeBsae64(std::string_view content) { return base64_utils::encode(content); }

std::string decodeBsae64(std::string_view content) { return base64_utils::decode(content); }

std::optional<CompoundTag> parseFromBsae64(std::string_view content, std::optional<NbtFileFormat> format) {
    auto input = base64_utils::decode(content);
    return _parseFromBinary(input, format);
}

std::string saveAsBase64(
    CompoundTag const& nbt,
    NbtFileFormat      format,
    CompressionType    compressionType,
    CompressionLevel   compressionLevel,
    std::optional<int> headerVersion
) {
    auto content = saveAsBinary(nbt, format, compressionType, compressionLevel, headerVersion);
    return base64_utils::encode(content);
}

int parseHeaderVersionFromContent(std::string_view content) { return CompoundTag::readHeaderVersion(content); }

int parseHeaderVersionFromFile(std::filesystem::path const& path) {
    std::ifstream file(path, std::ios::binary);
    if (file) {
        std::string buffer(4, '\0');
        file.read(buffer.data(), 4);
        file.close();
        return CompoundTag::readHeaderVersion(buffer);
    }
    return 0;
}

} // namespace nbt::io