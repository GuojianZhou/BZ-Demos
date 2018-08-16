licenses(["restricted"])  # MPL2, portions GPL v3, LGPL v3, BSD-like
package(default_visibility = ["//visibility:public"])

cc_library(
	name = "libcnrt",
	srcs = glob(["libcnrt.so"]),
	hdrs = ["cnrt.h"],
	linkopts = ["-lcnrt"],
	visibility = [
			"//src:__pkg__",
			"//classification:__pkg__",
	],
)

cc_library(
	name = "libcnml",
	srcs = glob(["libcnml.so"]),
	hdrs = ["cnml.h"],
	linkopts = ["-lcnml"],
	visibility = [	"//visibility:public",
			],
)
