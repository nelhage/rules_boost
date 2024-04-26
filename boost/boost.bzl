load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

# Building boost results in many warnings. Downstream users won't be interested, so just disable them.
default_copts = select({
    "@platforms//os:windows": ["/W0"],
    "//conditions:default": ["-w"],
})

default_defines = select({
    ":windows_x86_64": ["BOOST_ALL_NO_LIB"],  # Turn auto_link off in MSVC compiler
    "//conditions:default": [],
})

def srcs_list(library_name, exclude):
    return native.glob(
        ["libs/%s/src/*" % library_name],
        exclude = ["**/*.asm", "**/*.S", "**/*.doc"] + exclude,
        allow_empty = True,
    )

def hdr_list(library_name, exclude = []):
    return native.glob(["libs/%s/include/boost/**" % library_name], exclude = exclude, allow_empty = True)

def boost_library(
        name,
        boost_name = None,
        defines = [],
        includes = [],
        hdrs = [],
        srcs = [],
        copts = [],
        exclude_hdr = [],
        exclude_src = [],
        visibility = ["//visibility:public"],
        **kwargs):
    if boost_name == None:
        boost_name = name

    return native.cc_library(
        name = name,
        visibility = visibility,
        defines = default_defines + defines,
        includes = ["libs/%s/include" % boost_name] + includes,
        hdrs = hdr_list(boost_name, exclude_hdr) + hdrs,
        srcs = srcs_list(boost_name, exclude_src) + srcs,
        copts = default_copts + copts,
        licenses = ["notice"],
        **kwargs
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
        includes = [],
        hdrs = [],
        srcs = [],
        deps = [],
        copts = [],
        exclude_hdr = [],
        exclude_src = [],
        **kwargs):
    if boost_name == None:
        boost_name = name

    native.cc_binary(
        name = "lib_internal_%s" % name,
        defines = default_defines + defines,
        includes = ["libs/%s/include" % boost_name] + includes,
        srcs = hdr_list(boost_name, exclude_hdr) + hdrs + srcs_list(boost_name, exclude_src) + srcs,
        deps = deps,
        copts = default_copts + copts,
        linkshared = True,
        visibility = ["//visibility:private"],
        **kwargs
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
        includes = includes,
        hdrs = hdrs,
        exclude_hdr = exclude_hdr,
        exclude_src = native.glob(["**"]),
        deps = deps + [":_imported_%s" % name],
        **kwargs
    )

def boost_deps():
    maybe(
        http_archive,
        name = "bazel_skylib",
        url = "https://github.com/bazelbuild/bazel-skylib/releases/download/1.6.1/bazel-skylib-1.6.1.tar.gz",
        sha256 = "9f38886a40548c6e96c106b752f242130ee11aaa068a56ba7e56f4511f33e4f2",
    )

    maybe(
        http_archive,
        name = "zlib",
        build_file = "@com_github_nelhage_rules_boost//:zlib.BUILD",
        url = "https://github.com/madler/zlib/releases/download/v1.3/zlib-1.3.tar.gz",
        sha256 = "ff0ba4c292013dbc27530b3a81e1f9a813cd39de01ca5e0f8bf355702efa593e",
        strip_prefix = "zlib-1.3",
    )

    maybe(
        http_archive,
        name = "org_bzip_bzip2",
        build_file = "@com_github_nelhage_rules_boost//:bzip2.BUILD",
        sha256 = "ab5a03176ee106d3f0fa90e381da478ddae405918153cca248e682cd0c4a2269",
        strip_prefix = "bzip2-1.0.8",
        urls = [
            "https://mirror.bazel.build/sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz",
            "https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz",
        ],
    )

    maybe(
        http_archive,
        name = "org_lzma_lzma",
        build_file = "@com_github_nelhage_rules_boost//:lzma.BUILD",
        url = "https://src.fedoraproject.org/lookaside/extras/xz/xz-5.4.6.tar.gz/sha512/b08a61d8d478d3b4675cb1ddacdbbd98dc6941a55bcdd81a28679e54e9367d3a595fa123ac97874a17da571c1b712e2a3e901c2737099a9d268616a1ba3de497/xz-5.4.6.tar.gz",
        sha256 = "aeba3e03bf8140ddedf62a0a367158340520f6b384f75ca6045ccc6c0d43fd5c",
        strip_prefix = "xz-5.4.6",
    )

    maybe(
        http_archive,
        name = "com_github_facebook_zstd",
        build_file = "@com_github_nelhage_rules_boost//:zstd.BUILD",
        url = "https://github.com/facebook/zstd/releases/download/v1.5.6/zstd-1.5.6.tar.gz",
        sha256 = "8c29e06cf42aacc1eafc4077ae2ec6c6fcb96a626157e0593d5e82a34fd403c1",
        strip_prefix = "zstd-1.5.6",
    )

    maybe(
        http_archive,
        name = "boost",
        build_file = "@com_github_nelhage_rules_boost//:boost.BUILD",
        patch_cmds = ["rm -f doc/pdf/BUILD"],
        patch_cmds_win = ["Remove-Item -Force doc/pdf/BUILD"],
        url = "https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.tar.gz",
        sha256 = "4d27e9efed0f6f152dc28db6430b9d3dfb40c0345da7342eaa5a987dde57bd95",
        strip_prefix = "boost-1.84.0",
    )

    # We're pointing at hedronvision's mirror of google/boringssl:master-with-bazel to get Renovate auto-update. Otherwise, Renovate will keep moving us back to master, which doesn't support Bazel. See https://github.com/renovatebot/renovate/discussions/24854
    maybe(
        http_archive,
        name = "openssl",
        url = "https://github.com/hedronvision/boringssl/archive/0f1a639954dd7ab86f5f4ddd8b4e2edbea492acd.tar.gz",
        sha256 = "7ce152bdce1b85344cc36c6b255aab36905d39187c2c2f797a69d5ad220076ee",
        strip_prefix = "boringssl-0f1a639954dd7ab86f5f4ddd8b4e2edbea492acd",
    )
