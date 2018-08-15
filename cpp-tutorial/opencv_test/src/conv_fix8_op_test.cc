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
 * define tensors: input, weight, bias and output
 * op name : conv
 * input size:     ni x ci x hi x wi
 * weight size:    co x ci x kh x kw
 * bias size:      1 x co x 1 x 1
 * output size:    no x co x ho x wo
*/
#include <iostream>
#include "src/include/tests/tests.h"

int conv_fix8_test() {
  // prepare data for conv
  unsigned int seed = time(0);
  const int SPLIT_NUM = 1;
  const int ni = 1, ci = 64, hi = 32, wi = 32;
  const int no = 1, co = 64, ho = 30, wo = 30;
  const int kh = 3, kw = 3, stride_h = 1, stride_w = 1;
  const int dilation_h = 1, dilation_w = 1;

  float *input_cpu_data = (float*)malloc(ni * ci * hi * wi * sizeof(float));
  for (int index = 0; index < ni * ci * hi * wi; ++index) {
      input_cpu_data[index] = ((rand_r(&seed) % 100 / 100.0) - 0.5) / 8;
  }

  float *filter_cpu_data = (float*)malloc(co * ci * kh * kw * sizeof(float));
  for (int index = 0; index < co * ci * kh * kw; ++index) {
      filter_cpu_data[index] = ((rand_r(&seed) % 100 / 100.0) - 0.5) / 8;
  }

  float *output_cpu_data = (float*)malloc(no * co * ho * wo * sizeof(float));
  float *cpu_result = (float*)malloc(no * co * ho * wo * sizeof(float));
  for (int index = 0; index < no * co * ho * wo; ++index) {
      output_cpu_data[index] = 0.0;
  }

  float *bias_cpu_data = (float*)malloc(co * sizeof(float));
  for (int index = 0; index < co; ++index) {
      bias_cpu_data[index] = (rand_r(&seed) % 100 / 100.0) * 128;
  }

  // prepare input cpu tensor
  cnmlCpuTensor_t input_cpu;
  cnmlCreateCpuTensor(&input_cpu,
          CNML_TENSOR,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          ni,
          ci,
          hi,
          wi);

  // prepare weight cpu tensor
  cnmlCpuTensor_t filter_cpu;
  cnmlCreateCpuTensor(&filter_cpu,
          CNML_FILTER,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          co,
          ci,
          kh,
          kw);

  // prepare bias cpu tensor
  cnmlCpuTensor_t bias_cpu;
  cnmlCreateCpuTensor(&bias_cpu,
          CNML_CONST,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          1,
          co,
          1,
          1);

  // prepare output cpu tensor
  cnmlCpuTensor_t output_cpu;
  cnmlCreateCpuTensor(&output_cpu,
          CNML_TENSOR,
          CNML_DATA_FLOAT32,
          CNML_NCHW,
          no,
          co,
          ho,
          wo);

  // no data order needed in creating cnml tensor
  // prepare input cnml tensor
  cnmlTensor_t input_cnml;
  cnmlCreateTensor(&input_cnml,
          CNML_TENSOR,
          CNML_DATA_FIX8,
          ni,
          ci,
          hi,
          wi);
  int input_position = getFix8Position(input_cpu_data, ni * ci * hi * wi);
  cnmlSetFix8Position(input_cnml, input_position);

  // prepare filter cnml tensor
  cnmlTensor_t filter_cnml;
  cnmlCreateTensor(&filter_cnml,
          CNML_FILTER,
          CNML_DATA_FIX8,
          co,
          ci,
          kh,
          kw);
  int filter_position = getFix8Position(filter_cpu_data, co * ci * kh * kw);
  cnmlSetFix8Position(filter_cnml, filter_position);

  // prepare bias cnml tensor
  cnmlTensor_t bias_cnml;
  cnmlCreateTensor(&bias_cnml,
          CNML_CONST,
          CNML_DATA_FLOAT16,
          1,
          co,
          1,
          1);

  // prepare output cnml tensor
  cnmlTensor_t output_cnml;
  cnmlCreateTensor(&output_cnml,
          CNML_TENSOR,
          CNML_DATA_FLOAT16,
          no,
          co,
          ho,
          wo);

  // bind cpu filter, bias to cnmltensor
  cnmlBindConstData(filter_cnml,
                    filter_cpu,
                    filter_cpu_data);
  cnmlBindConstData(bias_cnml,
                    bias_cpu,
                    bias_cpu_data);
  // prepare conv param
  cnmlConvOpParam_t conv_param;
  cnmlCreateConvOpParam(&conv_param,
          stride_h,
          stride_w,
          dilation_h,
          dilation_w);

  // setup conv operator
  cnmlBaseOp_t conv_op;
  cnmlCreateConvOp(&conv_op,
          conv_param,
          input_cnml,
          output_cnml,
          filter_cnml,
          bias_cnml);

  cnmlEnableConvOpFix8Mode(conv_op);

  // compile op
  cnmlCompileBaseOp(conv_op, CNML_1H8);

  // malloc cnml tensor
  void *input_cnml_data = cnmlMallocBuffer(
          input_cnml);
  void *output_cnml_data = cnmlMallocBuffer(
          output_cnml);

  // copy input to cnml tensor
  cnmlMemcpyTensorToDevice(
          input_cpu,
          input_cpu_data,
          input_cnml,
          input_cnml_data);

  /* create stream and func_type and llc */
  cnrtFunctionType_t func_type = CNRT_FUNC_TYPE_BLOCK;
  cnrtStream_t stream;
  cnrtCreateStream(&stream);

  // compute conv op
  cnmlComputeConvOpForward(conv_op,
          input_cnml_data,
          output_cnml_data, func_type, stream);

  // copy output to cpu tensor
  cnrtSyncStream(stream);
  cnrtDestroyStream(stream);

  cnmlMemcpyTensorToHost(
          output_cnml,
          output_cnml_data,
          output_cpu,
          output_cpu_data);
  cnmlDumpTensor2File("mlu_output", output_cpu, CNML_TENSOR, output_cpu_data, false);

  cnmlCpuComputeConvOpForward(conv_param,
          input_cpu,
          input_cpu_data,
          filter_cpu,
          filter_cpu_data,
          output_cpu,
          cpu_result,
          bias_cpu,
          bias_cpu_data);
  cnmlDumpTensor2File("cpu_output", output_cpu, CNML_TENSOR, cpu_result, false);

  // delete conv param
  cnmlDestroyConvOpParam(&conv_param);

  // delete cnml tensors
  cnmlDestroyTensor(&input_cnml);
  cnmlDestroyTensor(&filter_cnml);
  cnmlDestroyTensor(&bias_cnml);
  cnmlDestroyTensor(&output_cnml);

  // delete cpu tensors
  cnmlDestroyCpuTensor(&input_cpu);
  cnmlDestroyCpuTensor(&filter_cpu);
  cnmlDestroyCpuTensor(&bias_cpu);
  cnmlDestroyCpuTensor(&output_cpu);
  cnmlDestroyBaseOp(&conv_op);

  //  delete pointers (including data pointers)
  free(input_cpu_data);
  free(filter_cpu_data);
  free(bias_cpu_data);
  free(output_cpu_data);
  free(cpu_result);

  return 0;
}
