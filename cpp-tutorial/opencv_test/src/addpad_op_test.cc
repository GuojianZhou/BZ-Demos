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

#include <iostream>
#include "src/include/tests/tests.h"

int addpad_test() {
  unsigned int seed = time(0);
  // prepare data and param for addpad
  int pad_h = 2, pad_w = 2, pad_value = 0;
  cnmlAddPadOpParam_t addpad_param_ptr;
  cnmlCreateAddPadOpParam(&addpad_param_ptr, pad_h, pad_w, pad_value);

  const int SPLIT_SUM = 1;
  const int ni = 1, ci = 48, hi = 2, wi = 2;
  const int no = 1, co = 48, ho = hi+pad_h, wo = wi+pad_w;

  float* input_cpu_ptr = (float*)malloc(ni * ci * hi * wi * sizeof(float));
  float* output_cpu_ptr = (float*)malloc(no * co * ho * wo * sizeof(float));
  float* cpu_result = (float*)malloc(no * co * ho * wo * sizeof(float));

  for (int i = 0; i < ni * ci * hi * wi; i++) {
    input_cpu_ptr[i] = rand_r(&seed) % 2 - 1;
  }

  // prepare cpu input tensor
  cnmlCpuTensor_t input_cpu_tensor_ptr;
  cnmlCreateCpuTensor(&input_cpu_tensor_ptr,
                     CNML_TENSOR,
                     CNML_DATA_FLOAT32,
                     CNML_NCHW,
                     ni,
                     ci,
                     hi,
                     wi);

  // prepare cpu output tensor
  cnmlCpuTensor_t output_cpu_tensor_ptr;
  cnmlCreateCpuTensor(&output_cpu_tensor_ptr,
                     CNML_TENSOR,
                     CNML_DATA_FLOAT32,
                     CNML_NCHW,
                     no,
                     co,
                     ho,
                     wo);

  // run cpu version
  cnmlCpuComputeAddPadOpForward(addpad_param_ptr, input_cpu_tensor_ptr,
          input_cpu_ptr, output_cpu_tensor_ptr, cpu_result);
  cnmlDumpTensor2File("cpu_output", output_cpu_tensor_ptr, CNML_TENSOR,
          cpu_result, false);

  // prepare cnml input tensor
  cnmlTensor_t input_cnml_tensor_ptr;
  cnmlCreateTensor(&input_cnml_tensor_ptr,
                  CNML_TENSOR,
                  CNML_DATA_FLOAT16,
                  ni,
                  ci,
                  hi,
                  wi);
  // prepare cnml output tensor
  cnmlTensor_t output_cnml_tensor_ptr;
  cnmlCreateTensor(&output_cnml_tensor_ptr,
                  CNML_TENSOR,
                  CNML_DATA_FLOAT16,
                  no,
                  co,
                  ho,
                  wo);
  // create addpad operator
  cnmlBaseOp_t op_ptr;
  cnmlCreateAddPadOp(&op_ptr,
                 addpad_param_ptr,
                 input_cnml_tensor_ptr,
                 output_cnml_tensor_ptr);

  cnmlCompileBaseOp(op_ptr, CNML_1H8);
  // cnml tensor malloc
  void* input_cnml_ptr = cnmlMallocBuffer(input_cnml_tensor_ptr);
  void* output_cnml_ptr = cnmlMallocBuffer(output_cnml_tensor_ptr);
  // copy tensor from cpu to cnml
  cnmlMemcpyTensorToDevice(
                  input_cpu_tensor_ptr,
                  input_cpu_ptr,
                  input_cnml_tensor_ptr,
                  input_cnml_ptr);
  /* create stream and func_type and llc */
  cnrtFunctionType_t func_type = CNRT_FUNC_TYPE_BLOCK;
  cnrtStream_t stream;
  cnrtCreateStream(&stream);

  // compute addpad
  cnmlComputeAddPadOpForward(op_ptr,
      input_cnml_ptr,
      output_cnml_ptr,
      func_type,
      stream
      );
  // copy cnml result to cpu
  cnrtSyncStream(stream);
  cnrtDestroyStream(stream);

  cnmlMemcpyTensorToHost(
                output_cnml_tensor_ptr,
                output_cnml_ptr,
                output_cpu_tensor_ptr,
                output_cpu_ptr);
  cnmlDumpTensor2File("mlu_output", output_cpu_tensor_ptr, CNML_TENSOR,
          output_cpu_ptr, false);

  // cnml tensor ptr free
  cnmlFreeBuffer(input_cnml_ptr);
  cnmlFreeBuffer(output_cnml_ptr);
  // destroy op, tensor etc.
  cnmlDestroyAddPadOpParam(&addpad_param_ptr);
  cnmlDestroyTensor(&input_cnml_tensor_ptr);
  cnmlDestroyTensor(&output_cnml_tensor_ptr);
  cnmlDestroyCpuTensor(&input_cpu_tensor_ptr);
  cnmlDestroyCpuTensor(&output_cpu_tensor_ptr);
  cnmlDestroyBaseOp(&op_ptr);

  // delete all pointers
  free(input_cpu_ptr);
  free(output_cpu_ptr);
  free(cpu_result);

  return 0;
}
