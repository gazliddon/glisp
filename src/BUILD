package(default_visibility = ["//visibility:public"])

cc_binary(
    name     = "glisp",

    deps = [
            "@boost//:spirit",
            "@boost//:optional",
            "@boost//:system",
            "@immer//:immer",
            "@spdlog//:spdlog"
            ],

    # link_opts = ['-framework cocoa', '-framework OpenGl'],

    srcs     = glob([ "*.cpp", "**/*.h"]),
)

