cc_library(
    name = "zstd",
    srcs = glob([
        "lib/common/*.h",
        "lib/common/*.c",
        "lib/compress/*.h",
        "lib/compress/*.c",
        "lib/decompress/*.h",
        "lib/decompress/*.c",
        "lib/deprecated/*.h",
        "lib/deprecated/*.c",
        "lib/dictBuilder/*.h",
        "lib/dictBuilder/*.c",
        "lib/legacy/*.h",
        "lib/legacy/*.c",
    ]) + select({
        ":linux_x86_64": ["lib/decompress/huf_decompress_amd64.S"],
        "//conditions:default": [],
    }),
    hdrs = [
        "lib/zdict.h",
        "lib/zstd.h",
        "lib/zstd_errors.h",
    ],
    includes = ["lib"],
    linkopts = [
        "-pthread",
    ],
    local_defines = [
        "ZSTD_LEGACY_SUPPORT=4",
        "ZSTD_MULTITHREAD",
        "XXH_NAMESPACE=ZSTD_",
    ],
    visibility = ["//visibility:public"],
)

# Hopefully, the need for these OSxCPU config_setting()s will be obviated by a fix to https://github.com/bazelbuild/platforms/issues/36

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)
