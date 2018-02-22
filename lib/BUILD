cc_library(
    name = "llvm",
    srcs = select({
        "@toolchain//:linux_x86_64": ["linux_x86_64/libllvm.a"],
        "//conditions:default": ["x64linux/libllvm.a"],
    }),
    tags = ["llvm"],
    visibility = ["//visibility:public"],
    deps = ["@llvm_archive//:llvm_inc"],
)
