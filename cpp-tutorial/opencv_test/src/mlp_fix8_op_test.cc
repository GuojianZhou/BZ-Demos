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
//  define tensors: input, weight, bias and output
/* 
 * op name : mlp
 * input size:    ni x ci x hi x wi
 * weight size:   co x ci x kh x kw
 * bias size:     1 x co x 1 x 1
 * output size:   no x co x ho x wo
 * param: input_cpu_ptr, weight_cpu_ptr, bias_cpu_ptr, output_cpu_ptr
*/
#include <iostream>
#include <cmath>
#include "src/include/tests/tests.h"

int mlp_fix8_test() {
  srand(time(0));
  /*  prepare data for conv  */
  const int SPLIT_NUM = 1;
  const int ni = 1, ci = 512, hi = 1, wi = 1;
  const int no = 1, co = 512, ho = 1, wo = 1;
  const int kh = hi, kw = wi;

  float* input_cpu_ptr = (float*)malloc(ni * ci * hi * wi * sizeof(float));
  for (int index = 0; index < ni * ci * hi * wi; index++) {
      input_cpu_ptr[index] = 0.5;
  }

  float* weight_cpu_ptr = (float*)malloc(co * ci * kh * kw * sizeof(float));
  for (int index = 0; index < co * ci * kh * kw; index++) {
      weight_cpu_ptr[index] = 2;
  }

  float* output_cpu_ptr = (float*)malloc(no * co * ho * wo * sizeof(float));
  float* cpu_result = (float*)malloc(no * co * ho * wo * sizeof(float));
  for (int index = 0; index < no * co * ho * wo; index++) {
      output_cpu_ptr[index] = 0.1;
  }

  float* bias_cpu_ptr = (float*)malloc(co * sizeof(float));
  for (int index = 0; index < co; index++) {
      bias_cpu_ptr[index] = 2;
  }

  /* prepare input cpu tensor */
  cnmlCpuTensor_t input_cpu_tensor_ptr;
  cnmlCreateCpuTensor(&input_cpu_tensor_ptr,
          CNML_TENSOR,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          ni,
          ci,
          hi,
          wi);

  /* prepare weight cpu tensor */
  cnmlCpuTensor_t weight_cpu_filter_ptr;
  cnmlCreateCpuTensor(&weight_cpu_filter_ptr,
          CNML_FILTER,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          co,
          ci,
          kh,
          kw);

  /* prepare bias cpu tensor */
  cnmlCpuTensor_t bias_cpu_tensor_ptr;
  cnmlCreateCpuTensor(&bias_cpu_tensor_ptr,
          CNML_CONST,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          1,
          co,
          1,
          1);

  /* prepare output cpu tensor */
  cnmlCpuTensor_t output_cpu_tensor_ptr;
  cnmlCreateCpuTensor(&output_cpu_tensor_ptr,
          CNML_TENSOR,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          no,
          co,
          ho,
          wo);

  /* prepare input cnml tensor */
  cnmlTensor_t input_cnml_tensor_ptr;
  cnmlCreateTensor(&input_cnml_tensor_ptr,
          CNML_TENSOR,
          CNML_DATA_FIX8,
          ni,
          ci,
          hi,
          wi);
  int input_position = getFix8Position(input_cpu_ptr, ni * ci * hi * wi);
  cnmlSetFix8Position(input_cnml_tensor_ptr, input_position);

  /* prepare weight cnml tensor */
  cnmlTensor_t weight_cnml_tensor_ptr;
  cnmlCreateTensor(&weight_cnml_tensor_ptr,
          CNML_FILTER,
          CNML_DATA_FIX8,
          co,
          ci,
          kh,
          kw);
  int weight_position = getFix8Position(weight_cpu_ptr, co * ci * kh * kw);
  cnmlSetFix8Position(weight_cnml_tensor_ptr, weight_position);

  /* prepare bias cnml tensor */
  cnmlTensor_t bias_cnml_tensor_ptr;
  cnmlCreateTensor(&bias_cnml_tensor_ptr,
          CNML_CONST,
          CNML_DATA_FLOAT16,
          1,
          co,
          1,
          1);

  /* prepare output cnml tensor */
  cnmlTensor_t output_cnml_tensor_ptr;
  cnmlCreateTensor(&output_cnml_tensor_ptr,
          CNML_TENSOR,
          CNML_DATA_FLOAT16,
          no,
          co,
          ho,
          wo);

  /* bind const data */
  cnmlBindConstData(weight_cnml_tensor_ptr,
                    weight_cpu_filter_ptr,
                    weight_cpu_ptr);
  cnmlBindConstData(bias_cnml_tensor_ptr,
                    bias_cpu_tensor_ptr,
                    bias_cpu_ptr);

  /* setup mlp operator */
  cnmlBaseOp_t mlp_op_ptr;
  cnmlCreateMlpOp(&mlp_op_ptr,
          input_cnml_tensor_ptr,
          output_cnml_tensor_ptr,
          weight_cnml_tensor_ptr,
          bias_cnml_tensor_ptr);

  cnmlEnableMlpOpFix8Mode(mlp_op_ptr);

  cnmlCompileBaseOp(mlp_op_ptr, CNML_1H8);
  cnmlModel_t model;
  cnmlCreateModel(&model, "mlp_model");
  cnmlAddBaseOpToModel(model, mlp_op_ptr, "mlp");
  cnmlSaveModel(model, "mlp.mef");
  cnmlDestroyModel(model);

  /* cnml tensor malloc */
  void* input_mlu = cnmlMallocBuffer(input_cnml_tensor_ptr);
  void* output_mlu = cnmlMallocBuffer(output_cnml_tensor_ptr);

  cnmlMemcpyTensorToDevice(input_cpu_tensor_ptr, input_cpu_ptr,
          input_cnml_tensor_ptr, input_mlu);

  /* create stream and func_type and llc */
  cnrtFunctionType_t func_type = CNRT_FUNC_TYPE_BLOCK;
  cnrtStream_t stream;
  cnrtCreateStream(&stream);

  /* conv compute */
  cnmlComputeMlpOpForward(mlp_op_ptr,
          input_mlu,
          output_mlu,
          func_type,
          stream
          );

  cnrtSyncStream(stream);
  cnrtDestroyStream(stream);

  cnmlMemcpyTensorToHost(output_cnml_tensor_ptr, output_mlu,
          output_cpu_tensor_ptr, output_cpu_ptr);

  cnmlDumpTensor2File("mlu_output", output_cpu_tensor_ptr,
          CNML_TENSOR, output_cpu_ptr, false);

  cnmlCpuComputeMlpOpForward(input_cpu_tensor_ptr, input_cpu_ptr,
          output_cpu_tensor_ptr, cpu_result,
          weight_cpu_filter_ptr, weight_cpu_ptr,
          bias_cpu_tensor_ptr, bias_cpu_ptr);
  cnmlDumpTensor2File("cpu_output", output_cpu_tensor_ptr,
          CNML_TENSOR, cpu_result, false);

  cnmlFreeBuffer(input_mlu);
  cnmlFreeBuffer(output_mlu);

  cnmlDestroyTensor(&input_cnml_tensor_ptr);
  cnmlDestroyTensor(&weight_cnml_tensor_ptr);
  cnmlDestroyTensor(&bias_cnml_tensor_ptr);
  cnmlDestroyTensor(&output_cnml_tensor_ptr);
  cnmlDestroyCpuTensor(&bias_cpu_tensor_ptr);
  cnmlDestroyCpuTensor(&weight_cpu_filter_ptr);
  cnmlDestroyCpuTensor(&input_cpu_tensor_ptr);
  cnmlDestroyCpuTensor(&output_cpu_tensor_ptr);
  cnmlDestroyBaseOp(&mlp_op_ptr);

  /* delete all pointers */
  free(input_cpu_ptr);
  free(weight_cpu_ptr);
  free(bias_cpu_ptr);
  free(output_cpu_ptr);

  return 0;
}
