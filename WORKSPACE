load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "immer",
    url = "https://github.com/arximboldi/immer/archive/800ddb04e528a3e83e69e8021d7e872e7c34cbcd.zip",
    sha256 = "c5e382c7fa2fef1d7766be293ccbb4ecefb1cb2f9372d1b15c3e4893470ee370",
    strip_prefix = "immer-800ddb04e528a3e83e69e8021d7e872e7c34cbcd",
    build_file = "@//:third_party/immer.BUILD",
)

http_archive(
        name = "spdlog",
        url = "https://github.com/gabime/spdlog/archive/v1.7.0.zip",
        sha256 = "c8f1e1103e0b148eb8832275d8e68036f2fdd3975a1199af0e844908c56f6ea5",
        build_file = "@//:third_party/spdlog.BUILD",
        strip_prefix = "spdlog-1.7.0",
        )





load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "1e3a69bf2d5cd10c34b74f066054cd335d033d71",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1591047380 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()
