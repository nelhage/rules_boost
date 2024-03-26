load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

def _non_module_dependencies_impl(_ctx):
    boost_deps()

non_module_dependencies = module_extension(
    implementation = _non_module_dependencies_impl,
)
