load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "9f9fb8b2f0213989247c9d5c0e814a8451d18d7f",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1570056263 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "immer",
    url = "https://github.com/arximboldi/immer/archive/ffbc180.zip",
    sha256 = "ec4c090d850cffdc8bc78355ec0bc6e694467d642c06ec9fe49936a1c97f377e",
    strip_prefix = "immer-ec4c090d850cffdc8bc78355ec0bc6e694467d642c06ec9fe49936a1c97f377e",
    build_file = "immer.BUILD",
)
