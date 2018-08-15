/*************************************************************************
 * Copyright (C) [2018] by Cambricon, Inc.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *************************************************************************/

/* test for primitive operator */
/*
 * define tensors: input, output
 * op name: active
 * input size: n x c x h x w
 * output size: n x c x h x w
 */
#include <iostream>
#include "src/include/tests/tests.h"

int active_test() {
  // prepare data for pool
  const int n = 1, c = 32, h = 4, w = 4;
  std::cout << "please choose active function:" << std::endl;
  std::cout << "1.relu" << std::endl;
  std::cout << "2.sigmoid" << std::endl;
  std::cout << "3.tanh" << std::endl;
  std::cout << "4.relu1" << std::endl;
  std::cout << "5.relu6" << std::endl;

  int option;
  std::cin >> option;
  cnmlActiveFunction_t active_func;
  switch (option) {
    case 1 : active_func = CNML_ACTIVE_RELU; break;
    case 2 : active_func = CNML_ACTIVE_SIGMOID; break;
    case 3 : active_func = CNML_ACTIVE_TANH; break;
    case 4 : active_func = CNML_ACTIVE_RELU1; break;
    case 5 : active_func = CNML_ACTIVE_RELU6; break;
    default : std::cout << "wrong input" << std::endl; break;
  }

  float* input_cpu_data = (float*)malloc(n * c * h * w * sizeof(float));
  float* output_cpu_data = (float*)malloc(n * c * h * w * sizeof(float));
  float* cpu_result = (float*)malloc(n * c * h * w * sizeof(float));

  for (int index = 0; index < n * c * h * w; ++index) {
    input_cpu_data[index] = (float)(index % 200 / 10.0 - 8);
  }

  // prepare cpu input tensor
  cnmlCpuTensor_t input_cpu;
  cnmlCreateCpuTensor(&input_cpu,
      CNML_TENSOR,
      CNML_DATA_FLOAT32,
      CNML_NCHW,
      n,
      c,
      h,
      w);

  // prepare cpu output tensor
  cnmlCpuTensor_t output_cpu;
  cnmlCreateCpuTensor(&output_cpu,
      CNML_TENSOR,
      CNML_DATA_FLOAT32,
      CNML_NCHW,
      n,
      c,
      h,
      w);

  // prepare cnml input tensor
  cnmlTensor_t input_cnml;
  cnmlCreateTensor(&input_cnml,
      CNML_TENSOR,
      CNML_DATA_FLOAT16,
      n,
      c,
      h,
      w);

  // int input_position = getFix8Position(input_cpu_data, n * c * h * w);
  // cnmlSetFix8Position(input_cnml, input_position);
  // prepare cnml output tensor
  cnmlTensor_t output_cnml;
  cnmlCreateTensor(&output_cnml,
      CNML_TENSOR,
      CNML_DATA_FLOAT16,
      n,
      c,
      h,
      w);

  // create active operator
  cnmlBaseOp_t active_op;
  cnmlCreateActiveOp(&active_op,
                    active_func,
                    input_cnml,
                    output_cnml);

  // compile active op
  cnmlCompileBaseOp(active_op, CNML_1H8);

  void *input_cnml_data = cnmlMallocBuffer(input_cnml);
  void *output_cnml_data = cnmlMallocBuffer(output_cnml);

  // copy tensor from cpu to cnml
  cnmlMemcpyTensorToDevice(input_cpu,
                  input_cpu_data,
                  input_cnml,
                  input_cnml_data);

/* create stream and func_type and llc */
  cnrtFunctionType_t func_type = CNRT_FUNC_TYPE_BLOCK;
  cnrtStream_t stream;
  cnrtCreateStream(&stream);

  // compute active
  cnmlComputeActiveOpForward(active_op,
                            input_cnml_data,
                            output_cnml_data, func_type,
                            stream);

  // copy cnml result to cpu
  cnrtSyncStream(stream);
  cnrtDestroyStream(stream);

  cnmlMemcpyTensorToHost(output_cnml,
                output_cnml_data,
                output_cpu,
                output_cpu_data);

  cnmlFreeBuffer(input_cnml_data);
  cnmlFreeBuffer(output_cnml_data);

  cnmlDumpTensor2File("mlu_output", output_cpu, CNML_TENSOR, output_cpu_data, false);

  cnmlCpuComputeActiveOpForward(active_func, input_cpu, input_cpu_data,
          output_cpu, cpu_result);
  cnmlDumpTensor2File("cpu_output", output_cpu, CNML_TENSOR, cpu_result, false);

  // destroy cnml tensors
  cnmlDestroyTensor(&input_cnml);
  cnmlDestroyTensor(&output_cnml);

  // destroy cpu tensors
  cnmlDestroyCpuTensor(&input_cpu);
  cnmlDestroyCpuTensor(&output_cpu);
  cnmlDestroyBaseOp(&active_op);

  // delete all pointers (including data pointers)
  free(input_cpu_data);
  free(output_cpu_data);
  free(cpu_result);
  return 0;
}
