add_rules("mode.debug", "mode.release")

add_repositories("groupmountain-repo https://github.com/GroupMountain/xmake-repo.git")

add_requires("binarystream 2.1.1")
add_requires("nlohmann_json")

if is_plat("windows") then
    if not has_config("vs_runtime") then
        set_runtimes("MD")
    end
else
    set_toolchains("clang")
end

option("kind")
    set_default("static")
    set_values("static", "shared")
    set_showmenu(true)
option_end()

target("NBT")
    set_kind("$(kind)")
    set_languages("c++23")
    add_packages(
        "binarystream",
        "nlohmann_json"
    )
    add_includedirs(
        "include",
        "src"
    )
    add_files("src/**.cpp")
    if is_mode("debug") then
        set_symbols("debug")
    else
        set_optimize("aggressive")
        set_strip("all")
    end
    if is_config("kind", "shared") then
        add_defines("NBT_EXPORT")
    end
    
    if is_plat("windows") then
        add_defines(
            "NOMINMAX",
            "UNICODE"
        )
        add_cxflags(
            "/EHsc",
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
        if is_config("kind", "shared") then
            add_cxflags(
                "-fvisibility=hidden",
                "-fvisibility-inlines-hidden"
            )
            add_syslinks("c++")
        end
    end
    if is_config("kind", "shared") then
        after_build(function (target)
            local output_dir = path.join(os.projectdir(), "bin")
            os.mkdir(output_dir)
            os.cp(target:targetfile(), path.join(output_dir, path.filename(target:targetfile())))
            cprint("${bright green}[Shared Library]: ${reset}".. path.filename(target:targetfile()) .. " already generated to " .. output_dir)
        end)
    end