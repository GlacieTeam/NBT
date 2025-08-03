# NBT
**Part of [Bedrock Procotol Library](https://github.com/GlacieTeam/ProtocolLib).**  
NBT Library written in C++

## Usage
```C++
using namespace bedrock_protocol;

CompoundTag nbt = {
    {"Byte",      (uint8_t)64                            },
    {"Short",     (short)16384                           },
    {"Int",       12345678                               },
    {"Int64",     5678900123456ll                        },
    {"Float",     3.14159f                               },
    {"Double",    1.23456789                             },
    {"String",    "homo string"                          },
    {"ByteArray", ByteArrayTag({2, 3, 4, 5, 6, 7, 8, 9}) },
    {"IntArray",  IntArrayTag({212345, 3765432, 4234567})},
    {"List",      ListTag()                              },
    {"Compound",  CompoundTag()                          }
};

nbt.toBinaryNbt();   // to binary nbt format
nbt.toNetworkNbt();  // to network nbt format
```

## Install
```xmake
add_repositories("groupmountain-repo https://github.com/GroupMountain/xmake-repo.git")

add_requires("binarystream")
```

## Build
- Build with Xmake
```bash
xmake --all
```
- If you want to use Cmake build system, you can generate CmakeLists.txt
```bash
xmake project -k cmake
```

## License
This project is licensed under the **Mozilla Public License 2.0 (MPL-2.0)**.  

### Key Requirements:
- **Modifications to this project's files** must be released under MPL-2.0.  
- **Static/dynamic linking with closed-source projects** is allowed (no requirement to disclose your own code).  
- **Patent protection** is explicitly granted to all users.  

For the full license text, see [LICENSE](LICENSE) file or visit [MPL 2.0 Official Page](https://www.mozilla.org/en-US/MPL/2.0/).  

---

### Copyright © 2025 GlacieTeam. All rights reserved.