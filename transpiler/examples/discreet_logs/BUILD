load("//transpiler:fhe.bzl", "fhe_cc_library")

licenses(["notice"])

cc_library(
    name = "hangman_api",
    srcs = ["hangman_api.cc"],
    hdrs = ["hangman_api.h"],
    deps = [],
)

cc_library(
    name = "discreet_logs_select_index",
    srcs = ["discreet_logs_select_index.cc"],
    hdrs = ["discreet_logs_select_index.h"],
    deps = [],
)

cc_library(
    name = "discreet_logs_count",
    srcs = ["discreet_logs_count.cc"],
    hdrs = ["discreet_logs_count.h"],
    deps = [],
)

cc_library(
    name = "discreet_logs_read_all",
    srcs = ["discreet_logs_read_all.cc"],
    hdrs = ["discreet_logs_read_all.h"],
    deps = [],
)

cc_binary(
    name = "hangman_client",
    srcs = [
        "hangman_api.h",
        "discreet_logs_select_index.h",
        "discreet_logs_count.h",
        "discreet_logs_read_all.h",
        "hangman_client_main.cc",
    ],
    deps = [
        ":hangman_api_tfhe",
        ":discreet_logs_select_index_tfhe",
        ":discreet_logs_count_tfhe",
        ":discreet_logs_read_all_tfhe",
        ":hangman_client_lib",
        "//transpiler/data:tfhe_data",
        "@com_google_absl//absl/base",
        "@com_google_xls//xls/common/logging",
        "@com_google_xls//xls/common/status:status_macros",
        "@tfhe//:libtfhe",
    ],
)

fhe_cc_library(
    name = "hangman_api_tfhe",
    src = "hangman_api.cc",
    hdrs = ["hangman_api.h"],
    encryption = "tfhe",
    num_opt_passes = 2,
    interpreter = True,
    optimizer = "xls",
)

fhe_cc_library(
    name = "discreet_logs_select_index_tfhe",
    src = "discreet_logs_select_index.cc",
    hdrs = ["discreet_logs_select_index.h"],
    encryption = "tfhe",
    num_opt_passes = 2,
    interpreter = True,
    optimizer = "xls",
)

fhe_cc_library(
    name = "discreet_logs_count_tfhe",
    src = "discreet_logs_count.cc",
    hdrs = ["discreet_logs_count.h"],
    encryption = "tfhe",
    num_opt_passes = 2,
    interpreter = True,
    optimizer = "xls",
)

fhe_cc_library(
    name = "discreet_logs_read_all_tfhe",
    src = "discreet_logs_read_all.cc",
    hdrs = ["discreet_logs_read_all.h"],
    encryption = "tfhe",
    num_opt_passes = 2,
    interpreter = True,
    optimizer = "xls",
)

cc_library(
    name = "hangman_client_lib",
    srcs = ["hangman_client.cc"],
    hdrs = ["hangman_client.h"],
    deps = [
        ":hangman_api",
        ":hangman_api_tfhe",
        ":discreet_logs_select_index",
        ":discreet_logs_select_index_tfhe",
        ":discreet_logs_count",
        ":discreet_logs_count_tfhe",
        ":discreet_logs_read_all",
        ":discreet_logs_read_all_tfhe",
        "//transpiler/data:tfhe_data",
    ],
)

cc_test(
    name = "hangman_client_test",
    srcs = ["hangman_client_test.cc"],
    deps = [
        ":hangman_client_lib",
        "@com_google_googletest//:gtest_main",
        "@com_google_xls//xls/common/status:matchers",
    ],
)
