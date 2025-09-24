// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <filesystem>
#include <nbt/CompoundTagVariant.hpp>

namespace nbt {

enum class NbtFileFormat : uint8_t {
    LittleEndian           = 0,
    LittleEndianWithHeader = 1,
    BigEndian              = 2,
    BigEndianWithHeader    = 3,
    BedrockNetwork         = 4,
};

enum class NbtCompressionType : uint8_t {
    None = 0,
    Gzip = 1,
    Zlib = 2,
};

enum class NbtCompressionLevel : int {
    Default         = -1,
    NoCompression   = 0,
    BestSpeed       = 1,
    Low             = 2,
    MediumLow       = 3,
    Medium          = 4,
    MediumHigh      = 5,
    High            = 6,
    VeryHigh        = 7,
    Ultra           = 8,
    BestCompression = 9,
};

class NbtFile {
public:
    std::filesystem::path              mFilePath;
    CompoundTag                        mFileData;
    bool                               mIsSnbtFile;
    std::optional<NbtFileFormat>       mFileFormat;
    std::optional<NbtCompressionType>  mCompressionType;
    std::optional<NbtCompressionLevel> mCompressionLevel;
    std::optional<SnbtFormat>          mSnbtFormat;
    std::optional<uint8_t>             mSnbtIndent;
    bool                               mAutoSave;

public:
    using iterator               = CompoundTag::TagMap::iterator;
    using const_iterator         = CompoundTag::TagMap::const_iterator;
    using reverse_iterator       = CompoundTag::TagMap::reverse_iterator;
    using const_reverse_iterator = CompoundTag::TagMap::const_reverse_iterator;

public:
    NbtFile() = delete;

    NBT_API NbtFile(
        std::filesystem::path const&       filePath,
        CompoundTag&&                      fileData,
        bool                               isSnbt,
        std::optional<NbtFileFormat>       fileFormat,
        std::optional<NbtCompressionType>  compressionType,
        std::optional<NbtCompressionLevel> compressionLevel,
        std::optional<SnbtFormat>          snbtFormat,
        std::optional<uint8_t>             snbtIndent
    );

    NBT_API ~NbtFile();

    NBT_API void setFilePath(std::filesystem::path const& filePath);

    NBT_API void setFileData(CompoundTag const& fileData);

    NBT_API void setIsSnbtFile(bool isSnbt);

    NBT_API void setFileFormat(NbtFileFormat format);

    NBT_API void setCompressionType(NbtCompressionType type);

    NBT_API void setCompressionLevel(NbtCompressionLevel level);

    NBT_API void setSnbtFormat(SnbtFormat format);

    NBT_API void setSnbtIndent(uint8_t indent);

    NBT_API void setAutoSave(bool value);

    NBT_API void save() const;

public:
    NBT_API void merge(CompoundTag const& other, bool mergeList = false);

    [[nodiscard]] NBT_API CompoundTag::TagMap& items() noexcept;
    [[nodiscard]] NBT_API CompoundTag::TagMap const& items() const noexcept;

    [[nodiscard]] NBT_API CompoundTagVariant&       operator[](std::string_view index);
    [[nodiscard]] NBT_API CompoundTagVariant const& operator[](std::string_view index) const;

    template <size_t N>
    [[nodiscard]] CompoundTagVariant& operator[](char const (&index)[N]) {
        return operator[](std::string_view{index, N - 1});
    }

    template <size_t N>
    [[nodiscard]] CompoundTagVariant const& operator[](char const (&index)[N]) const {
        return operator[](std::string_view{index, N - 1});
    }

    [[nodiscard]] NBT_API CompoundTagVariant&       at(std::string_view index);
    [[nodiscard]] NBT_API CompoundTagVariant const& at(std::string_view index) const;

    [[nodiscard]] NBT_API iterator begin() noexcept;
    [[nodiscard]] NBT_API iterator end() noexcept;

    [[nodiscard]] NBT_API const_iterator begin() const noexcept;
    [[nodiscard]] NBT_API const_iterator end() const noexcept;

    [[nodiscard]] NBT_API reverse_iterator rbegin() noexcept;
    [[nodiscard]] NBT_API reverse_iterator rend() noexcept;

    [[nodiscard]] NBT_API const_iterator cbegin() const noexcept;
    [[nodiscard]] NBT_API const_iterator cend() const noexcept;

    [[nodiscard]] NBT_API const_reverse_iterator crbegin() const noexcept;
    [[nodiscard]] NBT_API const_reverse_iterator crend() const noexcept;

    NBT_API iterator erase(const_iterator where) noexcept;
    NBT_API iterator erase(const_iterator first, const_iterator last) noexcept;

    [[nodiscard]] NBT_API bool contains(std::string_view key) const;
    [[nodiscard]] NBT_API bool contains(std::string_view key, Tag::Type type) const;

    [[nodiscard]] NBT_API bool empty() const noexcept;

    [[nodiscard]] NBT_API size_t size() const noexcept;

    NBT_API bool remove(std::string_view index);
    NBT_API bool rename(std::string_view index, std::string_view newName);
    NBT_API void clear() noexcept;

    [[nodiscard]] NBT_API std::string toNetworkNbt() const noexcept;
    [[nodiscard]] NBT_API std::string toBinaryNbt(bool isLittleEndian = true) const noexcept;
    [[nodiscard]] NBT_API             std::string
    toBinaryNbtWithHeader(bool isLittleEndian = true, std::optional<int> storageVersion = std::nullopt) const noexcept;
    [[nodiscard]] NBT_API std::string
                          toSnbt(SnbtFormat snbtFormat = SnbtFormat::Default, uint8_t indent = 4) const noexcept;
    [[nodiscard]] NBT_API std::string toJson(uint8_t indent = 4) const noexcept;

public:
    [[nodiscard]] NBT_API static std::optional<NbtFile> openFile(
        std::filesystem::path const& filePath,
        std::optional<NbtFileFormat> fileFormat      = std::nullopt,
        bool                         strictMatchSize = false
    );

    [[nodiscard]] NBT_API static std::optional<NbtFile> openSnbtFile(std::filesystem::path const& filePath);
};

} // namespace nbt
