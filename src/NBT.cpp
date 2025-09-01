// Copyright © 2025 GlacieTeam.All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "NBT.hpp"
#include <fstream>

namespace nbt {

std::optional<CompoundTag> parseFromFile(std::filesystem::path const& path, NbtFileFormat format) {
    if (std::filesystem::exists(path)) {
        auto mode = std::ios::ate;
        if (format != NbtFileFormat::SNBT) mode |= std::ios::binary;
        std::ifstream fRead(path, mode);
        if (fRead.is_open()) {
            std::string content;
            auto        size = fRead.tellg();
            fRead.seekg(0);
            content.resize(size);
            fRead.read(content.data(), size);
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
            case NbtFileFormat::SNBT: {
                return CompoundTag::fromSnbt(content);
            }
            default:
                return std::nullopt;
            }
        }
    }
    return std::nullopt;
}

bool saveToFile(CompoundTag const& nbt, std::filesystem::path const& path, NbtFileFormat format) {
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
    case NbtFileFormat::SNBT: {
        content = nbt.toSnbt(SnbtFormat::Minimize);
        break;
    }
    default:
        return false;
    }
    auto mode = std::ios::out;
    if (format != NbtFileFormat::SNBT) mode |= std::ios::binary;
    if (!std::filesystem::exists(path.parent_path())) { std::filesystem::create_directories(path.parent_path()); }
    std::ofstream fWrite(path, mode);
    if (fWrite.is_open()) {
        fWrite.write(content.data(), static_cast<std::streamsize>(content.size()));
        fWrite.close();
        return true;
    }
    return false;
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