cc_library(
    name = "llvm",
    srcs = select({
        "//bzl:x64-ubuntu-14.04": ["x64-ubuntu-14.04/libllvm.a"],
        "//conditions:default": ["x64linux/libllvm.a"],
    }),
    tags = ["llvm"],
    visibility = ["//visibility:public"],
    deps = ["@llvm_archive//:llvm_inc"],
)
