cc_library(
	name = "cnmlTest",
	srcs = [
		"active_op_test.cc",
	 	"addpad_op_test.cc",
	 	"conv_fix8_op_test.cc",
	 	"mlp_fix8_op_test.cc",
	 	"pool_op_test.cc",
	 	"tool.cc",
		],
	hdrs = [
		"include/tool.h",
	 	"include/tests/tests.h",
	 	],
	deps = [
		"//lib:libcnrt",
		"//lib:libcnml",
		],
)

cc_binary(
	name = "opTest",
	srcs = [
	 	"all_test.cc",
		],
	deps = [
		":cnmlTest",
		"//lib:libcnrt",
		],
)
