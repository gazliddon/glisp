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

http_archive(
        name = 'com_github_nelhage_rules_boost',
        urls = ['https://github.com/nelhage/rules_boost/archive/2613d04ab3d22dfc4543ea0a083d9adeaa0daf09.zip'],
        sha256 = '9a588a62062c8bf352b398a5db3ccc65561d43c40968762f7e05da76ccb5a6c9',
        strip_prefix = 'rules_boost-2613d04ab3d22dfc4543ea0a083d9adeaa0daf09',
        )

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

