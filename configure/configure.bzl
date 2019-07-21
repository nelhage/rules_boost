load("@bazel_tools//tools/cpp:toolchain_utils.bzl", "find_cpp_toolchain")
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "CPP_COMPILE_ACTION_NAME", "C_COMPILE_ACTION_NAME")

def _configure_impl(ctx):
    inputs = ctx.files.data
    outputs = ctx.outputs.configs

    cc_toolchain = find_cpp_toolchain(ctx)

    feature_configuration = cc_common.configure_features(
        ctx = ctx,
        cc_toolchain = cc_toolchain,
        unsupported_features = ctx.disabled_features,
    )
    compiler = cc_common.get_tool_for_action(
        feature_configuration = feature_configuration,
        action_name = C_COMPILE_ACTION_NAME,
    )

    compile_variables = cc_common.create_compile_variables(
        feature_configuration = feature_configuration,
        cc_toolchain = cc_toolchain,
    )

    compiler_options = cc_common.get_memory_inefficient_command_line(
        feature_configuration = feature_configuration,
        action_name = C_COMPILE_ACTION_NAME,
        variables = compile_variables,
    )

    compiler_env = cc_common.get_environment_variables(
        feature_configuration = feature_configuration,
        action_name = C_COMPILE_ACTION_NAME,
        variables = compile_variables,
    )

    args = ctx.actions.args()
    args.add_all(compiler_options)
    print(compiler_env)

    ctx.actions.run(
        inputs = depset(
            items = inputs,
            transitive = [cc_toolchain.all_files],
        ),
        outputs = outputs,
        tools = [ctx.executable.configurator],
        executable = ctx.executable.configurator,
        progress_message = "Configure target",
        env = {"CC": compiler, "CXX": compiler},
    )

    return DefaultInfo(files = depset(inputs + outputs))

_configure = rule(
    implementation = _configure_impl,
    attrs = {
        "data": attr.label_list(allow_files = True),
        "configurator": attr.label(executable = True, cfg = "target", allow_single_file = True, mandatory = True),
        "configs": attr.output_list(),
        "_cc_toolchain": attr.label(default = Label("@bazel_tools//tools/cpp:current_cc_toolchain")),
    },
    fragments = ["cpp"],
    toolchains = ["@bazel_tools//tools/cpp:toolchain_type"],
    output_to_genfiles = True,
)

def cc_configurable_lib(
        name,
        srcs,
        hdrs,
        data,
        configurator,
        config_outputs,
        deps,
        **kvargs):
    _configure(
        name = "{}_configure".format(name),
        data = data + srcs + hdrs,
        configurator = configurator,
        configs = config_outputs,
    )

    native.cc_library(
        name = name,
        srcs = srcs,
        hdrs = hdrs + config_outputs,
        deps = deps,
        **kvargs
    )
