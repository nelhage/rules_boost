# `rules_boost` -- Bazel build rules for Boost

To use these rules, add the following to your `WORKSPACE` file:

```bazel
http_archive(
    name = "com_github_nelhage_boost",
    sha256 = "bc42251e12bc35b03eab2edb6179cc06ca4caf9bf884566a28420253d6e118c3",
    strip_prefix = "rules_boost-dbfed66073378041cd0ee2a92d75ddd6def612ec",
    type = "tar.gz",
    urls = [
        "https://github.com/nelhage/rules_boost/archive/dbfed66073378041cd0ee2a92d75ddd6def612ec.tar.gz"
    ],
)

load("@com_github_nelhage_boost//:boost/boost.bzl", "boost_deps")
boost_deps()
```

You can then use libraries in `deps` through the `@boost` repository, for
example `@boost//:algorithm`.


Based in part on rules from https://github.com/mzhaom/trunk.
