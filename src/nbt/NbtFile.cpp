// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/NbtFile.hpp"
#include "nbt/NBT.hpp"
#include "nbt/detail/FileUtils.hpp"
#include <fstream>

namespace nbt {

NbtFile::NbtFile(
    std::filesystem::path const&       filePath,
    CompoundTag&&                      fileData,
    bool                               isSnbt,
    std::optional<NbtFileFormat>       fileFormat,
    std::optional<NbtCompressionType>  compressionType,
    std::optional<NbtCompressionLevel> compressionLevel,
    std::optional<SnbtFormat>          snbtFormat,
    std::optional<uint8_t>             snbtIndent
)
: mFilePath(filePath),
  mFileData(std::move(fileData)),
  mIsSnbtFile(isSnbt),
  mFileFormat(fileFormat),
  mCompressionType(compressionType),
  mCompressionLevel(compressionLevel),
  mSnbtFormat(snbtFormat),
  mSnbtIndent(snbtIndent),
  mAutoSave(true) {}

NbtFile::~NbtFile() {
    if (mAutoSave) { save(); }
}

void NbtFile::setFilePath(std::filesystem::path const& filePath) { mFilePath = filePath; }

void NbtFile::setFileData(CompoundTag const& fileData) { mFileData = fileData; }

void NbtFile::setIsSnbtFile(bool isSnbt) {
    mIsSnbtFile = isSnbt;
    if (mIsSnbtFile) { mFileFormat.reset(); }
}

void NbtFile::setFileFormat(NbtFileFormat format) {
    mIsSnbtFile = false;
    mFileFormat = format;
}

void NbtFile::setCompressionType(NbtCompressionType type) { mCompressionType = type; }

void NbtFile::setCompressionLevel(NbtCompressionLevel level) { mCompressionLevel = level; }

void NbtFile::setSnbtFormat(SnbtFormat format) {
    setIsSnbtFile(true);
    mSnbtFormat = format;
}

void NbtFile::setSnbtIndent(uint8_t indent) { mSnbtIndent = indent; }

void NbtFile::setAutoSave(bool value) { mAutoSave = value; }

void NbtFile::save() const {
    if (mIsSnbtFile) {
        io::saveSnbtToFile(mFileData, mFilePath, mSnbtFormat.value_or(SnbtFormat::Minimize), mSnbtIndent.value_or(4));
    } else {
        io::saveToFile(
            mFileData,
            mFilePath,
            mFileFormat.value_or(NbtFileFormat::LittleEndian),
            mCompressionType.value_or(NbtCompressionType::Gzip),
            mCompressionLevel.value_or(NbtCompressionLevel::Default)
        );
    }
}

void NbtFile::merge(CompoundTag const& other, bool mergeList) { return mFileData.merge(other, mergeList); }

CompoundTag::TagMap&       NbtFile::items() noexcept { return mFileData.items(); }
CompoundTag::TagMap const& NbtFile::items() const noexcept { return mFileData.items(); }

CompoundTagVariant&       NbtFile::operator[](std::string_view index) { return mFileData.operator[](index); }
CompoundTagVariant const& NbtFile::operator[](std::string_view index) const { return mFileData.operator[](index); }

CompoundTagVariant&       NbtFile::at(std::string_view index) { return mFileData.at(index); }
CompoundTagVariant const& NbtFile::at(std::string_view index) const { return mFileData.at(index); }

NbtFile::iterator NbtFile::begin() noexcept { return mFileData.begin(); }
NbtFile::iterator NbtFile::end() noexcept { return mFileData.end(); }

NbtFile::const_iterator NbtFile::begin() const noexcept { return mFileData.begin(); }
NbtFile::const_iterator NbtFile::end() const noexcept { return mFileData.end(); }

NbtFile::reverse_iterator NbtFile::rbegin() noexcept { return mFileData.rbegin(); }
NbtFile::reverse_iterator NbtFile::rend() noexcept { return mFileData.rend(); }

NbtFile::const_iterator NbtFile::cbegin() const noexcept { return mFileData.cbegin(); }
NbtFile::const_iterator NbtFile::cend() const noexcept { return mFileData.cend(); }

NbtFile::const_reverse_iterator NbtFile::crbegin() const noexcept { return mFileData.crbegin(); }
NbtFile::const_reverse_iterator NbtFile::crend() const noexcept { return mFileData.crend(); }

NbtFile::iterator NbtFile::erase(const_iterator where) noexcept { return mFileData.erase(where); }
NbtFile::iterator NbtFile::erase(const_iterator first, const_iterator last) noexcept {
    return mFileData.erase(first, last);
}

bool NbtFile::contains(std::string_view key) const { return mFileData.contains(key); }
bool NbtFile::contains(std::string_view key, Tag::Type type) const { return mFileData.contains(key, type); }

bool NbtFile::empty() const noexcept { return mFileData.empty(); }

size_t NbtFile::size() const noexcept { return mFileData.size(); }

bool NbtFile::remove(std::string_view index) { return mFileData.remove(index); }
bool NbtFile::rename(std::string_view index, std::string_view newName) { return mFileData.rename(index, newName); }
void NbtFile::clear() noexcept { return mFileData.clear(); }

std::string NbtFile::toNetworkNbt() const noexcept { return mFileData.toNetworkNbt(); }
std::string NbtFile::toBinaryNbt(bool isLittleEndian) const noexcept { return mFileData.toBinaryNbt(isLittleEndian); }
std::string NbtFile::toBinaryNbtWithHeader(bool isLittleEndian, std::optional<int> storageVersion) const noexcept {
    return mFileData.toBinaryNbtWithHeader(isLittleEndian, storageVersion);
}
std::string NbtFile::toSnbt(SnbtFormat snbtFormat, uint8_t indent) const noexcept {
    return mFileData.toSnbt(snbtFormat, indent);
}
std::string NbtFile::toJson(uint8_t indent) const noexcept { return mFileData.toJson(indent); }

std::optional<NbtFile> NbtFile::openFile(
    std::filesystem::path const& filePath,
    std::optional<NbtFileFormat> fileFormat,
    bool                         fileMemoryMap,
    bool                         strictMatchSize
) {
    std::string content;
    detail::readFile(filePath, content, fileMemoryMap);
    if (!fileFormat.has_value()) { fileFormat = io::detectContentFormat(content, strictMatchSize); }
    if (auto data = io::parseFromContent(content, fileFormat, strictMatchSize)) {
        auto compressionType = io::detectContentCompressionType(content);
        return NbtFile(
            filePath,
            std::move(data).value(),
            false,
            fileFormat,
            compressionType,
            NbtCompressionLevel::Default,
            std::nullopt,
            std::nullopt
        );
    }
    return std::nullopt;
}

std::optional<NbtFile> NbtFile::openSnbtFile(std::filesystem::path const& filePath) {
    std::ifstream fRead(filePath, std::ios::ate);
    if (fRead.is_open()) {
        std::string content;
        auto        size = fRead.tellg();
        fRead.seekg(0);
        content.resize(static_cast<size_t>(size));
        fRead.read(content.data(), size);
        if (auto data = CompoundTag::fromSnbt(content)) {
            return NbtFile(
                filePath,
                std::move(data).value(),
                true,
                std::nullopt,
                std::nullopt,
                std::nullopt,
                SnbtFormat::Minimize,
                0
            );
        }
    }
    return std::nullopt;
}

} // namespace nbt
