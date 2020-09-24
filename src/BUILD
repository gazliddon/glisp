package(default_visibility = ["//visibility:public"])

cc_binary(
    name     = "glisp",

    deps = [
            "@boost//:spirit",
            "@boost//:system",
            "@immer//:immer",
            ],

    # link_opts = ['-framework cocoa', '-framework OpenGl'],

    srcs     = glob([ "*.cpp", "**/*.h"]),
)

