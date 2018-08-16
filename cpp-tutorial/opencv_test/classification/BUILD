cc_library(
	name = "opencv_lib",
	linkopts = [
	"-lopencv_highgui",
	"-lopencv_imgproc",
	"-lopencv_core",
	],
	visibility = ["//visibility:public"],
)

cc_binary(
	name = "CFA",
	srcs = [
	 	"classfication_fix8.cpp",
		],
	deps = [
		":opencv_lib",
		"//lib:libcnrt",
		],
)
