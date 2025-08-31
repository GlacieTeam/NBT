// Copyright © 2025 GlacieTeam.All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#pragma once
#include <filesystem>
#include <nbt-c/Macros.h>
#include <nbt/CompoundTagVariant.hpp>
#include <optional>

namespace nbt {

enum class NbtFileFormat : uint8_t {
    LittleEndianBinary           = 0,
    LittleEndianBinaryWithHeader = 1,
    BigEndianBinary              = 2,
    BigEndianBinaryWithHeader    = 3,
    BedrockNetwork               = 4,
    SNBT                         = 5,
};

[[nodiscard]] NBT_API std::optional<CompoundTag>
parseFromFile(std::filesystem::path const& path, NbtFileFormat format = NbtFileFormat::LittleEndianBinary);

NBT_API bool saveToFile(
    CompoundTag const&           nbt,
    std::filesystem::path const& path,
    NbtFileFormat                format = NbtFileFormat::LittleEndianBinary
);

NBT_API bool saveSnbtToFile(
    CompoundTag const&           nbt,
    std::filesystem::path const& path,
    SnbtFormat                   format = SnbtFormat::PrettyFilePrint,
    uint8_t                      indent = 4
);

} // namespace nbt