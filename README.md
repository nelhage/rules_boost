# `rules_boost` -- Bazel build rules for [Boost](https://www.boost.org)

Copy this into your Bazel `MODULE.bazel` file to add this repo as an external dependency, making sure to update to the [latest commit](https://github.com/nelhage/rules_boost/commits/master) per the instructions below.

```Starlark

# Boost
# Famous C++ library that has given rise to many new additions to the C++ Standard Library
# Makes @boost available for use: For example, add `@boost//:algorithm` to your deps.
# For more, see https://github.com/nelhage/rules_boost and https://www.boost.org
bazel_dep(name = "rules_boost", repo_name = "com_github_nelhage_rules_boost")
archive_override(
    module_name = "rules_boost",
    urls = "https://github.com/nelhage/rules_boost/archive/refs/heads/master.tar.gz",
    strip_prefix = "rules_boost-master",
    # It is recommended to edit the above URL and the below sha256 to point to a specific version of this repository.
    # integrity = "sha256-...",
)

non_module_boost_repositories = use_extension("@com_github_nelhage_rules_boost//:boost/repositories.bzl", "non_module_dependencies")
use_repo(
    non_module_boost_repositories,
    "boost",
)
```

You can now use libraries in `deps` through the `@boost` repository, for example `@boost//:algorithm`.

## If you're using the Apple or Android-specific rules...

As with all platform-dependent C/C++ in Bazel, you'll need to use Bazel 7+ (and a similarly recent version of rules_apple) for per-platform configuration to work automatically out of the box. (Please file a PR to delete this section if Bazel 7 is now so old as to be standard.)

## Suggestion: Updates

Improvements come frequently to the underlying libraries, including security patches, so we'd recommend keeping up-to-date.

We'd strongly recommend you set up [Renovate](https://github.com/renovatebot/renovate) (or similar) at some point to keep this dependency (and others) up-to-date by default. [We aren't affiliated with Renovate or anything, but we think it's awesome. It watches for new versions and sends you PRs for review or automated testing. It's free and easy to set up. It's been astoundingly useful in our codebase, and we've worked with the wonderful maintainer to make things great for Bazel use. And it's used in official Bazel repositories--and this one!]

If not now, maybe come back to this step later, or watch this repo for updates. [Or hey, maybe give us a quick star, while you're thinking about watching.] Like Abseil, we live at head; the latest commit to the main branch is the commit you want. So don't rely on release notifications; use [Renovate](https://github.com/renovatebot/renovate) or poll manually for new commits.

## Configuration of Sub-Libraries

### ASIO

#### SSL Support

These rules implement support for Boost ASIO's SSL support. To use
ASIO-SSL, you must depend on the `"@boost//:asio_ssl"` target, instead
of `"@boost//:asio"`. ASIO-SSL depends on OpenSSL; By default,
`rules_boost` will download and build the latest
[BoringSSL](https://boringssl.googlesource.com/boringssl/) commit; To
use a different OpenSSL implementation, create a remote named
`openssl` before calling `boost_deps`. This remote must make available
OpenSSL's libssl at `@boringssl//:ssl`.

#### io\_uring support

To enable io\_uring for asio, use `--@boost//:asio_has_io_uring`.
Optionally, pass`--@boost//:asio_disable_epoll` to use io\_uring
for all operations.

### Beast

Boost Beast uses `beast::string_view` for things like request/response headers,
which by default is a type alias of `boost::string_view`. If you're using a
C++17 compiler that supports `std::string_view` you can use
`--@boost//:beast_use_std_string_view` to make `beast::string_view` instead be a
type alias of `std::string_view`.

---

Based in part on rules from https://github.com/mzhaom/trunk.
