# Description
#    lzma is a general purpose data compression library https://tukaani.org/xz/
#    Public Domain

load("@bazel_skylib//rules:copy_file.bzl", "copy_file")
load("@bazel_skylib//lib:selects.bzl", "selects")

# Hopefully, the need for these OSxCPU config_setting()s will be obviated by a fix to https://github.com/bazelbuild/platforms/issues/36

config_setting(
    name = "linux_x86",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "linux_arm64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:arm64",
    ],
)

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
        ":linux_x86": "@com_github_nelhage_rules_boost//:config.lzma-linux.h",
        ":linux_arm64": "@com_github_nelhage_rules_boost//:config.lzma-linux-aarch64.h",
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
        "src/liblzma/api/config.h",  # Generated, so missed by glob.
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
    hdrs = glob(["src/liblzma/api/**/*.h"]),
    copts = select({
        "@platforms//os:windows": [],
        "//conditions:default": ["-std=c99"],
    }) + [
        # Replace with local_includes if it's ever added https://github.com/bazelbuild/bazel/issues/16472
        "-Iexternal/org_lzma_lzma/src/common",
        "-Iexternal/org_lzma_lzma/src/liblzma",
        "-Iexternal/org_lzma_lzma/src/liblzma/api",
        "-Iexternal/org_lzma_lzma/src/liblzma/common",
        "-Iexternal/org_lzma_lzma/src/liblzma/check",
        "-Iexternal/org_lzma_lzma/src/liblzma/delta",
        "-Iexternal/org_lzma_lzma/src/liblzma/lz",
        "-Iexternal/org_lzma_lzma/src/liblzma/lzma",
        "-Iexternal/org_lzma_lzma/src/liblzma/rangecoder",
        "-Iexternal/org_lzma_lzma/src/liblzma/simple",
    ],
    defines = select({
        "@platforms//os:windows": ["LZMA_API_STATIC"],
        "//conditions:default": [],
    }),
    includes = [
        "src/liblzma/api",
    ],
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
    visibility = ["//visibility:public"],
)
