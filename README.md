# `rules_boost` -- Bazel build rules for Boost

To use these rules, add the following to your `WORKSPACE` file:

```bazel
git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "6d6fd834281cb8f8e758dd9ad76df86304bf1869",
    remote = "https://github.com/nelhage/rules_boost",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()
```

You can then use libraries in `deps` through the `@boost` repository, for
example `@boost//:algorithm`.


Based in part on rules from https://github.com/mzhaom/trunk.
