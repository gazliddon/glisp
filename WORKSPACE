load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
        name = 'com_github_nelhage_rules_boost',
        urls = ['https://github.com/nelhage/rules_boost/archive/2613d04ab3d22dfc4543ea0a083d9adeaa0daf09.zip'],
        sha256 = '9a588a62062c8bf352b398a5db3ccc65561d43c40968762f7e05da76ccb5a6c9',
        strip_prefix = 'rules_boost-2613d04ab3d22dfc4543ea0a083d9adeaa0daf09',
        )

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

http_archive(
    name = "immer",
    url = "https://github.com/arximboldi/immer/archive/ffbc180.zip",
    sha256 = "ec4c090d850cffdc8bc78355ec0bc6e694467d642c06ec9fe49936a1c97f377e",
    strip_prefix = "immer-ec4c090d850cffdc8bc78355ec0bc6e694467d642c06ec9fe49936a1c97f377e",
    build_file = "immer.BUILD",
)
