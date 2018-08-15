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

/************************************************************************
 *
 *  @file pool_op_test.cc
 *
 **************************************************************************/

/* test for primitive operator */
/*
 * define tensors: input, output
 * op name: pool
 * input size:     ni x ci x hi x wi
 * output size:    no x co x ho x wo
 */
#include <iostream>
#include "src/include/tests/tests.h"

int pool_test() {
  // prepare data for pool
  int ni = 1, ci = 2, hi = 6, wi = 6;
  int kh = 5, kw = 5, stride_h = 2, stride_w = 2;
  int pad_h = 0, pad_w = 0;
  int dilation_h = 0, dilation_w = 0;
  cnmlPoolMode_t pool_mode = CNML_POOL_AVG;
  // CNML_POOL_AVG, CNML_POOL_MAX
  cnmlPoolStrategyMode_t strategy = CNML_POOL_KFULL;
  // CNML_POOL_KFULL, CNML_POOL_KVALID
  bool real = true;
  int no, co, ho, wo;
  cnmlCoreVersion_t coreVersion = CNML_1H8;

  // prepare cnml input tensor
  cnmlTensor_t input_cnml;
  cnmlCreateTensor(&input_cnml,
          CNML_TENSOR,
          CNML_DATA_FLOAT16,
          ni, ci, hi, wi);

  // prepare pool param
  cnmlPoolOpParam_t pool_param;
  cnmlCreatePoolOpParam(&pool_param,
          kh, kw, stride_h, stride_w,
          pad_h, pad_w, dilation_h, dilation_w,
          pool_mode, strategy, real);

  cnmlGetPoolOpOutputDim(input_cnml, pool_param, &no, &co, &ho, &wo);
  printf("PPPPPP no = %d, co = %d, ho = %d, wo = %d\n", no, co, ho, wo);

  // prepare cnml output tensor
  cnmlTensor_t output_cnml;
  cnmlCreateTensor(&output_cnml,
          CNML_TENSOR,
          CNML_DATA_FLOAT16,
          no, co, ho, wo);

  // create pool operator
  cnmlBaseOp_t pool_op;
  cnmlCreatePoolOp(&pool_op,
          pool_param,
          input_cnml,
          output_cnml);

  // compile pool op
  cnmlCompileBaseOp(pool_op, coreVersion);

  // cnmlModel_t model;
  // cnmlCreateModel(&model, "pool_model");
  // cnmlAddBaseOpToModel(model, pool_op, "pool");
  // cnmlSaveModel(model, "pool.mef");
  // cnmlDestroyModel(model);

  // prepare cpu input tensor
  cnmlCpuTensor_t input_cpu;
  cnmlCreateCpuTensor(&input_cpu,
          CNML_TENSOR,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          ni, ci, hi, wi);

  // prepare cpu output tensor
  cnmlCpuTensor_t output_cpu;
  cnmlCreateCpuTensor(&output_cpu,
          CNML_TENSOR,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          no, co, ho, wo);

  float* input_cpu_data = (float*) malloc(ni * ci * hi * wi * sizeof(float));
  float* output_cpu_data = (float*) malloc(no * co * ho * wo * sizeof(float));
  float* cpu_result = (float*) malloc(no * co * ho * wo * sizeof(float));
  for (int index = 0; index < ni * ci * hi * wi; ++index) {
      input_cpu_data[index] = index % 10 - 5;
  }

  // cnml tensor malloc
  void *input_cnml_data = cnmlMallocBuffer(input_cnml);
  void *output_cnml_data = cnmlMallocBuffer(output_cnml);

  // copy tensor from cpu to cnml
  cnmlMemcpyTensorToDevice(
          input_cpu,
          input_cpu_data,
          input_cnml,
          input_cnml_data);

  /* create stream and func_type and llc */
  cnrtFunctionType_t func_type = CNRT_FUNC_TYPE_BLOCK;
  cnrtStream_t stream;
  cnrtCreateStream(&stream);

  // compute pool
  cnmlComputePoolOpForward(pool_op,
          input_cnml_data,
          output_cnml_data, func_type, stream);

  // copy cnml result to cpu
  cnrtSyncStream(stream);
  cnrtDestroyStream(stream);

  cnmlMemcpyTensorToHost(
          output_cnml,
          output_cnml_data,
          output_cpu,
          output_cpu_data);

  const char* file_name = "mlu_output";
  cnmlDumpTensor2File(file_name, output_cpu,
          CNML_TENSOR, output_cpu_data, false);

  cnmlCpuComputePoolOpForward(pool_param,
          input_cpu, input_cpu_data,
          output_cpu, cpu_result);

  const char* cpu_fname = "cpu_output";
  cnmlDumpTensor2File(cpu_fname, output_cpu,
          CNML_TENSOR, cpu_result, false);

  cnmlFreeBuffer(input_cnml_data);
  cnmlFreeBuffer(output_cnml_data);
  // destroy pool param
  cnmlDestroyPoolOpParam(&pool_param);
  // delete cnml tensors
  cnmlDestroyTensor(&input_cnml);
  cnmlDestroyTensor(&output_cnml);
  // delete cpu tensors
  cnmlDestroyCpuTensor(&input_cpu);
  cnmlDestroyCpuTensor(&output_cpu);

  cnmlDestroyBaseOp(&pool_op);

  // delete all pointers (including data pointers)
  free(input_cpu_data);
  free(output_cpu_data);
  free(cpu_result);

  return 0;
}
