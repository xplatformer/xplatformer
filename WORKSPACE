new_local_repository(
    name = "system_libs",
    build_file_content = """
cc_library(
    name = "x11",
    srcs = ["libX11.so"],
    visibility = ["//visibility:public"],
)
""",
    path = "/usr/lib/x86_64-linux-gnu",
)