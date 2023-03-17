load("@bazel_skylib//lib:selects.bzl", "selects")

licenses(["notice"])  # BSD/MIT-like license (for zlib)

alias(
    name = "zlib",
    actual = selects.with_or({
        ("@platforms//os:android", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ":zlib_system",
        "//conditions:default": ":zlib_source",
    }),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "zlib_source",
    srcs = [
        "adler32.c",
        "compress.c",
        "crc32.c",
        "crc32.h",
        "deflate.c",
        "deflate.h",
        "gzclose.c",
        "gzguts.h",
        "gzlib.c",
        "gzread.c",
        "gzwrite.c",
        "infback.c",
        "inffast.c",
        "inffast.h",
        "inffixed.h",
        "inflate.c",
        "inflate.h",
        "inftrees.c",
        "inftrees.h",
        "trees.c",
        "trees.h",
        "uncompr.c",
        "zconf.h",
        "zutil.c",
        "zutil.h",
    ],
    hdrs = ["zlib.h"],
    copts = select({
        "@platforms//os:windows": [],
        "//conditions:default": ["-Wno-shift-negative-value"],
    }),
    includes = ["."],
    local_defines = select({
        "@platforms//os:windows": [],
        "//conditions:default": ["Z_HAVE_UNISTD_H"],
    }),
)

# For OSs that bundle libz
cc_library(
    name = "zlib_system",
    linkopts = ["-lz"],
)
