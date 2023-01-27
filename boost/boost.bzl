load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

hdrs_patterns = [
    "boost/%s.h",
    "boost/%s_fwd.h",
    "boost/%s.hpp",
    "boost/%s_fwd.hpp",
    "boost/%s/**/*.hpp",
    "boost/%s/**/*.ipp",
    "boost/%s/**/*.h",
    "libs/%s/src/*.ipp",
]

srcs_patterns = [
    "libs/%s/src/*.cpp",
    "libs/%s/src/*.hpp",
]

# Building boost results in many warnings for unused values. Downstream users
# won't be interested, so just disable the warning.
default_copts = select({
    "@platforms//os:windows": [],
    "//conditions:default": ["-Wno-unused"],
})

default_defines = select({
    ":windows_x86_64": ["BOOST_ALL_NO_LIB"],  # Turn auto_link off in MSVC compiler
    "//conditions:default": [],
})

def srcs_list(library_name, exclude):
    return native.glob(
        [p % (library_name,) for p in srcs_patterns],
        exclude = exclude,
        allow_empty = True,
    )

def hdr_list(library_name, exclude = []):
    return native.glob([p % (library_name,) for p in hdrs_patterns], exclude = exclude, allow_empty = True)

def boost_library(
        name,
        boost_name = None,
        defines = None,
        local_defines = None,
        includes = None,
        hdrs = None,
        srcs = None,
        deps = None,
        copts = None,
        exclude_src = [],
        exclude_hdr = [],
        linkopts = None,
        linkstatic = None,
        visibility = ["//visibility:public"]):
    if boost_name == None:
        boost_name = name

    if defines == None:
        defines = []

    if local_defines == None:
        local_defines = []

    if includes == None:
        includes = []

    if hdrs == None:
        hdrs = []

    if srcs == None:
        srcs = []

    if deps == None:
        deps = []

    if copts == None:
        copts = []

    if linkopts == None:
        linkopts = []

    return native.cc_library(
        name = name,
        visibility = visibility,
        defines = default_defines + defines,
        includes = ["."] + includes,
        local_defines = local_defines,
        hdrs = hdr_list(boost_name, exclude_hdr) + hdrs,
        srcs = srcs_list(boost_name, exclude_src) + srcs,
        deps = deps,
        copts = default_copts + copts,
        linkopts = linkopts,
        linkstatic = linkstatic,
        licenses = ["notice"],
    )

# Some boost libraries are not safe to use as dynamic libraries unless a
# BOOST_*_DYN_LINK define is set when they are compiled and included, notably
# Boost.Test. When the define is set, the libraries are not safe to use
# statically. This is an attempt to work around that. We build an explicit .so
# with cc_binary's linkshared=True and then we reimport it as a C++ library and
# expose it as a boost_library.

def boost_so_library(
        name,
        boost_name = None,
        defines = [],
        srcs = [],
        deps = [],
        copts = [],
        exclude_src = [],
        exclude_hdr = []):
    if boost_name == None:
        boost_name = name

    native.cc_binary(
        name = "lib_internal_%s" % name,
        visibility = ["//visibility:private"],
        srcs = hdr_list(boost_name, exclude_hdr) + srcs_list(boost_name, exclude_src) + srcs,
        deps = deps,
        copts = default_copts + copts,
        defines = default_defines + defines,
        linkshared = True,
        licenses = ["notice"],
    )
    native.filegroup(
        name = "%s_dll_interface_file" % name,
        srcs = [":lib_internal_%s" % name],
        output_group = "interface_library",
        visibility = ["//visibility:private"],
    )
    native.cc_import(
        name = "_imported_%s" % name,
        shared_library = ":lib_internal_%s" % name,
        interface_library = ":%s_dll_interface_file" % name,
        visibility = ["//visibility:private"],
    )
    return boost_library(
        name = name,
        boost_name = boost_name,
        defines = defines,
        exclude_hdr = exclude_hdr,
        exclude_src = native.glob([
            "libs/%s/**" % boost_name,
        ]),
        deps = deps + [":_imported_%s" % name],
    )

def boost_deps():
    maybe(
        http_archive,
        name = "bazel_skylib",
        url = "https://github.com/bazelbuild/bazel-skylib/archive/1.4.0.tar.gz",
        sha256 = "4dd05f44200db3b78f72f56ebd8b102d5bcdc17c0299955d4eb20c38c6f07cd7",
        strip_prefix = "bazel-skylib-1.4.0",
    )

    maybe(
        http_archive,
        name = "net_zlib_zlib",
        build_file = "@com_github_nelhage_rules_boost//:BUILD.zlib",
        url = "https://github.com/madler/zlib/archive/v1.2.13.tar.gz",
        sha256 = "1525952a0a567581792613a9723333d7f8cc20b87a81f920fb8bc7e3f2251428",
        strip_prefix = "zlib-1.2.13",
    )

    maybe(
        http_archive,
        name = "org_bzip_bzip2",
        build_file = "@com_github_nelhage_rules_boost//:BUILD.bzip2",
        sha256 = "ab5a03176ee106d3f0fa90e381da478ddae405918153cca248e682cd0c4a2269",
        strip_prefix = "bzip2-1.0.8",
        urls = [
            "https://mirror.bazel.build/sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz",
            "https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz",
        ]
    )

    SOURCEFORGE_MIRRORS = ["cfhcable", "superb-sea2", "cytranet", "iweb", "gigenet", "ayera", "astuteinternet", "pilotfiber", "svwh"]

    maybe(
        http_archive,
        name = "org_lzma_lzma",
        build_file = "@com_github_nelhage_rules_boost//:BUILD.lzma",
        sha256 = "06327c2ddc81e126a6d9a78b0be5014b976a2c0832f492dcfc4755d7facf6d33",
        strip_prefix = "xz-5.2.7",
        urls = [
            "https://%s.dl.sourceforge.net/project/lzmautils/xz-5.2.7.tar.gz" % m
            for m in SOURCEFORGE_MIRRORS
        ],
    )

    maybe(
        http_archive,
        name = "com_github_facebook_zstd",
        build_file = "@com_github_nelhage_rules_boost//:BUILD.zstd",
        url = "https://github.com/facebook/zstd/archive/v1.5.2/zstd-1.5.2.tar.gz",
        sha256 = "f7de13462f7a82c29ab865820149e778cbfe01087b3a55b5332707abf9db4a6e",
        strip_prefix = "zstd-1.5.2",
    )

    maybe(
        http_archive,
        name = "boost",
        build_file = "@com_github_nelhage_rules_boost//:BUILD.boost",
        patch_cmds = ["rm -f doc/pdf/BUILD"],
        patch_cmds_win = ["Remove-Item -Force doc/pdf/BUILD"],
        sha256 = "205666dea9f6a7cfed87c7a6dfbeb52a2c1b9de55712c9c1a87735d7181452b6",
        strip_prefix = "boost_1_81_0",
        urls = [
            "https://mirror.bazel.build/boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz",
            "https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz",
        ],
    )

    # We're pointing at hedronvision's mirror of google/boringssl:master-with-bazel to get Renovate auto-update. Otherwise, Renovate will keep moving us back to master, which doesn't support Bazel. See https://github.com/renovatebot/renovate/issues/18492
    maybe(
        http_archive,
        name = "openssl",
        url = "https://github.com/hedronvision/boringssl/archive/3f5914ce541eb31463c5e4266eec8011c63cd601.tar.gz",
        sha256 = "6ce0bb10a86e7f40bfda04810b8fad0014b1b6c1bcc7046981bf81a8a2b3c3eb",
        strip_prefix = "boringssl-3f5914ce541eb31463c5e4266eec8011c63cd601",
    )
