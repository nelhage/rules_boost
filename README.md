# `rules_boost` -- Bazel build rules for Boost

To use these rules, add the following to your `WORKSPACE` file:

```bazel
git_repository(
    name = "com_github_nelhage_boost",
    commit = "d6446dc9de6e43b039af07482a9361bdc6da5237",
    remote = "https://github.com/nelhage/rules_boost",
)

load("@com_github_nelhage_boost//:boost/boost.bzl", "boost_deps")
boost_deps()
```

You can then use libraries in `deps` through the `@boost` repository, for
example `@boost//:algorithm`.


Based in part on rules from https://github.com/mzhaom/trunk.
