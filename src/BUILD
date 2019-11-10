package(default_visibility = ["//visibility:public"])

cc_binary(
    name     = "glisp",

    deps = [
            "@boost//:spirit",
            "@boost//:system"
            ],

    srcs     = glob([ "*.cpp", "**/*.h"]),

    copts    = ["-Werror", "-Wno-unused-variable"],

)

