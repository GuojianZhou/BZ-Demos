To build the opTest project

bazel build //src:opTest

To build the Caffe classification with the opencv library
sudo apt update && sudo apt install libopencv-dev

bazel build //classification:CFA
And the CFA application will be located in the bazel-bin/classification/CFA
