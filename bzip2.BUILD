# Description:
#   bzip2 is a high-quality data compressor.

load("@bazel_skylib//lib:selects.bzl", "selects")

licenses(["notice"])  # BSD derivative

alias(
    name = "bz2lib",
    actual = selects.with_or({
        ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ":bz2lib_system",
        "//conditions:default": ":bz2lib_source",
    }),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "bz2lib_source",
    srcs = [
        "blocksort.c",
        "bzlib.c",
        "bzlib_private.h",
        "compress.c",
        "crctable.c",
        "decompress.c",
        "huffman.c",
        "randtable.c",
    ],
    hdrs = [
        "bzlib.h",
    ],
    includes = ["."],
)

# For OSs that bundle bz2lib
cc_library(
    name = "bz2lib_system",
    linkopts = ["-lbz2"],
)
