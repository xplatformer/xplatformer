load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "xgamelib",
    srcs = glob(["lib/*.cc"]),
    hdrs = glob(["lib/*.h"]),
    copts = [
        "--std=c++1y",
    ],
    visibility = ["//samples:__pkg__"],
    deps = ["@system_libs//:x11"],
)

filegroup(
    name = "assets",
    srcs = glob([
        "assets/*",
        "assets/**/*",
    ]),
    visibility = ["//samples:__pkg__"],
)
