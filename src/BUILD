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

    defines = ["BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS", "BOOST_MPL_LIMIT_LIST_SIZE=50"],

    # link_opts = ['-framework cocoa', '-framework OpenGl'],

    srcs     = glob([ "**/*.cpp", "**/*.h"]),
)

