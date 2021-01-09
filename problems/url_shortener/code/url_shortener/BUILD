cc_binary(
    name = "bin",
    srcs = glob([
        "bin/**/*.cc",
        "bin/**/*.h",
    ]),
    includes = ["."],
    deps = [
        ":inc",
        "@com_github_jarro2783_cxxopts//:cxxopts",
        "@nlohmann_json_lib//:nlohmann_json",
    ],
)

cc_library(
    name = "inc",
    hdrs = glob(["inc/cache_storage.h"]),
    deps = [
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
    ],
)
