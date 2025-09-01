# High-Performance Minecraft NBT Library for Morden C++

[![GitHub License](https://img.shields.io/github/license/GlacieTeam/NBT)](https://www.mozilla.org/en-US/MPL/2.0/)
[![C++](https://img.shields.io/badge/C++-23-blue?logo=C%2B%2B&logoColor=41a3ed)](https://en.cppreference.com/w/cpp/compiler_support.html) 

A blazing-fast C++ library for parsing, generating, and manipulating Minecraft Named Binary Tag (NBT) data with benchmarked speeds of 210-220 MB/s.

## Features ⚡
- **Ultra-fast parsing** - Achieves 210-220 MB/s processing speed on modern CPUs
- **Complete NBT support** - All NBT types (TAG_Compound, TAG_List, TAG_Int_Array, etc.)
- **Zero-copy design** - Minimal memory allocation during parsing
- **Streaming interface** - Process large NBT files with constant memory footprint
- **Cross-platform** - Windows, Linux, macOS support
- **Endian-aware** - Automatic little/big-endian conversion
- **Zlib/Gzip support** - Built-in compression handling
- **Simple API** - Intuitive interface inspired by standard libraries

## Benchmarks 🚀
Parsing speed: 210-220 MB/s
> Tested on Intel i7 14700-HX with 32GB DDR5-5400 using 720MB little-endian binary NBT

## Quick Start 🚀
```C++
using namespace nbt;
using namespace nbt::literals;

CompoundTag nbt = {
    {"Byte",      64_b                                   },
    {"Short",     16384_s                                },
    {"Int",       12345678                               },
    {"Int64",     5678900123456_l                        },
    {"Float",     3.14159f                               },
    {"Double",    1.23456789                             },
    {"String",    "homo string"                          },
    {"ByteArray", ByteArrayTag({2, 3, 4, 5, 6, 7, 8, 9}) },
    {"List",      ListTag()                              },
    {"Compound",  CompoundTag()                          },
    {"IntArray",  IntArrayTag({212345, 3765432, 4234567})},
    {"LongArray", LongArrayTag({45, 432, 244, 524})      },
};

nbt.toBinaryNbt();   // to binary nbt format
nbt.toNetworkNbt();  // to network nbt format
```

## Install and Using 🔧
### Requirements
- C++23 compatible compiler (GCC 13+, Clang 17+, MSVC 2022+)
- Xmake 3.0.0+

### Build Scripts
```xmake
add_repositories("groupmountain-repo https://github.com/GroupMountain/xmake-repo.git")

add_requires("nbt")
```

## Building from Source 🔧
- Build with Xmake
```bash
xmake --all
```
- If you want to use Cmake build system, you can generate CmakeLists.txt
```bash
xmake project -k cmake
```


## Contributing 🤝
Contributions are welcome! Please follow these steps:

1. Fork the repository and create your feature branch
2. Add tests for any new functionality
3. Submit a pull request with detailed description


## License 📄
This project is licensed under the **Mozilla Public License 2.0 (MPL-2.0)**.  

### Key Requirements:
- **Modifications to this project's files** must be released under MPL-2.0.  
- **Static/dynamic linking with closed-source projects** is allowed (no requirement to disclose your own code).  
- **Patent protection** is explicitly granted to all users.  

For the full license text, see [LICENSE](LICENSE) file or visit [MPL 2.0 Official Page](https://www.mozilla.org/en-US/MPL/2.0/).  

---

### Copyright © 2025 GlacieTeam. All rights reserved.