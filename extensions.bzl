load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")  # a repo rule

def _boost_modules_impl(ctx):
    """_boost_module_impl

    Downloads the correct version of boost and make the repository available to the requesting module.

    Args:
        ctx: a context object that contains the module's attributes
    """
    for mod in ctx.modules:
        if len(mod.tags.version) == 0:
            boost_version = "1.84.0"
            boost_sha256 = "4d27e9efed0f6f152dc28db6430b9d3dfb40c0345da7342eaa5a987dde57bd95"
            http_archive(
                name = "boost",
                build_file = "@com_github_nelhage_rules_boost//:boost.BUILD",
                patch_cmds = ["rm -f doc/pdf/BUILD"],
                patch_cmds_win = ["Remove-Item -Force doc/pdf/BUILD"],
                url = "https://github.com/boostorg/boost/releases/download/boost-%s/boost-%s.tar.gz" % (boost_version, boost_version),
                sha256 = boost_sha256,
                strip_prefix = "boost-%s" % boost_version,
            )
        for attr in mod.tags.version:
            boost_version = attr.version
            boost_sha256 = attr.sha256
            http_archive(
                name = "boost",
                build_file = "@com_github_nelhage_rules_boost//:boost.BUILD",
                patch_cmds = ["rm -f doc/pdf/BUILD"],
                patch_cmds_win = ["Remove-Item -Force doc/pdf/BUILD"],
                url = "https://github.com/boostorg/boost/releases/download/boost-%s/boost-%s.tar.gz" % (boost_version, boost_version),
                sha256 = boost_sha256,
                strip_prefix = "boost-%s" % boost_version,
            )

_version = tag_class(attrs = {"version": attr.string(), "sha256": attr.string()})
boost_modules = module_extension(
    implementation = _boost_modules_impl,
    tag_classes = {"version": _version},
)
