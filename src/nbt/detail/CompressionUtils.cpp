// Copyright © 2025 GlacieTeam. All rights reserved.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// SPDX-License-Identifier: MPL-2.0

#include "nbt/detail/CompressionUtils.hpp"
#include <cstdint>
#include <zlib.h>

namespace nbt::detail {

static constexpr auto ZLIB_STREAM_CHUNK = 1048576ull;

constexpr std::byte operator""_byte(unsigned long long value) noexcept { return static_cast<std::byte>(value); }

std::string compress(std::string_view input, int level, int windowBits) {
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;
    if (deflateInit2(&strm, level, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        return std::string(input);
    }
    strm.next_in  = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    strm.avail_in = static_cast<uInt>(input.size());
    std::string   output;
    unsigned char out_buffer[ZLIB_STREAM_CHUNK];
    int           ret;
    do {
        strm.next_out  = out_buffer;
        strm.avail_out = ZLIB_STREAM_CHUNK;
        ret            = deflate(&strm, Z_FINISH);
        if (ret == Z_STREAM_ERROR) break;
        size_t have = ZLIB_STREAM_CHUNK - strm.avail_out;
        output.append(reinterpret_cast<char*>(out_buffer), have);
    } while (strm.avail_out == 0);
    deflateEnd(&strm);
    return (ret == Z_STREAM_END) ? output : std::string(input);
}

std::string decompress(std::string_view input) {
    if (input.size() > 2) {
        const auto& b0 = static_cast<std::byte>(static_cast<uint8_t const&>(input[0]));
        const auto& b1 = static_cast<std::byte>(static_cast<uint8_t const&>(input[1]));

        int windowBits = 0;

        if (b0 == 0x1F_byte && b1 == 0x8B_byte) { windowBits = 31; }
        if (b0 == 0x78_byte && (b1 == 0x01_byte || b1 == 0x9C_byte || b1 == 0xDA_byte)) { windowBits = 15; }

        if (windowBits != 0) {
            z_stream zstr;
            zstr.zalloc = Z_NULL;
            zstr.zfree  = Z_NULL;
            zstr.opaque = Z_NULL;
            if (inflateInit2(&zstr, windowBits) != Z_OK) { return std::string(input); }
            zstr.next_in  = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
            zstr.avail_in = static_cast<uInt>(input.size());
            std::string output;
            uint8_t     out_buffer[ZLIB_STREAM_CHUNK];
            int         ret;
            do {
                zstr.next_out  = out_buffer;
                zstr.avail_out = ZLIB_STREAM_CHUNK;
                ret            = inflate(&zstr, Z_NO_FLUSH);
                if (ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) break;
                size_t have = ZLIB_STREAM_CHUNK - zstr.avail_out;
                output.append(reinterpret_cast<char*>(out_buffer), have);
            } while (zstr.avail_out == 0);
            inflateEnd(&zstr);
            if (ret == Z_STREAM_END) { return output; }
        }
    }
    return std::string(input);
}

} // namespace nbt::detail
