package(default_visibility = ["//visibility:public"])

cc_binary(
    name     = "glisp",

    srcs     = glob([ "*.cpp", "**/*.h"]),

    includes = ["."],

    linkopts = ["-lm"],
    copts    = ["-Werror", "-Wno-unused-variable"],

    deps = ["@immer//:immer"]
)

