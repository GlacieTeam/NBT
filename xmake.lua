add_rules("mode.debug", "mode.release")

add_repositories("groupmountain-repo https://github.com/GroupMountain/xmake-repo.git")

add_requires("binarystream 1.1.1")

if is_plat("windows") and not has_config("vs_runtime") then
    set_runtimes("MD")
end

target("NBT")
    set_kind("static")
    set_languages("cxx20")
    set_exceptions("none")
    add_packages("binarystream")
    add_includedirs("include")
    add_files("src/**.cpp")
    if is_mode("debug") then
        set_symbols("debug")
    else
        set_optimize("aggressive")
        set_strip("all")
    end
    
    if is_plat("windows") then
        add_defines(
            "NOMINMAX",
            "UNICODE"
        )
        add_cxflags(
            "/EHa",
            "/utf-8",
            "/W4"
        )
        if is_mode("release") then
            add_cxflags(
                "/O2",
                "/Ob3"
            )
        end
    else
        set_toolchains("clang")
        add_cxxflags("-Wno-gnu-line-marker")
        add_cxflags(
            "-Wall",
            "-pedantic",
            "-fexceptions",
            "-stdlib=libc++"
        )
        add_ldflags(
            "-stdlib=libc++"
        )
        if is_mode("release") then
            add_cxflags(
                "-O3"
            )
        end
    end