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
- Please note that this project is licensed under the LGPLv3.
- If you modify or distribute this project, you must comply with the requirements of the LGPLv3 license, including but not limited to providing the complete source code and retaining the copyright notices. For more detailed information, please visit the GNU Official Website.

### Copyright © 2025 GlacieTeam. All rights reserved.