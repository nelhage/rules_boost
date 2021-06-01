# `rules_boost` -- Bazel build rules for Boost

To use these rules, add the following to your `WORKSPACE` file:

```bazel
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

_RULES_BOOST_COMMIT = "652b21e35e4eeed5579e696da0facbe8dba52b1f"

http_archive(
    name = "com_github_nelhage_rules_boost",
    sha256 = "c1b8b2adc3b4201683cf94dda7eef3fc0f4f4c0ea5caa3ed3feffe07e1fb5b15",
    strip_prefix = "rules_boost-%s" % _RULES_BOOST_COMMIT,
    urls = [
        "https://github.com/nelhage/rules_boost/archive/%s.tar.gz" % _RULES_BOOST_COMMIT,
    ],
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()
```

You can then use libraries in `deps` through the `@boost` repository, for
example `@boost//:algorithm`.


Based in part on rules from https://github.com/mzhaom/trunk.

## ASIO

### SSL Support

These rules implement support for Boost ASIO's SSL support. To use
ASIO-SSL, you must depend on the `"@boost//:asio_ssl"` target, instead
of `"@boost//:asio"`. ASIO-SSL depends on OpenSSL; By default,
`rules_boost` will download and build a recent
[BoringSSL](https://boringssl.googlesource.com/boringssl/) commit; To
use a different OpenSSL implementation, create a remote named
`openssl` before calling `boost_deps`. This remote must make available
OpenSSL's libssl at `@openssl//:ssl`.

### io\_uring support

To enable io\_uring for asio, use `--@boost//:asio_has_io_uring`.
Optionally, pass`--@boost//:asio_disable_epoll` to use io\_uring
for all operations.

## Beast

Boost Beast uses `beast::string_view` for things like request/response headers,
which by default is a type alias of `boost::string_view`. If you're using a
C++17 compiler that supports `std::string_view` you can use
`--@boost//:beast_use_std_string_view` to make `beast::string_view` instead be a
type alias of `std::string_view`.
