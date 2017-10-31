# Description:
#   bzip2 is a high-quality data compressor.

licenses(["notice"])  # BSD derivative

cc_library(
    name = "bz2lib",
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
    visibility = ["//visibility:public"],
)
