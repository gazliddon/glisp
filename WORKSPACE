load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "immer",
    url = "https://github.com/arximboldi/immer/archive/2076affd9d814afc019ba8cd8c2b18a6c79c9589.zip",
    sha256 = "13c926c3c9d83d8b67b0e83f3ec244aaa1c9e0b16023f299b674d5a23b637929",
    strip_prefix = "immer-2076affd9d814afc019ba8cd8c2b18a6c79c9589",
    build_file = "@//:third_party/immer.BUILD",
)

http_archive(
        name = 'com_github_nelhage_rules_boost',
        urls = ['https://github.com/nelhage/rules_boost/archive/2613d04ab3d22dfc4543ea0a083d9adeaa0daf09.zip'],
        sha256 = '9a588a62062c8bf352b398a5db3ccc65561d43c40968762f7e05da76ccb5a6c9',
        strip_prefix = 'rules_boost-2613d04ab3d22dfc4543ea0a083d9adeaa0daf09',
        )

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

