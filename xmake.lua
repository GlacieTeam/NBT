add_rules("mode.debug", "mode.release")

add_repositories("groupmountain-repo https://github.com/GroupMountain/xmake-repo.git")

add_requires("binarystream 1.0.1")

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

target("NBT")
    set_kind("static")
    set_languages("cxx23")
    set_symbols("debug")   
    set_exceptions("none")
    add_includedirs("include")
    add_defines(
        "NOMINMAX", 
        "UNICODE"
    )
    add_cxflags(
        "/EHa",
        "/utf-8",
        "/W4",
        "/w44265",
        "/w44289",
        "/w44296",
        "/w45263",
        "/w44738",
        "/w45204",
        "/O2",
        "/Ob3"
    )
    add_ldflags(
        "/OPT:REF", 
        "/OPT:ICF"
    )
    add_files("src/**.cpp")
    add_packages("binarystream")