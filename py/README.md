== Overview

A simple workspace rule to configure an external repository that contains
targets for the host's installation of python and numpy headers. The
implementation is currently quite naive and does not integrate at all with
Bazel's python toolchain support.

Your mileage may vary.

This implementation comes from TensorFlow:
https://github.com/tensorflow/tensorflow/tree/master/third_party/py.

== Usage

If you want to use Boost.Python (`@boost//:python`), you need to declare an
external repository `@local_config_python` that declares the `cc_library`s
`:python_headers` and `:numpy_headers`. On windows, it also needs to declare a
`cc_library` of `:python_lib`. This repository rule will attempt to declare
such a repository for you, using the `python` binary it finds on your `PATH`.

In your `WORKSPACE`, add:

```
load("@com_github_nelhage_rules_boost//py:python_configure.bzl", "python_configure")

python_configure(name = "local_config_python")
```

The detection can use a different `python` binary by setting `PYTHON_BIN_PATH`
in your environment.
