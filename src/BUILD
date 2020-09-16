package(default_visibility = ["//visibility:public"])

cc_binary(
    name     = "glisp",

    deps = [
            "@boost//:spirit",
            "@boost//:system"
            ],

    # link_opts = ['-framework cocoa', '-framework OpenGl'],

    srcs     = glob([ "*.cpp", "**/*.h"]),
)

