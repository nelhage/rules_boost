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
        name = "boost",
        build_file = "@com_github_nelhage_rules_boost//:boost.BUILD",
        patch_cmds = ["rm -f doc/pdf/BUILD"],
        patch_cmds_win = ["Remove-Item -Force doc/pdf/BUILD"],
        patches = ["@com_github_nelhage_rules_boost//:zlib.patch"],
        patch_args = ["-p1"],
        url = "https://github.com/boostorg/boost/releases/download/boost-1.85.0/boost-1.84.0.tar.gz",
        sha256 = "4d27e9efed0f6f152dc28db6430b9d3dfb40c0345da7342eaa5a987dde57bd95",
        strip_prefix = "boost-1.84.0",
    )
