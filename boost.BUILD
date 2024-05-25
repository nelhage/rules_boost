load("@bazel_skylib//rules:copy_file.bzl", "copy_file")
load("@bazel_skylib//lib:selects.bzl", "selects")
load("@bazel_skylib//rules:common_settings.bzl", "bool_flag")
load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_library", "boost_so_library", "default_copts", "default_defines", "hdr_list")

_repo_dir = repository_name().removeprefix("@")

# Hopefully, the need for these OSxCPU config_setting()s will be obviated by a fix to https://github.com/bazelbuild/platforms/issues/36

config_setting(
    name = "linux_arm",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:arm",
    ],
)

config_setting(
    name = "linux_ppc",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:ppc",
    ],
)

config_setting(
    name = "linux_aarch64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "osx_arm64",
    constraint_values = [
        "@platforms//os:osx",
        "@platforms//cpu:aarch64",
    ],
)

config_setting(
    name = "osx_x86_64",
    constraint_values = [
        "@platforms//os:osx",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
)

config_setting(
    name = "android_arm",
    constraint_values = [
        "@platforms//os:android",
        "@platforms//cpu:arm",
    ],
)

config_setting(
    name = "android_aarch64",
    constraint_values = [
        "@platforms//os:android",
        "@platforms//cpu:aarch64",
    ],
)

# Rename .asm to .S so that it will be handled with the C preprocessor.
[copy_file(
    name = "rename_%s" % name,
    src = "libs/context/src/asm/%s_x86_64_ms_pe_masm.asm" % name,
    out = "libs/context/src/asm/%s_x86_64_ms_pe_masm.S" % name,
) for name in [
    "make",
    "jump",
    "ontop",
]]

BOOST_CTX_ASM_SOURCES = selects.with_or({
    (":linux_aarch64", ":android_aarch64"): [
        "libs/context/src/asm/jump_arm64_aapcs_elf_gas.S",
        "libs/context/src/asm/make_arm64_aapcs_elf_gas.S",
        "libs/context/src/asm/ontop_arm64_aapcs_elf_gas.S",
    ],
    (":linux_arm", ":android_arm"): [
        "libs/context/src/asm/jump_arm_aapcs_elf_gas.S",
        "libs/context/src/asm/make_arm_aapcs_elf_gas.S",
        "libs/context/src/asm/ontop_arm_aapcs_elf_gas.S",
    ],
    ":linux_ppc": [
        "libs/context/src/asm/jump_ppc64_sysv_elf_gas.S",
        "libs/context/src/asm/make_ppc64_sysv_elf_gas.S",
        "libs/context/src/asm/ontop_ppc64_sysv_elf_gas.S",
    ],
    ":linux_x86_64": [
        "libs/context/src/asm/jump_x86_64_sysv_elf_gas.S",
        "libs/context/src/asm/make_x86_64_sysv_elf_gas.S",
        "libs/context/src/asm/ontop_x86_64_sysv_elf_gas.S",
    ],
    ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ["apple_ctx_asm_sources"],
    ":windows_x86_64": [
        "libs/context/src/asm/make_x86_64_ms_pe_masm.S",
        "libs/context/src/asm/jump_x86_64_ms_pe_masm.S",
        "libs/context/src/asm/ontop_x86_64_ms_pe_masm.S",
    ],
    "//conditions:default": [],
})

filegroup(
    name = "apple_ctx_asm_sources",
    srcs = select({
        "@platforms//cpu:aarch64": [
            "libs/context/src/asm/jump_arm64_aapcs_macho_gas.S",
            "libs/context/src/asm/make_arm64_aapcs_macho_gas.S",
            "libs/context/src/asm/ontop_arm64_aapcs_macho_gas.S",
        ],
        "@platforms//cpu:arm": [
            "libs/context/src/asm/jump_arm_aapcs_macho_gas.S",
            "libs/context/src/asm/make_arm_aapcs_macho_gas.S",
            "libs/context/src/asm/ontop_arm_aapcs_macho_gas.S",
        ],
        "@platforms//cpu:x86_64": [
            "libs/context/src/asm/jump_x86_64_sysv_macho_gas.S",
            "libs/context/src/asm/make_x86_64_sysv_macho_gas.S",
            "libs/context/src/asm/ontop_x86_64_sysv_macho_gas.S",
        ],
    }),
)

boost_library(
    name = "context",
    srcs = BOOST_CTX_ASM_SOURCES + selects.with_or({
        ("@platforms//os:linux", "@platforms//os:android", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [
            "libs/context/src/posix/stack_traits.cpp",
        ],
        ":windows_x86_64": [
            "libs/context/src/windows/stack_traits.cpp",
        ],
        "//conditions:default": [],
    }),
    exclude_src = [
        "libs/context/src/fiber.cpp",
        "libs/context/src/untested.cpp",
        "libs/context/src/continuation.cpp",
    ],
    local_defines = select({
        ":windows_x86_64": ["BOOST_CONTEXT_EXPORT="],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":assert",
        ":config",
        ":cstdint",
        ":detail",
        ":intrusive_ptr",
        ":mp11",
        ":pool",
    ],
)

BOOST_FIBER_NUMA_SRCS = select({
    ":linux_arm": [
    ],
    ":linux_ppc": [
        "libs/fiber/src/numa/linux/pin_thread.cpp",
        "libs/fiber/src/numa/linux/topology.cpp",
    ],  # MAYBE SHOULD BE BLANK
    ":linux_x86_64": [
        "libs/fiber/src/numa/linux/pin_thread.cpp",
        "libs/fiber/src/numa/linux/topology.cpp",
    ],
    ":windows_x86_64": [
        "libs/fiber/src/numa/windows/pin_thread.cpp",
        "libs/fiber/src/numa/windows/topology.cpp",
    ],
    "//conditions:default": [],
})

boost_library(
    name = "fiber",
    srcs = BOOST_FIBER_NUMA_SRCS + glob(["libs/fiber/src/algo/**/*.cpp"]) + [
        "libs/fiber/examples/asio/detail/yield.hpp",
    ],
    hdrs = [
        "libs/fiber/examples/asio/round_robin.hpp",
        "libs/fiber/examples/asio/yield.hpp",
    ],
    copts = select({
        ":windows_x86_64": [
            "/D_WIN32_WINNT=0x0601",
        ],
        "//conditions:default": [],
    }),
    exclude_src = ["libs/fiber/src/numa/**/*.cpp"],
    linkopts = selects.with_or({
        ("@platforms//os:linux", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ["-lpthread"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":algorithm",
        ":assert",
        ":atomic",
        ":config",
        ":context",
        ":core",
        ":filesystem",
        ":format",
        ":intrusive",
        ":intrusive_ptr",
        ":pool",
        ":predef",
        ":static_assert",
        ":system",
        ":throw_exception",
    ],
)

boost_library(
    name = "pool",
    deps = [
        ":integer",
    ],
)

boost_library(
    name = "accumulators",
    deps = [
        ":circular_buffer",
        ":concept_check",
        ":config",
        ":fusion",
        ":mpl",
        ":numeric_ublas",
        ":parameter",
        ":serialization",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "algorithm",
    deps = [
        ":function",
        ":iterator",
        ":range",
    ],
)

boost_library(
    name = "align",
)

boost_library(
    name = "any",
    deps = [
        ":config",
        ":mpl",
        ":static_assert",
        ":throw_exception",
        ":type_index",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "assign",
    deps = [
        ":config",
        ":detail",
        ":mpl",
        ":preprocessor",
        ":ptr_container",
        ":range",
        ":static_assert",
        ":type_traits",
    ],
)

BOOST_ATOMIC_SSE_SRCS = glob(
    [
        "libs/atomic/src/*sse2.cpp",
        "libs/atomic/src/*sse41.cpp",
    ],
    allow_empty = True,
)

BOOST_ATOMIC_DEPS = [
    ":align",
    ":assert",
    ":config",
    ":cstdint",
    ":predef",
    ":type_traits",
    ":winapi",
    ":static_assert",
    ":preprocessor",
]

cc_library(
    name = "atomic_sse",
    srcs = BOOST_ATOMIC_SSE_SRCS + glob([
        "libs/atomic/src/*.hpp",
        "libs/atomic/include/boost/atomic/detail/*.hpp",
    ]),
    copts = default_copts + [
        "-msse2",
        "-msse4.1",
        "-Iexternal/%s/libs/atomic/src" % _repo_dir,
        "-Iexternal/%s/libs/atomic/include" % _repo_dir,
    ],
    defines = default_defines,
    linkstatic = select({
        ":windows_x86_64": True,
        "//conditions:default": False,
    }),
    visibility = ["//visibility:private"],
    deps = BOOST_ATOMIC_DEPS,
)

boost_library(
    name = "atomic",
    srcs = select({
        ":windows_x86_64": ["libs/atomic/src/wait_on_address.cpp"],
        "//conditions:default": [],
    }),
    copts = ["-Iexternal/%s/libs/atomic/src" % _repo_dir],
    exclude_src = ["libs/atomic/src/wait_on_address.cpp"] + BOOST_ATOMIC_SSE_SRCS,
    deps = BOOST_ATOMIC_DEPS + select({
        "@platforms//cpu:x86_64": [":atomic_sse"],
        "//conditions:default": [],
    }),
)

boost_library(
    name = "archive",
    deps = [
        ":assert",
        ":cstdint",
        ":integer",
        ":io",
        ":iterator",
        ":mpl",
        ":noncopyable",
        ":smart_ptr",
        ":spirit",
    ],
)

boost_library(
    name = "array",
    deps = [
        ":assert",
        ":concept_check",
        ":config",
        ":core",
        ":functional",
        ":swap",
        ":throw_exception",
    ],
)

bool_flag(
    name = "asio_disable_epoll",
    build_setting_default = False,
)

config_setting(
    name = "asio_no_epoll",
    flag_values = {
        ":asio_disable_epoll": "True",
    },
)

bool_flag(
    name = "asio_has_io_uring",
    build_setting_default = False,
)

config_setting(
    name = "asio_io_uring",
    flag_values = {
        ":asio_has_io_uring": "True",
    },
)

boost_library(
    name = "asio",
    srcs = [
        "@com_github_nelhage_rules_boost//src:build_asio.cpp",
    ],
    defines = [
        "BOOST_ASIO_SEPARATE_COMPILATION",
    ] + selects.with_or({
        ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [
            "BOOST_ASIO_DISABLE_STD_EXPERIMENTAL_STRING_VIEW",
        ],
        "//conditions:default": [],
    }) + select({
        ":asio_no_epoll": ["BOOST_ASIO_DISABLE_EPOLL"],
        "//conditions:default": [],
    }) + select({
        ":asio_io_uring": ["BOOST_ASIO_HAS_IO_URING"],
        "//conditions:default": [],
    }),
    linkopts = select({
        "@platforms//os:android": [],
        "//conditions:default": ["-lpthread"],
    }) + select({
        ":asio_io_uring": ["-luring"],
        "//conditions:default": [],
    }),
    deps = [
        ":assert",
        ":bind",
        ":cerrno",
        ":config",
        ":date_time",
        ":limits",
        ":regex",
        ":static_assert",
        ":system",
        ":throw_exception",
    ],
)

cc_library(
    name = "asio_ssl",
    srcs = [
        "@com_github_nelhage_rules_boost//src:build_asio_ssl.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":asio",
        "@boringssl//:ssl",
    ],
)

boost_library(
    name = "assert",
    deps = [
        ":current_function",
    ],
)

bool_flag(
    name = "beast_use_std_string_view",
    build_setting_default = False,
)

config_setting(
    name = "beast_std_string_view",
    flag_values = {
        ":beast_use_std_string_view": "True",
    },
)

boost_library(
    name = "beast",
    srcs = [
        "@com_github_nelhage_rules_boost//src:build_beast.cpp",
    ],
    defines = [
        "BOOST_BEAST_SEPARATE_COMPILATION",
    ] + select({
        ":beast_std_string_view": ["BOOST_BEAST_USE_STD_STRING_VIEW"],
        "//conditions:default": [],
    }),
    deps = [
        ":asio",
        ":assert",
        ":config",
        ":core",
        ":detail",
        ":endian",
        ":intrusive",
        ":is_placeholder",
        ":mp11",
        ":optional",
        ":preprocessor",
        ":shared_ptr",
        ":smart_ptr",
        ":static_assert",
        ":static_string",
        ":system",
        ":throw_exception",
        ":tribool",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "bimap",
    deps = [
        ":concept_check",
        ":functional",
        ":multi_index",
        ":serialization",
    ],
)

boost_library(
    name = "bind",
    deps = [
        ":get_pointer",
        ":is_placeholder",
        ":mem_fn",
        ":ref",
        ":type",
        ":visit_each",
    ],
)

boost_library(
    name = "callable_traits",
)

boost_library(
    name = "call_traits",
)

boost_library(
    name = "cerrno",
)

boost_library(
    name = "checked_delete",
)

boost_library(
    name = "chrono",
    deps = [
        ":assert",
        ":config",
        ":cstdint",
        ":detail",
        ":integer",
        ":mpl",
        ":operators",
        ":predef",
        ":ratio",
        ":system",
        ":throw_exception",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "circular_buffer",
    deps = [
        ":call_traits",
        ":concept_check",
        ":config",
        ":container",
        ":detail",
        ":iterator",
        ":limits",
        ":move",
        ":static_assert",
        ":throw_exception",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "compute",
    linkopts = selects.with_or({
        # OpenCL (required for Boost.Compute) is deprecated on macOS and not supported on other Apple OSs.
        # This will at least produce the right error message, indicating that OpenCL is not present
        ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [
            "-framework OpenCL",
        ],
        "//conditions:default": [
            "-lOpenCL",
        ],
    }),
    deps = [
        ":algorithm",
        ":chrono",
        ":config",
        ":throw_exception",
    ],
)

boost_library(
    name = "concept_archetype",
    deps = [
        ":config",
        ":iterator",
        ":mpl",
    ],
)

boost_library(
    name = "concept_check",
    deps = [
        ":concept",
        ":concept_archetype",
    ],
)

boost_library(
    name = "config",
)

boost_library(
    name = "concept",
)

BOOST_CONTAINER_INCLUDES_WITH_SRC_EXTENSION = [
    "libs/container/src/dlmalloc_ext_2_8_6.c",
    "libs/container/src/dlmalloc_2_8_6.c",
]

boost_library(
    name = "container",
    hdrs = BOOST_CONTAINER_INCLUDES_WITH_SRC_EXTENSION,
    exclude_src = BOOST_CONTAINER_INCLUDES_WITH_SRC_EXTENSION,
    deps = [
        ":config",
        ":core",
        ":intrusive",
        ":move",
        ":static_assert",
    ],
)

boost_library(
    name = "container_hash",
    defines = ["BOOST_NO_CXX98_FUNCTION_BASE"],
    deps = [
        ":assert",
        ":config",
        ":core",
        ":describe",
        ":integer",
        ":limits",
        ":type_traits",
    ],
)

boost_library(
    name = "conversion",
    hdrs = [
        "libs/lexical_cast/include/boost/lexical_cast.hpp",
        "libs/lexical_cast/include/boost/lexical_cast/bad_lexical_cast.hpp",
        "libs/numeric/conversion/include/boost/cast.hpp",
    ],
    deps = [
        ":assert",
        ":config",
        ":fusion",
        ":throw_exception",
    ],
)

boost_library(
    name = "core",
    deps = [
        ":config",
    ],
)

BOOST_CORO_SRCS = selects.with_or({
    ("@platforms//os:linux", "@platforms//os:android", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [
        "libs/coroutine/src/posix/stack_traits.cpp",
    ],
    ":windows_x86_64": [
        "libs/coroutine/src/windows/stack_traits.cpp",
    ],
})

boost_library(
    name = "coroutine",
    srcs = BOOST_CORO_SRCS + [
        "libs/coroutine/src/detail/coroutine_context.cpp",
    ],
    deps = [
        ":assert",
        ":config",
        ":context",
        ":core",
        ":move",
        ":range",
        ":static_assert",
        ":system",
        ":thread",
        ":type_traits",
    ],
)

boost_library(
    name = "coroutine2",
    deps = [
        ":assert",
        ":config",
        ":context",
        ":detail",
    ],
)

boost_library(
    name = "crc",
    deps = [
        ":array",
        ":config",
        ":cstdint",
        ":integer",
        ":limits",
    ],
)

boost_library(
    name = "cstdint",
    deps = [
        ":config",
        ":limits",
    ],
)

boost_library(
    name = "current_function",
)

boost_library(
    name = "date_time",
    deps = [
        ":algorithm",
        ":io",
        ":lexical_cast",
        ":mpl",
        ":operators",
        ":static_assert",
        ":tokenizer",
        ":type_traits",
    ],
)

boost_library(
    name = "detail",
    deps = [
        ":limits",
        ":winapi",
    ],
)

boost_library(
    name = "describe",
    deps = [
        ":mp11",
    ],
)

boost_library(
    name = "dll",
    deps = [
        ":filesystem",
    ],
)

boost_library(
    name = "dynamic_bitset",
    deps = [
        ":config",
        ":core",
        ":detail",
        ":functional",
        ":integer",
        ":move",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "enable_shared_from_this",
)

boost_library(
    name = "endian",
    deps = [
        ":config",
        ":core",
        ":cstdint",
        ":detail",
        ":predef",
        ":type_traits",
    ],
)

boost_library(
    name = "exception",
    deps = [
        ":assert",
        ":config",
        ":current_function",
        ":detail",
        ":smart_ptr",
        ":tuple",
        ":type_traits",
    ],
)

boost_library(
    name = "exception_ptr",
    deps = [
        ":config",
    ],
)

boost_library(
    name = "filesystem",
    defines = [
        "BOOST_FILESYSTEM_NO_CXX20_ATOMIC_REF",
    ] + select({
        "@platforms//os:linux": [
            "BOOST_FILESYSTEM_HAS_POSIX_AT_APIS",
        ],
        "//conditions:default": [],
    }),
    deps = [
        ":assert",
        ":atomic",
        ":config",
        ":core",
        ":cstdint",
        ":detail",
        ":functional",
        ":io",
        ":iterator",
        ":predef",
        ":range",
        ":scoped_array",
        ":smart_ptr",
        ":static_assert",
        ":system",
        ":throw_exception",
        ":type_traits",
    ],
)

boost_library(
    name = "flyweight",
    deps = [
        ":core",
        ":detail",
        ":interprocess",
        ":mpl",
        ":multi_index",
        ":parameter",
        ":preprocessor",
        ":serialization",
        ":smart_ptr",
        ":throw_exception",
        ":type_traits",
    ],
)

boost_library(
    name = "foreach",
    deps = [
        ":config",
        ":detail",
        ":iterator",
        ":mpl",
        ":noncopyable",
        ":range",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "format",
    deps = [
        ":assert",
        ":config",
        ":detail",
        ":limits",
        ":optional",
        ":smart_ptr",
        ":throw_exception",
        ":timer",
        ":utility",
    ],
)

boost_library(
    name = "function",
    deps = [
        ":bind",
        ":integer",
        ":type_index",
    ],
)

boost_library(
    name = "function_types",
)

boost_library(
    name = "functional",
    deps = [
        ":container_hash",
        ":detail",
        ":integer",
    ],
)

boost_library(
    name = "fusion",
    deps = [
        ":call_traits",
        ":config",
        ":core",
        ":detail",
        ":function_types",
        ":functional",
        ":get_pointer",
        ":mpl",
        ":preprocessor",
        ":ref",
        ":static_assert",
        ":tuple",
        ":type_traits",
        ":typeof",
        ":utility",
    ],
)

boost_library(
    name = "geometry",
    deps = [
        ":algorithm",
        ":call_traits",
        ":config",
        ":function_types",
        ":lexical_cast",
        ":math",
        ":mpl",
        ":multiprecision",
        ":numeric_conversion",
        ":qvm",
        ":range",
        ":rational",
        ":tokenizer",
        ":variant",
    ],
)

boost_library(
    name = "gil",
    deps = [
        ":concept_check",
        ":config",
        ":integer",
        ":iterator",
        ":mp11",
        ":mpl",
        ":ref",
        ":type_traits",
    ],
)

boost_library(
    name = "property_tree",
    deps = [
        ":any",
        ":bind",
        ":format",
        ":multi_index",
        ":optional",
        ":range",
        ":ref",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "python",
    exclude_src = ["**/fabscript"],
    deps = [
        "@python",
        ":config",
        ":function",
        ":align",
        ":shared_ptr",
        ":smart_ptr",
        ":numeric_conversion",
        ":implicit_cast",
        ":iterator",
        ":conversion",
    ],
)

boost_library(
    name = "get_pointer",
)

boost_library(
    name = "heap",
    deps = [
        ":array",
        ":intrusive",
        ":parameter",
    ],
)

boost_library(
    name = "icl",
    deps = [
        ":assert",
        ":concept_check",
        ":config",
        ":container",
        ":core",
        ":date_time",
        ":detail",
        ":iterator",
        ":move",
        ":mpl",
        ":range",
        ":rational",
        ":static_assert",
        ":type_traits",
        ":utility"
    ],
)

boost_library(
    name = "implicit_cast",
)

boost_library(
    name = "is_placeholder",
)

boost_library(
    name = "integer",
    deps = [
        ":cstdint",
        ":static_assert",
    ],
)

boost_library(
    name = "interprocess",
    linkopts = select({
        "@platforms//os:linux": [
            "-lpthread",
            "-lrt",
        ],
        "//conditions:default": [],
    }),
    deps = [
        ":assert",
        ":checked_delete",
        ":config",
        ":container",
        ":core",
        ":date_time",
        ":detail",
        ":integer",
        ":intrusive",
        ":limits",
        ":move",
        ":static_assert",
        ":type_traits",
        ":unordered",
        ":utility",
    ],
)

boost_library(
    name = "iterator",
    deps = [
        ":detail",
        ":static_assert",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "intrusive",
    deps = [
        ":assert",
        ":cstdint",
        ":noncopyable",
        ":static_assert",
    ],
)

alias(
    name = "intrusive_ptr",
    actual = ":smart_ptr",
    visibility = ["//visibility:public"],
)

boost_library(
    name = "io",
)

boost_library(
    name = "iostreams",
    deps = [
        ":assert",
        ":bind",
        ":call_traits",
        ":checked_delete",
        ":config",
        ":core",
        ":cstdint",
        ":detail",
        ":function",
        ":integer",
        ":mpl",
        ":noncopyable",
        ":numeric_conversion",
        ":preprocessor",
        ":random",
        ":range",
        ":ref",
        ":regex",
        ":shared_ptr",
        ":static_assert",
        ":throw_exception",
        ":type",
        ":type_traits",
        ":utility",
        "@bzip2//:bz2",
        "@xz//:lzma",
        "@zstd",
        "@zlib",
    ],
)

boost_library(
    name = "lexical_cast",
    deps = [
        ":array",
        ":chrono",
        ":config",
        ":container",
        ":detail",
        ":integer",
        ":limits",
        ":math",
        ":mpl",
        ":noncopyable",
        ":numeric_conversion",
        ":range",
        ":static_assert",
        ":throw_exception",
        ":type_traits",
    ],
)

boost_library(
    name = "limits",
)

BOOST_LOCALE_COMMON_SOURCES = glob(
    [
        "libs/locale/src/boost/locale/encoding/*.cpp",
        "libs/locale/src/boost/locale/encoding/*.hpp",
        "libs/locale/src/boost/locale/shared/*.cpp",
        "libs/locale/src/boost/locale/shared/*.hpp",
        "libs/locale/src/boost/locale/std/*.cpp",
        "libs/locale/src/boost/locale/std/*.hpp",
        "libs/locale/src/boost/locale/util/*.cpp",
        "libs/locale/src/boost/locale/util/*.hpp",
    ],
    exclude = [
        "libs/locale/src/boost/locale/util/iconv.hpp",
    ],
)

BOOST_LOCALE_STD_SOURCES = BOOST_LOCALE_COMMON_SOURCES + [
    "libs/locale/src/boost/locale/util/iconv.hpp",
]

BOOST_LOCALE_POSIX_SOURCES = BOOST_LOCALE_COMMON_SOURCES + [
    "libs/locale/src/boost/locale/util/iconv.hpp",
] + glob([
    "libs/locale/src/boost/locale/posix/*.cpp",
    "libs/locale/src/boost/locale/posix/*.hpp",
])

BOOST_LOCALE_WIN32_SOURCES = BOOST_LOCALE_COMMON_SOURCES + glob([
    "libs/locale/src/boost/locale/win32/*.cpp",
    "libs/locale/src/boost/locale/win32/*.hpp",
])

BOST_LOCALE_STD_COPTS = [
    "-DBOOST_LOCALE_NO_POSIX_BACKEND=1",
    "-DBOOST_LOCALE_NO_WINAPI_BACKEND",
    "-DBOOST_LOCALE_WITH_ICONV",
]

BOOST_LOCALE_POSIX_COPTS = [
    "-DBOOST_LOCALE_WITH_ICONV",
    "-DBOOST_LOCALE_NO_WINAPI_BACKEND",
]

BOOST_LOCALE_WIN32_COPTS = [
    "-DBOOST_LOCALE_NO_POSIX_BACKEND",
]

boost_library(
    name = "locale",
    srcs = selects.with_or({
        "@platforms//os:android": BOOST_LOCALE_STD_SOURCES,
        ("@platforms//os:linux", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): BOOST_LOCALE_POSIX_SOURCES,
        ":windows_x86_64": BOOST_LOCALE_WIN32_SOURCES,
    }),
    copts = selects.with_or({
        "@platforms//os:android": BOST_LOCALE_STD_COPTS,
        ("@platforms//os:linux", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): BOOST_LOCALE_POSIX_COPTS,
        ":windows_x86_64": BOOST_LOCALE_WIN32_COPTS,
    }),
    includes = ["libs/locale/src/"],
    deps = [
        ":assert",
        ":config",
        ":core",
        ":cstdint",
        ":function",
        ":iterator",
        ":predef",
        ":shared_ptr",
        ":smart_ptr",
        ":static_assert",
        ":thread",
        ":type_traits",
        ":unordered",
        ":utility",
    ],
    linkopts = selects.with_or({
        ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ["-liconv"],
        ("@platforms//os:android", "@platforms//os:linux", ":windows_x86_64"): [],
    }),
)

boost_library(
    name = "lockfree",
    deps = [
        ":align",
        ":array",
        ":assert",
        ":config",
        ":detail",
        ":iterator",
        ":noncopyable",
        ":parameter",
        ":predef",
        ":static_assert",
        ":tuple",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "math",
    srcs = [
        # See https://github.com/boostorg/math/blob/boost-1.56.0/doc/overview/roadmap.qbk#L11-L14
        "libs/math/include_private/boost/math/tools/remez.hpp",
        "libs/math/include_private/boost/math/constants/generate.hpp",
        "libs/math/include_private/boost/math/tools/solve.hpp",
        "libs/math/include_private/boost/math/tools/test.hpp",
    ],
    includes = ["libs/math/include_private"],
    deps = [
        ":array",
        ":assert",
        ":atomic",
        ":concept_check",
        ":config",
        ":core",
        ":cstdint",
        ":detail",
        ":fusion",
        ":integer",
        ":lambda",
        # https://github.com/boostorg/lexical_cast/issues/27
        #":lexical_cast",
        ":limits",
        ":mp11",
        ":mpl",
        # https://github.com/boostorg/math/issues/201
        #":multiprecision",
        ":predef",
        ":range",
        ":scoped_array",
        ":static_assert",
        ":throw_exception",
        ":tuple",
        ":type",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "mem_fn",
)

boost_library(
    name = "move",
    deps = [
        ":assert",
        ":detail",
        ":static_assert",
    ],
)

boost_library(
    name = "mp11",
    deps = [
        ":config",
        ":detail",
    ],
)

boost_library(
    name = "mpl",
    deps = [
        ":move",
        ":preprocessor",
    ],
)

boost_library(
    name = "hana",
)

boost_library(
    name = "msm",
    deps = [
        ":any",
        ":bind",
        ":detail",
        ":function",
        ":fusion",
        ":mpl",
        ":noncopyable",
        ":parameter",
        ":proto",
        ":serialization",
        ":type_traits",
    ],
)

boost_library(
    name = "multi_index",
    deps = [
        ":foreach",
        ":serialization",
        ":static_assert",
        ":tuple",
    ],
)

boost_library(
    name = "multiprecision",
    deps = [
        ":config",
        ":cstdint",
        ":lexical_cast",
        ":math",
        ":mpl",
        ":predef",
        ":rational",
        ":throw_exception",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "nowide",
    deps = [
        ":config",
        ":filesystem",
    ],
)

boost_library(
    name = "noncopyable",
    deps = [
        ":core",
    ],
)

boost_library(
    name = "none",
    hdrs = [
        "libs/optional/include/boost/none_t.hpp",
    ],
)

boost_library(
    name = "numeric_conversion",
    boost_name = "numeric/conversion",
    deps = [
        ":config",
        ":detail",
        ":integer",
        ":limits",
        ":mpl",
        ":throw_exception",
        ":type",
        ":type_traits",
    ],
)

boost_library(
    name = "numeric_ublas",
    boost_name = "numeric/ublas",
    deps = [
        ":concept_check",
        ":config",
        ":core",
        ":iterator",
        ":mpl",
        ":noncopyable",
        ":numeric_interval",
        ":range",
        ":serialization",
        ":shared_array",
        ":static_assert",
        ":timer",
        ":type_traits",
        ":typeof",
        ":utility",
    ],
)

boost_library(
    name = "numeric_odeint",
    boost_name = "numeric/odeint",
    linkopts = select({
        "@platforms//os:android": ["-lm"],
        "//conditions:default": [],
    }),
    deps = [
        ":fusion",
        ":lexical_cast",
        ":math",
        # There is currently a circular dependency between math and
        # multiprecision. We know that :numeric_odeint triggers it,
        # despite only depending directly on :math, so we include
        # :multiprecision here. Some users of :math will need to do
        # so as well.
        # See: https://github.com/boostorg/math/issues/201
        ":multiprecision",
        ":multi_array",
        ":numeric_ublas",
        ":serialization",
        ":units",
    ],
)

boost_library(
    name = "multi_array",
)

boost_library(
    name = "units",
    deps = [
        ":assert",
        ":config",
        ":core",
        ":integer",
        ":io",
        ":lambda",
        ":math",
        ":mpl",
        ":preprocessor",
        ":serialization",
        ":static_assert",
        ":type_traits",
        ":typeof",
        ":utility",
    ],
)

boost_library(
    name = "operators",
    deps = [
        ":config",
        ":core",
        ":detail",
    ],
)

boost_library(
    name = "optional",
    deps = [
        ":assert",
        ":config",
        ":detail",
        ":none",
        ":static_assert",
        ":throw_exception",
        ":type",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "outcome",
    deps = [
        ":config",
        ":exception",
        ":smart_ptr",
        ":system",
    ],
)

boost_library(
    name = "parameter",
    deps = [
        ":mp11",
    ],
)

boost_library(
    name = "polygon",
    deps = [
        ":config",
        ":integer",
        ":mpl",
        ":utility",
    ],
)

boost_library(
    name = "predef",
)

boost_library(
    name = "preprocessor",
)

boost_library(
    name = "process",
    deps = [
        ":algorithm",
        ":asio",
        ":config",
        ":filesystem",
        ":fusion",
        ":system",
        ":winapi",
    ],
)

boost_library(
    name = "program_options",
    deps = [
        ":any",
        ":bind",
        ":config",
        ":detail",
        ":function",
        ":iterator",
        ":lexical_cast",
        ":limits",
        ":noncopyable",
        ":shared_ptr",
        ":static_assert",
        ":throw_exception",
        ":tokenizer",
        ":type_traits",
    ],
)

boost_library(
    name = "ptr_container",
    deps = [
        ":assert",
        ":checked_delete",
        ":circular_buffer",
        ":config",
        ":iterator",
        ":mpl",
        ":range",
        ":serialization",
        ":static_assert",
        ":type_traits",
        ":unordered",
        ":utility",
    ],
)

boost_library(
    name = "qvm",
    linkopts = select({
        "@platforms//os:android": ["-lm"],
        "//conditions:default": [],
    }),
    deps = [
        ":assert",
        ":core",
        ":exception",
        ":static_assert",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "random",
    linkopts = select({
        "@platforms//os:windows": ["-defaultlib:bcrypt.lib"],
        "//conditions:default": [],
    }),
    deps = [
        ":assert",
        ":config",
        ":detail",
        ":foreach",
        ":integer",
        ":lexical_cast",
        ":limits",
        ":math",
        ":mpl",
        ":noncopyable",
        ":operators",
        ":range",
        ":regex",
        ":shared_ptr",
        ":static_assert",
        ":system",
        ":throw_exception",
        ":timer",
        ":tuple",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "range",
    deps = [
        ":array",
        ":assert",
        ":concept_check",
        ":config",
        ":detail",
        ":functional",
        ":integer",
        ":iterator",
        ":mpl",
        ":noncopyable",
        ":optional",
        ":preprocessor",
        ":ref",
        ":regex",
        ":static_assert",
        ":tuple",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "ratio",
    deps = [
        ":integer",
    ],
)

boost_library(
    name = "rational",
    deps = [
        ":assert",
        ":call_traits",
        ":config",
        ":detail",
        ":integer",
        ":operators",
        ":static_assert",
        ":throw_exception",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "ref",
    deps = [
        ":config",
        ":core",
        ":detail",
        ":utility",
    ],
)

boost_library(
    name = "regex",
    deps = [
        ":assert",
        ":config",
        ":core",
        ":cstdint",
        ":detail",
        ":exception",
        ":functional",
        ":integer",
        ":limits",
        ":mpl",
        ":predef",
        ":ref",
        ":smart_ptr",
        ":throw_exception",
        ":type_traits",
    ],
)

boost_library(
    name = "safe_numerics",
    deps = [
        ":concept_check",
        ":config",
        ":core",
        ":integer",
        ":mp11",
        ":tribool",
    ],
)

boost_library(
    name = "scope_exit",
    deps = [
        ":config",
        ":detail",
        ":function",
        ":mpl",
        ":preprocessor",
        ":type_traits",
        ":typeof",
        ":utility",
    ],
)

boost_library(
    name = "scoped_array",
    deps = [
        ":checked_delete",
    ],
)

boost_library(
    name = "scoped_ptr",
    deps = [
        ":checked_delete",
    ],
)

boost_library(
    name = "shared_ptr",
    deps = [
        ":checked_delete",
    ],
)

boost_library(
    name = "shared_array",
    deps = [
        ":checked_delete",
    ],
)

boost_library(
    name = "signals2",
    deps = [
        ":assert",
        ":bind",
        ":checked_delete",
        ":config",
        ":core",
        ":detail",
        ":function",
        ":iterator",
        ":mpl",
        ":multi_index",
        ":noncopyable",
        ":optional",
        ":parameter",
        ":predef",
        ":preprocessor",
        ":ref",
        ":scoped_ptr",
        ":shared_ptr",
        ":smart_ptr",
        ":swap",
        ":throw_exception",
        ":tuple",
        ":type_traits",
        ":utility",
        ":variant",
        ":visit_each",
    ],
)

boost_library(
    name = "serialization",
    deps = [
        ":archive",
        ":array",
        ":assert",
        ":call_traits",
        ":config",
        ":core",
        ":cstdint",
        ":detail",
        ":function",
        ":integer",
        ":io",
        ":limits",
        ":mpl",
        ":noncopyable",
        ":operators",
        ":preprocessor",
        ":spirit",
        ":static_assert",
        ":type_traits",
    ],
)

boost_library(
    name = "smart_ptr",
    deps = [
        ":align",
        ":core",
        ":predef",
        ":scoped_array",
        ":scoped_ptr",
        ":shared_array",
        ":shared_ptr",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "spirit",
    deps = [
        ":endian",
        ":foreach",
        ":function",
        ":iostreams",
        ":optional",
        ":phoenix",
        ":range",
        ":ref",
        ":tti",
        ":utility",
        ":variant",
    ],
)

BOOST_STACKTRACE_SOURCES = selects.with_or({
    "@platforms//os:android": [
        "libs/stacktrace/src/basic.cpp",
        #"libs/stacktrace/src/noop.cpp",
    ],
    ":linux_arm": [
        "libs/stacktrace/src/basic.cpp",
        "libs/stacktrace/src/noop.cpp",
    ],
    ":linux_aarch64": [
        "libs/stacktrace/src/basic.cpp",
        # "libs/stacktrace/src/noop.cpp",
    ],
    ":linux_ppc": [
        "libs/stacktrace/src/backtrace.cpp",
    ],
    ":linux_x86_64": [
        "libs/stacktrace/src/backtrace.cpp",
    ],
    ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [
        "libs/stacktrace/src/addr2line.cpp",
    ],
    ":windows_x86_64": [
        "libs/stacktrace/src/windbg.cpp",
        "libs/stacktrace/src/windbg_cached.cpp",
    ],
    "//conditions:default": [],
})

boost_library(
    name = "stacktrace",
    srcs = BOOST_STACKTRACE_SOURCES,
    defines = selects.with_or({
        ("@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [
            "BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED",
        ],
        "//conditions:default": [],
    }),
    exclude_src = ["libs/stacktrace/src/*.cpp"],
    linkopts = select({
        ":linux_ppc": [
            "-lbacktrace -ldl",
        ],
        ":linux_x86_64": [
            "-lbacktrace -ldl",
        ],
        ":linux_aarch64": [
            "-lbacktrace -ldl",
        ],
        "//conditions:default": [],
    }),
    deps = [
        ":array",
        ":config",
        ":core",
        ":detail",
        ":lexical_cast",
        ":predef",
        ":static_assert",
        ":type_traits",
    ],
)

boost_library(
    name = "static_assert",
    hdrs = [
        "libs/config/include/boost/config/workaround.hpp",
    ],
)

boost_library(
    name = "static_string",
    deps = [
        ":container_hash",
        ":static_assert",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "stl_interfaces",
    deps = [
        ":assert",
        ":config",
    ],
)

boost_library(
    name = "system",
    deps = [
        ":assert",
        ":cerrno",
        ":config",
        ":core",
        ":cstdint",
        ":noncopyable",
        ":predef",
        ":utility",
        ":variant2",
        ":winapi",
    ],
)

boost_library(
    name = "swap",
    deps = [
        ":core",
    ],
)

boost_library(
    name = "throw_exception",
    deps = [
        ":assert",
        ":config",
        ":cstdint",
        ":current_function",
        ":detail",
    ],
)

boost_library(
    name = "thread",
    srcs = selects.with_or({
        ("@platforms//os:linux", "@platforms//os:android", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [
            "libs/thread/src/pthread/once.cpp",
            "libs/thread/src/pthread/thread.cpp",
        ],
        ":windows_x86_64": [
            "libs/thread/src/win32/thread.cpp",
            "libs/thread/src/win32/tss_dll.cpp",
            "libs/thread/src/win32/tss_pe.cpp",
        ],
    }),
    hdrs = selects.with_or({
        ("@platforms//os:linux", "@platforms//os:android", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [
            "libs/thread/src/pthread/once_atomic.cpp",
        ],
        ":windows_x86_64": [],
    }),
    linkopts = selects.with_or({
        ("@platforms//os:linux", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): ["-lpthread"],
        ":windows_x86_64": [],
        "@platforms//os:android": [],
    }),
    local_defines = selects.with_or({
        ("@platforms//os:linux", "@platforms//os:osx", "@platforms//os:ios", "@platforms//os:watchos", "@platforms//os:tvos"): [],
        ":windows_x86_64": [
            "BOOST_ALL_NO_LIB",
            "BOOST_THREAD_BUILD_LIB",
            "BOOST_THREAD_USE_LIB",
            "BOOST_WIN32_THREAD",
        ],
        "//conditions:default": [],
    }),
    deps = [
        ":algorithm",
        ":assert",
        ":atomic",
        ":bind",
        ":chrono",
        ":config",
        ":core",
        ":cstdint",
        ":date_time",
        ":detail",
        ":enable_shared_from_this",
        ":exception",
        ":function",
        ":functional",
        ":io",
        ":iterator",
        ":lexical_cast",
        ":move",
        ":optional",
        ":predef",
        ":preprocessor",
        ":scoped_array",
        ":shared_ptr",
        ":smart_ptr",
        ":static_assert",
        ":system",
        ":throw_exception",
        ":tuple",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "tokenizer",
    deps = [
        ":assert",
        ":config",
        ":detail",
        ":iterator",
        ":mpl",
        ":throw_exception",
    ],
)

boost_library(
    name = "tribool",
    boost_name = "logic",
    exclude_hdr = [
        "libs/logic/include/boost/logic/tribool_io.hpp",
    ],
    deps = [
        ":config",
        ":detail",
    ],
)

boost_library(
    name = "tti",
    deps = [
        ":config",
        ":function_types",
        ":mpl",
        ":preprocessor",
        ":type_traits",
    ],
)

boost_library(
    name = "type",
    deps = [
        ":core",
    ],
)

boost_library(
    name = "type_index",
    deps = [
        ":container_hash",
        ":core",
        ":functional",
        ":throw_exception",
    ],
)

boost_library(
    name = "type_traits",
    deps = [
        ":config",
        ":core",
        ":mpl",
        ":static_assert",
    ],
)

boost_library(
    name = "typeof",
    deps = [
        ":config",
        ":detail",
        ":mpl",
        ":preprocessor",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "tuple",
    deps = [
        ":ref",
    ],
)

boost_library(
    name = "unordered",
    deps = [
        ":assert",
        ":config",
        ":container",
        ":detail",
        ":functional",
        ":iterator",
        ":limits",
        ":move",
        ":mp11",
        ":preprocessor",
        ":smart_ptr",
        ":swap",
        ":throw_exception",
        ":tuple",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "utility",
    deps = [
        ":config",
        ":container_hash",
        ":core",
        ":cstdint",
        ":detail",
        ":io",
        ":preprocessor",
        ":static_assert",
        ":swap",
        ":throw_exception",
        ":type_traits",
    ],
)

boost_library(
    name = "uuid",
    deps = [
        ":assert",
        ":config",
        ":core",
        ":detail",
        ":io",
        ":random",
        ":serialization",
        ":static_assert",
        ":throw_exception",
        ":tti",
        ":type_traits",
    ],
)

boost_library(
    name = "variant",
    deps = [
        ":call_traits",
        ":config",
        ":detail",
        ":functional",
        ":math",
        ":static_assert",
        ":type_index",
        ":type_traits",
        ":utility",
    ],
)

boost_library(
    name = "variant2",
    deps = [
        ":assert",
        ":cstdint",
        ":mp11",
    ],
)

boost_library(
    name = "visit_each",
)

boost_library(
    name = "vmd",
    deps = [
        ":preprocessor",
    ],
)

boost_library(
    name = "timer",
    deps = [
        ":cerrno",
        ":chrono",
        ":config",
        ":cstdint",
        ":io",
        ":limits",
        ":predef",
        ":system",
        ":throw_exception",
    ],
)

boost_library(
    name = "numeric_interval",
    boost_name = "numeric/interval",
)

_BOOST_TEST_DEPS = [
    ":algorithm",
    ":assert",
    ":bind",
    ":call_traits",
    ":config",
    ":core",
    ":cstdint",
    ":current_function",
    ":detail",
    ":exception",
    ":function",
    ":io",
    ":iterator",
    ":limits",
    ":mpl",
    ":numeric_conversion",
    ":optional",
    ":preprocessor",
    ":scoped_array",
    ":scoped_ptr",
    ":shared_ptr",
    ":smart_ptr",
    ":static_assert",
    ":timer",
    ":type",
    ":type_traits",
    ":utility",
]

# An uncompiled Boost.Test library, to be used through boost/test/included/*.
boost_library(
    name = "test",
    exclude_src = glob([
        "libs/test/src/*.cpp",
    ]),
    deps = _BOOST_TEST_DEPS,
)

# A statically linked Boost.Test library, used through boost/test/* and without
# defining BOOST_TEST_DYN_LINK.
boost_library(
    name = "test.a",
    boost_name = "test",
    exclude_hdr = glob([
        "libs/test/include/boost/test/included/*.hpp",
    ]),
    exclude_src = [
        "libs/test/src/test_main.cpp",
        "libs/test/src/cpp_main.cpp",
    ],
    linkstatic = True,
    deps = _BOOST_TEST_DEPS,
)

# A dynamically linked Boost.Test library, used through boost/test/*.  Bazel
# will add -DBOOST_TEST_DYN_LINK for you if you depend on this library.
boost_so_library(
    name = "test.so",
    boost_name = "test",
    defines = ["BOOST_TEST_DYN_LINK"],
    exclude_hdr = glob([
        "libs/test/include/boost/test/included/*.hpp",
    ]),
    exclude_src = [
        "libs/test/src/test_main.cpp",
        "libs/test/src/cpp_main.cpp",
    ],
    deps = _BOOST_TEST_DEPS,
)

boost_library(
    name = "proto",
    deps = [
        ":fusion",
    ],
)

boost_library(
    name = "phoenix",
    deps = [
        ":bind",
        ":proto",
    ],
)

BOOST_LOG_CFLAGS = select({
    "@platforms//cpu:x86_64": [
        "-msse4.2",
    ],
    "//conditions:default": [
    ],
})

BOOST_LOG_DEFINES = [
    "BOOST_LOG_WITHOUT_WCHAR_T",
    "BOOST_LOG_USE_STD_REGEX",
    "BOOST_LOG_WITHOUT_DEBUG_OUTPUT",
    "BOOST_LOG_WITHOUT_EVENT_LOG",
    "BOOST_LOG_WITHOUT_SYSLOG",
]

BOOST_LOG_DEPS = [
    ":align",
    ":array",
    ":asio",
    ":assert",
    ":atomic",
    ":bind",
    ":config",
    ":core",
    ":cstdint",
    ":current_function",
    ":date_time",
    ":detail",
    ":exception",
    ":filesystem",
    ":fusion",
    ":interprocess",
    ":intrusive",
    ":io",
    ":iterator",
    ":lexical_cast",
    ":limits",
    ":move",
    ":mpl",
    ":none",
    ":optional",
    ":parameter",
    ":phoenix",
    ":predef",
    ":preprocessor",
    ":property_tree",
    ":proto",
    ":random",
    ":range",
    ":ref",
    ":smart_ptr",
    ":spirit",
    ":static_assert",
    ":system",
    ":thread",
    ":throw_exception",
    ":type_index",
    ":type_traits",
    ":utility",
    ":variant",
]

BOOST_LOG_SSSE3_DEP = select({
    "@platforms//cpu:x86_64": [
        ":log_dump_ssse3",
    ],
    "//conditions:default": [
    ],
})

cc_library(
    name = "log_dump_ssse3",
    srcs = ["libs/log/src/dump_ssse3.cpp"] + hdr_list("log"),
    copts = default_copts + [
        "-msse4.2",
        "-Iexternal/%s/libs/log/include" % _repo_dir,
    ],
    defines = default_defines,
    licenses = ["notice"],
    local_defines = BOOST_LOG_DEFINES,
    visibility = ["//visibility:public"],
    deps = BOOST_LOG_DEPS,
)

boost_library(
    name = "log",
    srcs = glob([
        "libs/log/src/setup/*.hpp",
        "libs/log/src/setup/*.cpp",
    ]) + ["libs/locale/include/boost/locale/utf.hpp"],
    copts = BOOST_LOG_CFLAGS + [
        "-Iexternal/%s/libs/log/src/" % _repo_dir,
    ],
    exclude_src = [
        "libs/log/src/dump_avx2.cpp",
        "libs/log/src/dump_ssse3.cpp",
    ],
    local_defines = BOOST_LOG_DEFINES,
    deps = BOOST_LOG_DEPS + BOOST_LOG_SSSE3_DEP,
)

boost_library(
    name = "statechart",
)

boost_library(
    name = "winapi",
    visibility = ["//visibility:private"],
)

boost_library(
    name = "xpressive",
    deps = [
        ":config",
        ":core",
        ":type_traits",
    ],
)

boost_library(
    name = "property_map",
    deps = [
        ":config",
        ":core",
        ":function",
        ":lexical_cast",
        ":type_traits",
    ],
)

boost_library(
    name = "graph",
    deps = [
        ":algorithm",
        ":any",
        ":assert",
        ":concept",
        ":concept_check",
        ":config",
        ":conversion",
        ":core",
        ":foreach",
        ":function",
        ":functional",
        ":fusion",
        ":integer",
        ":intrusive_ptr",
        ":iterator",
        ":lexical_cast",
        ":limits",
        ":mpl",
        ":multi_index",
        ":operators",
        ":optional",
        ":parameter",
        ":property_map",
        ":property_tree",
        ":proto",
        ":range",
        ":ref",
        ":regex",
        ":scoped_ptr",
        ":spirit",
        ":static_assert",
        ":throw_exception",
        ":tti",
        ":tuple",
        ":type_traits",
        ":typeof",
        ":unordered",
        ":utility",
        ":xpressive",
    ],
)

boost_library(
    name = "lambda",
    deps = [
        ":is_placeholder",
    ],
)

boost_library(
    name = "lambda2",
)

boost_library(
    name = "sort",
)

boost_library(
    name = "contract",
    deps = [
        ":any",
        ":assert",
        ":config",
        ":detail",
        ":exception",
        ":function",
        ":function_types",
        ":mpl",
        ":noncopyable",
        ":optional",
        ":preprocessor",
        ":shared_ptr",
        ":smart_ptr",
        ":static_assert",
        ":thread",
        ":type_traits",
        ":typeof",
        ":utility",
    ],
)

boost_library(
    name = "json",
    deps = [
        ":align",
        ":assert",
        ":config",
        ":container",
        ":core",
        ":describe",
        ":exception",
        ":mp11",
        ":shared_ptr",
        ":smart_ptr",
        ":system",
        ":throw_exception",
        ":utility",
    ],
)

boost_library(
    name = "leaf",
)

boost_library(
    name = "pfr",
)

boost_library(
    name = "histogram",
    deps = [
        ":config",
        ":core",
        ":mp11",
        ":throw_exception",
        ":variant2",
    ],
)

boost_library(
    name = "poly_collection",
    deps = [
        ":assert",
        ":config",
        ":core",
        ":iterator",
        ":mp11",
        ":type_erasure",
        ":type_traits",
    ],
)

boost_library(
    name = "type_erasure",
    deps = [
        ":assert",
        ":config",
        ":fusion",
        ":iterator",
        ":mp11",
        ":mpl",
        ":preprocessor",
        ":shared_ptr",
        ":static_assert",
        ":thread",
        ":throw_exception",
        ":type_traits",
        ":typeof",
        ":utility",
        ":vmd",
    ],
)

boost_library(
    name = "url",
    srcs = glob([
        "libs/url/src/detail/**/*.cpp",
        "libs/url/src/grammar/**/*.cpp",
        "libs/url/src/rfc/**/*.cpp",
    ]),
    deps = [
        ":align",
        ":assert",
        ":config",
        ":core",
        ":mp11",
        ":optional",
        ":static_assert",
        ":system",
        ":throw_exception",
        ":type_traits",
        ":variant2",
    ],
)
