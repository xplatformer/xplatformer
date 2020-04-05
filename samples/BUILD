load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "xanimation",
    srcs = glob(["animation/*.cc"]),
    deps = [
        "//:xgamelib",
    ],
)

cc_binary(
    name = "xclip",
    srcs = glob(["clipping/*.cc"]),
    deps = [
        "//:xgamelib",
    ],
)

cc_binary(
    name = "xdraw",
    srcs = glob(["drawing/*.cc"]),
    deps = [
        "//:xgamelib",
    ],
)

cc_binary(
    name = "xinput",
    srcs = glob(["input/*.cc"]),
    deps = [
        "//:xgamelib",
    ],
)

cc_binary(
    name = "xnull",
    srcs = glob(["null/*.cc"]),
    deps = [
        "//:xgamelib",
    ],
)

cc_binary(
    name = "xsprites",
    srcs = glob(["sprites/*.cc"]),
    data = [
        "//:assets",
    ],
    deps = [
        "//:xgamelib",
    ],
)

cc_binary(
    name = "xwindows",
    srcs = glob(["windows/*.cc"]),
    deps = [
        "//:xgamelib",
    ],
)
