# Description
#    lzma is a general purpose data compression library https://tukaani.org/xz/
#    Public Domain

load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_skylib//rules:copy_file.bzl", "copy_file")

# Hopefully, the need for these OSxCPU config_setting()s will be obviated by a fix to https://github.com/bazelbuild/platforms/issues/36

config_setting(
    name = "osx_arm64",
    constraint_values = [
        "@platforms//os:osx",
        "@platforms//cpu:aarch64",
    ],
)

config_setting(
    name = "osx_x86_64",
    constraint_values = [
        "@platforms//os:osx",
        "@platforms//cpu:x86_64",
    ],
)

copy_file(
    name = "copy_config",
    src = selects.with_or({
        "@platforms//os:android": "@com_github_nelhage_rules_boost//:config.lzma-android.h",
        "@platforms//os:linux": "@com_github_nelhage_rules_boost//:config.lzma-linux.h",
        ":osx_arm64": "@com_github_nelhage_rules_boost//:config.lzma-osx-arm64.h",
        ":osx_x86_64": "@com_github_nelhage_rules_boost//:config.lzma-osx-x86_64.h",
        ("@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): "apple_config",
        "@platforms//os:windows": "@com_github_nelhage_rules_boost//:config.lzma-windows.h",
    }),
    out = "src/liblzma/api/config.h",  # minimize the number of exported include paths
)

# Configuration is the same across iOS, watchOS, and tvOS
alias(
    name = "apple_config",
    actual = select({
        "@platforms//cpu:arm64": "@com_github_nelhage_rules_boost//:config.lzma-ios-arm64.h",
        "@platforms//cpu:armv7": "@com_github_nelhage_rules_boost//:config.lzma-ios-armv7.h",
        "@platforms//cpu:x86_64": "@com_github_nelhage_rules_boost//:config.lzma-osx-x86_64.h",  # Configuration same as macOS
        "@platforms//cpu:x86_32": "@com_github_nelhage_rules_boost//:config.lzma-ios-i386.h",
    }),
)

# Note: lzma is bundled with Apple platforms, but sadly, not considered public API because its header is not exposed. lzma is not bundled on Android.

cc_library(
    name = "lzma",
    srcs = [
        "src/common/tuklib_cpucores.c",
        "src/common/tuklib_physmem.c",
    ] + glob(
        [
            "src/**/*.h",
            "src/liblzma/**/*.c",
        ],
        exclude = [
            "src/liblzma/check/crc*_small.c",
            "src/liblzma/**/*_tablegen.c",
        ],
    ),
    hdrs = [
        "src/liblzma/api/lzma.h",  # Publicly exported header
    ],
    copts = select({
        "@platforms//os:windows": [],
        "//conditions:default": ["-std=c99"],
    }),
    defines = select({
        "@platforms//os:windows": ["LZMA_API_STATIC"],
        "//conditions:default": [],
    }),
    linkopts = select({
        "@platforms//os:android": [],
        "//conditions:default": ["-lpthread"],
    }),
    linkstatic = select({
        "@platforms//os:windows": True,
        "//conditions:default": False,
    }),
    local_defines = [
        "HAVE_CONFIG_H",
    ],
    strip_include_prefix = "src/liblzma/api",  # Allows public header without the path and without COPTS -I or includes = []
    visibility = ["//visibility:public"],
    deps = [
        "lzma_src_common",
        "lzma_src_liblzma",
        "lzma_src_liblzma_api",
        "lzma_src_liblzma_check",
        "lzma_src_liblzma_common",
        "lzma_src_liblzma_delta",
        "lzma_src_liblzma_lz",
        "lzma_src_liblzma_lzma",
        "lzma_src_liblzma_rangecoder",
        "lzma_src_liblzma_simpler",
    ],
)

cc_library(
    name = "lzma_src_common",
    hdrs = glob(["src/common/*.h"]),
    strip_include_prefix = "src/common",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma",
    hdrs = glob(["src/liblzma/*.h"]),
    strip_include_prefix = "src/liblzma",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma_api",
    srcs = [],
    hdrs = [
        "src/liblzma/api/config.h",  # Generated, so missed by glob. In srcs so it's not public like the other headers
    ] + glob(
        [
            "src/liblzma/api/**/*.h",
        ],
        exclude = [
            "src/liblzma/api/lzma.h",  # The public header, only used in hdrs of main lib (//visibility:public)
        ],
    ),
    strip_include_prefix = "src/liblzma/api",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma_check",
    hdrs = glob(["src/liblzma/check/*.h"]),
    strip_include_prefix = "src/liblzma/check",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma_common",
    hdrs = glob(["src/liblzma/common/*.h"]),
    strip_include_prefix = "src/liblzma/common",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma_delta",
    hdrs = glob(["src/liblzma/delta/*.h"]),
    strip_include_prefix = "src/liblzma/delta",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma_lz",
    hdrs = glob(["src/liblzma/lz/*.h"]),
    strip_include_prefix = "src/liblzma/lz",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma_lzma",
    hdrs = glob(["src/liblzma/lzma/*.h"]),
    strip_include_prefix = "src/liblzma/lzma",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma_rangecoder",
    hdrs = glob(["src/liblzma/rangecoder/*.h"]),
    strip_include_prefix = "src/liblzma/rangecoder",
    visibility = ["//visibility:private"],
)

cc_library(
    name = "lzma_src_liblzma_simpler",
    hdrs = glob(["src/liblzma/simple/*.h"]),
    strip_include_prefix = "src/liblzma/simple",
    visibility = ["//visibility:private"],
)
