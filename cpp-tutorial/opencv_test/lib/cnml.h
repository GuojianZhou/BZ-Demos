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
 *  @file cnml.h
 *
 *  @brief cnml APIs provide programmable interfaces for users to develop
 *  their-owned programs, which includes device managment, context
 *  management, memory managment of both sides (devices and hosts), etc.
 *
 **************************************************************************/

#ifndef CNML_H_
#define CNML_H_

#if defined (__cplusplus)
extern "C" {
#endif

#include "cnrt.h"  // NOLINT

typedef enum {
  CNML_1H8  = 1,
  CNML_1H16 = 2,
  CNML_C10  = 3,
} cnmlCoreVersion_t;

typedef enum {
    CNML_NCHW = 0,
    CNML_NCWH = 1,
    CNML_NHCW = 2,
    CNML_NHWC = 3,
    CNML_NWHC = 4,
    CNML_NWCH = 5,
    CNML_CNHW = 6,
    CNML_CNWH = 7,
    CNML_CHNW = 8,
    CNML_CHWN = 9,
    CNML_CWHN = 10,
    CNML_CWNH = 11,
    CNML_HCNW = 12,
    CNML_HCWN = 13,
    CNML_HNCW = 14,
    CNML_HNWC = 15,
    CNML_HWNC = 16,
    CNML_HWCN = 17,
    CNML_WCHN = 18,
    CNML_WCNH = 19,
    CNML_WHCN = 20,
    CNML_WHNC = 21,
    CNML_WNHC = 22,
    CNML_WNCH = 23,
} cnmlDataOrder_t;

typedef enum {
    CNML_DATA_INVALID = 0,
    CNML_DATA_FLOAT16 = 1,
    CNML_DATA_FLOAT32 = 2,
    CNML_DATA_DOUBLE  = 3,
    CNML_DATA_FIX8    = 4,
    CNML_DATA_INT8    = 5,
    CNML_DATA_INT16   = 6,
    CNML_DATA_INT32   = 7,
    CNML_DATA_UINT8   = 8,
    CNML_DATA_UINT16  = 9,
    CNML_DATA_UINT32  = 10,
    CNML_DATA_QUANT8  = 11,
    CNML_DATA_BINARY  = 12,
    CNML_DATA_BOOL    = 13,
} cnmlDataType_t;

typedef enum {
    CNML_STATUS_NODEVICE     = -1,
    CNML_STATUS_SUCCESS      = 0,
    CNML_STATUS_DOMAINERR    = 1,
    CNML_STATUS_INVALIDARG   = 2,
    CNML_STATUS_LENGTHERR    = 3,
    CNML_STATUS_OUTOFRANGE   = 4,
    CNML_STATUS_RANGEERR     = 5,
    CNML_STATUS_OVERFLOWERR  = 6,
    CNML_STATUS_UNDERFLOWERR = 7,
    CNML_STATUS_INVALIDPARAM = 8,
    CNML_STATUS_BADALLOC     = 9,
    CNML_STATUS_BADTYPEID    = 10,
    CNML_STATUS_BADCAST      = 11,
    CNML_STATUS_UNSUPPORT    = 12
} cnmlStatus_t;

typedef enum {
    CNML_POOL_AVG = 0,
    CNML_POOL_MAX = 1,
    // L2,
    // None
} cnmlPoolMode_t;

typedef enum {
    CNML_UNPOOL = 0,
    CNML_ROWWISE_UNPOOL = 1,
    CNML_MAXPOOLBP = 2,
    CNML_AVGPOOLBP = 3,
    CNML_MIDUNPOOL = 4,
    CNML_DIV = 5,
    CNML_REP = 6
} cnmlUnpoolMode_t;

typedef enum {
    CNML_NoSparse = 0,
    CNML_Sparse   = 1,
} cnmlSparseMode_t;

typedef enum {
    CNML_SOFTMAX_DIM_C = 1,
    CNML_SOFTMAX_DIM_W = 2,
    CNML_SOFTMAX_DIM_H = 3,
    CNML_SOFTMAX_DIM_N = 4
} cnmlSoftmaxDim_t;

typedef enum {
    CNML_POOL_KFULL  = 0,
    CNML_POOL_KVALID = 1,
} cnmlPoolStrategyMode_t;

typedef enum {
    CNML_ACTIVE_NONE      = 0,
    CNML_ACTIVE_SIGMOID   = 1,
    CNML_ACTIVE_RELU      = 2,
    CNML_ACTIVE_TANH      = 3,
    CNML_ACTIVE_RELU1     = 4,
    CNML_ACTIVE_RELU6     = 5,
} cnmlActiveFunction_t;

typedef enum {
    CNML_REDUCEMAX_BATCH = 0,
    CNML_REDUCEMAX_FEAT  = 1,
    CNML_REDUCEMAX_HIGHT = 2,
    CNML_REDUCEMAX_WIDTH = 3,
} cnmlReduceMaxMode_t;

typedef enum {
    CNML_DIM_N = 0,
    CNML_DIM_C = 1,
    CNML_DIM_H = 2,
    CNML_DIM_W = 3,
} cnmlDimension_t;

typedef enum {
    CNML_CONCAT_FEAT  = 0,
    CNML_CONCAT_BATCH = 1,
    CNML_CONCAT_HIGHT = 2,
    CNML_CONCAT_WIDTH = 3,
} cnmlConcatMode_t;

typedef enum {
    CNML_SPLIT_FEAT  = 0,
    CNML_SPLIT_BATCH = 1,
    CNML_SPLIT_HIGHT = 2,
    CNML_SPLIT_WIDTH = 3,
} cnmlSplitMode_t;

typedef enum {
    CNML_TENSOR = 0,
    CNML_FILTER = 1,
    CNML_CONST  = 2,
} cnmlTensorType_t;

typedef enum {
    CNML_CAST_FLOAT32_TO_UINT8    = 0,
    CNML_CAST_UINT8_TO_FLOAT32    = 1,
    CNML_CAST_INT8_TO_FLOAT16     = 2,
    CNML_CAST_FIX8_TO_FLOAT16     = 3,
    CNML_CAST_FLOAT16_TO_FLOAT32  = 4,
    CNML_CAST_FLOAT16_TO_FIX8     = 5,
    CNML_CAST_FLOAT32_TO_FLOAT16  = 6,
    CNML_CAST_INT16_TO_FLOAT16    = 7,
    CNML_CAST_FLOAT16_TO_INT16    = 8,
} cnmlCastType_t;

typedef enum {
    CNML_ARGMAX_AXIS_N = 0,
    CNML_ARGMAX_AXIS_C = 1,
    CNML_ARGMAX_AXIS_H = 2,
    CNML_ARGMAX_AXIS_W = 3,
} cnmlArgmaxAxis_t;

typedef enum {
    CNML_REVERSE_AXIS_N,
    CNML_REVERSE_AXIS_C,
    CNML_REVERSE_AXIS_H,
    CNML_REVERSE_AXIS_W,
    CNML_REVERSE_AXIS_HW
} cnmlReverseAxis_t;
//////////////////////// common /////////////////////////

cnmlStatus_t cnmlInit(int flag);
cnmlStatus_t cnmlExit();

// cnml tensor
struct cnmlTensor;
typedef struct cnmlTensor *cnmlTensor_t;

cnmlStatus_t cnmlCreateTensor(cnmlTensor_t* tensor,
                            cnmlTensorType_t tensor_type,
                            cnmlDataType_t data_type,
                            int n,
                            int c,
                            int h,
                            int w);

void* cnmlMallocBuffer(cnmlTensor_t tensor);

void* cnmlMallocBatchBuffer(cnmlTensor_t tensor, int batch_size);

cnmlStatus_t cnmlFreeBuffer(void* ptr);

cnmlStatus_t cnmlDestroyTensor(cnmlTensor_t* tensor);

cnmlDataType_t cnmlGetTensorDataType(cnmlTensor_t tensor);

void cnmlGetTensorShape(cnmlTensor_t tensor, int* shape);

unsigned int cnmlGetTensorSize(cnmlTensor_t tensor, cnmlTensorType_t type);

cnmlStatus_t cnmlSetFix8Position(cnmlTensor_t tensor, int position);

cnmlStatus_t cnmlGetFix8Position(cnmlTensor_t tensor, int *position);

cnmlStatus_t cnmlSetQuant8Param(cnmlTensor_t tensor, float scale, float offset);

cnmlStatus_t cnmlGetQuant8Param(cnmlTensor_t tensor, float *scale,
                                float *offset);

cnmlStatus_t cnmlCloseSoftwareReshape(cnmlTensor_t tensor);

void cnmlPrintTensor(cnmlTensor_t tensor, cnmlTensorType_t type);

cnmlStatus_t cnmlSetWeightName(cnmlTensor_t tensor, char* name);

cnmlStatus_t cnmlGetWeightName(cnmlTensor_t tensor, char** name);

cnmlStatus_t cnmlUpdateWeight(cnmlTensor_t tensor);

// cpu tensor
struct cnmlCpuTensor;

typedef struct cnmlCpuTensor *cnmlCpuTensor_t;

cnmlStatus_t cnmlCreateCpuTensor(cnmlCpuTensor_t* cpu_tensor,
                               cnmlTensorType_t tensor_type,
                               cnmlDataType_t data_type,
                               cnmlDataOrder_t data_order,
                               int n,
                               int c,
                               int h,
                               int w);

cnmlStatus_t cnmlDestroyCpuTensor(cnmlCpuTensor_t* cpu_tensor);

cnmlStatus_t cnmlSetCpuTensorFix8Position(cnmlCpuTensor_t tensor, int position);

void cnmlPrintCpuTensor(cnmlCpuTensor_t tensor, cnmlTensorType_t type);

void cnmlDumpTensor2File(const char* filename, cnmlCpuTensor_t tensor,
        cnmlTensorType_t type, void *output_addr, bool app);

// cnml const tensor
cnmlStatus_t cnmlBindConstData(cnmlTensor_t tensor,
                               cnmlCpuTensor_t cpu_tensor,
                               void* cpu_tensor_ptr);

////////////////////////// operation /////////////////////////
/* base operation start */
struct cnmlBaseOp;
typedef struct cnmlBaseOp *cnmlBaseOp_t;

cnmlStatus_t cnmlGetMaxMemUsed(cnmlBaseOp_t op,
    int64_t* totalmem, int64_t* sharemem, int64_t* privatemem);

cnmlStatus_t cnmlSplitBaseOp(cnmlBaseOp_t op, int splitNum);

cnmlStatus_t cnmlCheckBaseOpRunnable(cnmlBaseOp_t op,
    cnmlCoreVersion_t version);

cnmlStatus_t cnmlCompileBaseOp(cnmlBaseOp_t op, cnmlCoreVersion_t version);

cnmlStatus_t cnmlDestroyBaseOp(cnmlBaseOp_t* op);

cnmlStatus_t cnmlSetFix8Mode(cnmlBaseOp_t op, bool fix8_mode);
/* base operation end */

/* fuse op start */
struct cnmlFusionOp;
typedef struct cnmlFusionOp *cnmlFusionOp_t;

cnmlStatus_t cnmlGetFusionMaxMemUsed(cnmlFusionOp_t op,
    int64_t* totalmem, int64_t* sharemem, int64_t* privatemem);

cnmlStatus_t cnmlFuseOp(cnmlBaseOp_t op, cnmlFusionOp_t fusionOp);

cnmlStatus_t cnmlSplitFusionOp(cnmlFusionOp_t op, int splitNum);

cnmlStatus_t cnmlCompileFusionOp(cnmlFusionOp_t op, cnmlCoreVersion_t version);

cnmlStatus_t cnmlComputeFusionOpForward(cnmlFusionOp_t op,
        void * inputs[], int inputNum, void * outputs[], int outputNum,
        cnrtFunctionType_t type, cnrtStream_t stream);

cnmlStatus_t cnmlCreateFusionOp(cnmlFusionOp_t* op);

cnmlStatus_t cnmlDestroyFusionOp(cnmlFusionOp_t* op);

cnmlStatus_t cnmlSetFusionIO(cnmlFusionOp_t op,
        cnmlTensor_t* inputs,
        int inputNum,
        cnmlTensor_t* outputs,
        int outputNum);

cnmlStatus_t cnmlAddFusionInput(cnmlFusionOp_t op, cnmlTensor_t input);

cnmlStatus_t cnmlAddFusionOutput(cnmlFusionOp_t op, cnmlTensor_t output);

/* fusion op end */

/* basic rnn operation begin */
cnmlStatus_t cnmlCreateBasicRNNOp(cnmlBaseOp_t *op,
          cnmlTensor_t inputTensor,
          cnmlTensor_t outputTensor,
          cnmlTensor_t weightTensor,
          cnmlTensor_t biasTensor,
          cnmlTensor_t stateInputTensor,
          cnmlTensor_t stateOutputTensor,
          cnmlTensor_t stateWeightTensor,
          cnmlActiveFunction_t active_func);

cnmlStatus_t cnmlComputeBasicRNNOpForward(cnmlBaseOp_t op,
        void *input,
        void *state,
        void *output,
        void *state_output,
        cnrtFunctionType_t type,
        cnrtStream_t stream);
/* basic rnn operation end 8*/

/* deconv operation begin */
struct cnmlDeconvOpParam;
typedef cnmlDeconvOpParam *cnmlDeconvOpParam_t;

struct cnmlDeconvOptLevel;
typedef cnmlDeconvOptLevel  *cnmlDeconvOptLevel_t;

cnmlStatus_t cnmlCreateDeconvOpParam(cnmlDeconvOpParam_t *param,
                int stride_height,
                int stride_width,
                int hu_crop,
                int hd_crop,
                int wl_crop,
                int wr_crop);
cnmlStatus_t cnmlDestroyDeconvOpParam(cnmlDeconvOpParam_t *param);

cnmlStatus_t cnmlCreateDeconvOptLevel(cnmlDeconvOptLevel_t *optlevel, int opt = 0);
cnmlStatus_t cnmlDestroyDeconvOptLevel(cnmlDeconvOptLevel_t *optlevel);

cnmlStatus_t cnmlCreateDeconvOp(cnmlBaseOp_t *op,
                cnmlDeconvOpParam_t param,
                cnmlTensor_t inputTensor,
                cnmlTensor_t outputTensor,
                cnmlTensor_t filterTensor,
                cnmlTensor_t biasTensor,
                cnmlDeconvOptLevel_t opt = nullptr);

cnmlStatus_t cnmlComputeDeConvOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream);


cnmlStatus_t cnmlEnableDeconvOpFix8Mode(cnmlBaseOp_t op);
/* deconv operation end */

/* conv operation begin */
struct cnmlConvOpParam;
typedef cnmlConvOpParam *cnmlConvOpParam_t;

cnmlStatus_t cnmlCreateConvOpParam(cnmlConvOpParam_t *param,
                int stride_height,
                int stride_width,
                int dilation_height,
                int dilation_width,
                int pad_height = 0,
                int pad_width = 0,
                cnmlSparseMode_t sparse = CNML_NoSparse);
cnmlStatus_t cnmlDestroyConvOpParam(cnmlConvOpParam_t *param);

cnmlStatus_t cnmlCreateConvOp(cnmlBaseOp_t *op,
                cnmlConvOpParam_t param,
                cnmlTensor_t inputTensor,
                cnmlTensor_t outputTensor,
                cnmlTensor_t filterTensor,
                cnmlTensor_t biasTensor);

cnmlStatus_t cnmlComputeConvOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream);

cnmlStatus_t cnmlEnableConvOpFix8Mode(cnmlBaseOp_t op);

/* conv operation end */

/* conv first operation begin */
struct cnmlConvFirstOpParam;
typedef cnmlConvFirstOpParam *cnmlConvFirstOpParam_t;

cnmlStatus_t cnmlCreateConvFirstOpParam(cnmlConvFirstOpParam_t *param,
                int stride_height, int stride_width,
                int pad_l, int pad_r, int pad_t, int pad_b,
                cnmlSparseMode_t sparse = CNML_NoSparse);
cnmlStatus_t cnmlDestroyConvFirstOpParam(cnmlConvFirstOpParam_t *param);

cnmlStatus_t cnmlCreateConvFirstOp(cnmlBaseOp_t *op,
                cnmlConvFirstOpParam_t param,
                cnmlTensor_t inputTensor,
                cnmlTensor_t meanTensor,
                cnmlTensor_t outputTensor,
                cnmlTensor_t filterTensor,
                cnmlTensor_t biasTensor = nullptr,
                cnmlTensor_t stdtTensor = nullptr);

cnmlStatus_t cnmlComputeConvFirstOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );

cnmlStatus_t cnmlEnableConvFirstOpFix8Mode(cnmlBaseOp_t op);

cnmlStatus_t cnmlEnableConvFirstOpBgraMode(cnmlBaseOp_t op);

cnmlStatus_t cnmlEnableConvFirstOpFusionPadMode(cnmlBaseOp_t op);

/* conv operation end */

/* conv group operation begin */
cnmlStatus_t cnmlCreateConvGroupOp(cnmlBaseOp_t *op,
                    cnmlConvOpParam_t param,
                    cnmlTensor_t inputTensor,
                    cnmlTensor_t outputTensor,
                    cnmlTensor_t filterTensor,
                    cnmlTensor_t biasTensor,
                    int group);

cnmlStatus_t cnmlComputeConvGroupOpForward(cnmlBaseOp_t op,
                    void *input,
                    void *output,
                    cnrtFunctionType_t type,
                    cnrtStream_t stream);
/* conv group operation end */

/* conv_depthwise operation begin */
struct cnmlConvDepthwiseOpParam;
typedef cnmlConvDepthwiseOpParam *cnmlConvDepthwiseOpParam_t;

cnmlStatus_t cnmlCreateConvDepthwiseOpParam(cnmlConvDepthwiseOpParam_t *param,
                int stride_height,
                int stride_width);

cnmlStatus_t cnmlDestroyConvDepthwiseOpParam(cnmlConvDepthwiseOpParam_t *param);

cnmlStatus_t cnmlCreateConvDepthwiseOp(cnmlBaseOp_t *op,
                cnmlConvDepthwiseOpParam_t param,
                cnmlTensor_t inputTensor,
                cnmlTensor_t outputTensor,
                cnmlTensor_t filterTensor,
                cnmlTensor_t biasTensor);

cnmlStatus_t cnmlComputeConvDepthwiseOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream);
/* conv_depthwise operation end */

/* add pad operation begin */
struct cnmlAddPadOpParam;
typedef struct cnmlAddPadOpParam *cnmlAddPadOpParam_t;

cnmlStatus_t cnmlCreateAddPadOpParam(cnmlAddPadOpParam_t* param,
                                 int pad_h, int pad_w, float pad_value);

cnmlStatus_t cnmlCreateAddPadOp4Param(cnmlAddPadOpParam_t* param,
                                 int pad_htop, int pad_hbottom,
                                 int pad_wleft, int pad_wright, float pad_value);

cnmlStatus_t cnmlDestroyAddPadOpParam(cnmlAddPadOpParam_t* param);

cnmlStatus_t cnmlCreateAddPadOp(cnmlBaseOp_t* op,
                              cnmlAddPadOpParam_t param,
                              cnmlTensor_t input_tensor,
                              cnmlTensor_t output_tensor);

cnmlStatus_t cnmlComputeAddPadOpForward(cnmlBaseOp_t op,
                               void *input,
                               void *output,
                               cnrtFunctionType_t type,
                               cnrtStream_t stream
                               );
/* add pad operation end */

/* pool operation begin */
struct cnmlPoolOpParam;
typedef cnmlPoolOpParam *cnmlPoolOpParam_t;

cnmlStatus_t cnmlCreatePoolOpParam(cnmlPoolOpParam_t *param,
                int window_height,
                int window_width,
                int stride_height,
                int stride_width,
                int pad_height,
                int pad_width,
                int dilation_height,
                int dilation_width,
                cnmlPoolMode_t pool_mode,
                cnmlPoolStrategyMode_t strategy_mode,
                bool real = false);

cnmlStatus_t cnmlDestroyPoolOpParam(cnmlPoolOpParam_t *param);

cnmlStatus_t cnmlGetPoolOpOutputDim(cnmlTensor_t input, cnmlPoolOpParam_t param,
                int* no, int* co, int* ho, int* wo);

cnmlStatus_t cnmlCreatePoolOp(cnmlBaseOp_t *op,
                cnmlPoolOpParam_t param,
                cnmlTensor_t input,
                cnmlTensor_t output);

cnmlStatus_t cnmlComputePoolOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );
/* pool operation end */

/* l2_pool operation begin */
cnmlStatus_t cnmlCreateL2_PoolOpParam(cnmlPoolOpParam_t *param,
                int window_height,
                int window_width,
                int stride_height,
                int stride_width,
                int pad_height,
                int pad_width,
                cnmlPoolMode_t pool_mode = CNML_POOL_AVG);

cnmlStatus_t cnmlDestroyL2_PoolOpParam(cnmlPoolOpParam_t *param);

cnmlStatus_t cnmlCreateL2_PoolOp(cnmlBaseOp_t *op,
                cnmlPoolOpParam_t param,
                cnmlTensor_t input,
                cnmlTensor_t output);

cnmlStatus_t cnmlComputeL2_PoolOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );
/* l2_pool operation end */

/* active operation begin */
cnmlStatus_t cnmlCreateActiveOp(cnmlBaseOp_t *op,
               cnmlActiveFunction_t function,
               cnmlTensor_t input,
               cnmlTensor_t output);

cnmlStatus_t cnmlComputeActiveOpForward(cnmlBaseOp_t op,
               void *input,
               void *output,
               cnrtFunctionType_t type,
               cnrtStream_t stream
               );
/* active operation end */

/* device memcpy operation begin */
cnmlStatus_t cnmlCreateDeviceMemcpyOp(cnmlBaseOp_t *op,
               cnmlTensor_t input,
               cnmlTensor_t output);

cnmlStatus_t cnmlComputeDeviceMemcpyOpForward(cnmlBaseOp_t op,
               void *input,
               void *output,
               cnrtFunctionType_t type,
               cnrtStream_t stream
               );
/* active operation end */

/* transpose pro operation begin */
struct cnmlTransposeOpParam;
typedef struct cnmlTransposeOpParam *cnmlTransposeOpParam_t;

cnmlStatus_t cnmlCreateTransposeOpParam(cnmlTransposeOpParam_t* param,
                cnmlDataOrder_t cpuDataOrder,
                int dimId0, int dimId1, int dimId2, int dimId3);

cnmlStatus_t cnmlDestroyTransposeOpParam(cnmlTransposeOpParam_t* param);

cnmlStatus_t cnmlCreateTransposeProOp(cnmlBaseOp_t *op,
               cnmlTensor_t input,
               cnmlTensor_t output,
               cnmlTransposeOpParam_t param);

cnmlStatus_t cnmlComputeTransposeProOpForward(cnmlBaseOp_t op,
               void *input,
               void *output,
               cnrtFunctionType_t type,
               cnrtStream_t stream
               );

/* transpose pro operation end */

/* transpose operation begin */
cnmlStatus_t cnmlCreateTransposeOp(cnmlBaseOp_t *op,
               cnmlTensor_t input,
               cnmlTensor_t output);

cnmlStatus_t cnmlComputeTransposeOpForward(cnmlBaseOp_t op,
               void *input,
               void *output,
               cnrtFunctionType_t type,
               cnrtStream_t stream
               );
/* transpose operation end */

/* mlp operation begin */
cnmlStatus_t cnmlCreateMlpOp(cnmlBaseOp_t* op,
              cnmlTensor_t inputTensor,
              cnmlTensor_t outputTensor,
              cnmlTensor_t filterTensor,
              cnmlTensor_t biasTensor,
              cnmlSparseMode_t sparse = CNML_NoSparse);

cnmlStatus_t cnmlComputeMlpOpForward(cnmlBaseOp_t op,
       void *inputs, void *outputs,
       cnrtFunctionType_t type,
       cnrtStream_t stream
       );

cnmlStatus_t cnmlEnableMlpOpFix8Mode(cnmlBaseOp_t op);

cnmlStatus_t cnmlEnableMlpOpBinaryMode(cnmlBaseOp_t op);

/* mlp operation end */

/* xw_plus_b operation begin */
cnmlStatus_t cnmlCreateXwPlusBOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputTensor,
               cnmlTensor_t outputTensor,
               cnmlTensor_t filterTensor,
               cnmlTensor_t biasTensor,
               cnmlSparseMode_t sparse = CNML_NoSparse);

cnmlStatus_t cnmlComputeXwPlusBOpForward(cnmlBaseOp_t op,
                                   void *input,
                                   void *output,
                                   cnrtFunctionType_t type,
                                   cnrtStream_t stream
                                   );

cnmlStatus_t cnmlEnableXwPlusBOpFix8Mode(cnmlBaseOp_t op);

/* xw_plus_b operation end */

/* add operation begin */
cnmlStatus_t cnmlComputeAddOpForward(cnmlBaseOp_t op,
                              void *inputA, void *inputB, void *output,
                              cnrtFunctionType_t type,
                              cnrtStream_t stream
                              );

cnmlStatus_t cnmlCreateAddOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputATensor,
               cnmlTensor_t inputBTensor,
               cnmlTensor_t outputTensor);
/* add operation end */

/* real div operation begin */
cnmlStatus_t cnmlComputeRealDivOpForward(cnmlBaseOp_t op,
                              void *inputA, void *inputB, void *output,
                              cnrtFunctionType_t type,
                              cnrtStream_t stream
                              );

cnmlStatus_t cnmlCreateRealDivOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputATensor,
               cnmlTensor_t inputBTensor,
               cnmlTensor_t outputTensor);
/* real div operation end */

/* broadcast sub operation begin */
cnmlStatus_t cnmlComputeBroadcastSubOpForward(cnmlBaseOp_t op,
        void *inputA, void *inputB, void *output,
        cnrtFunctionType_t type,
        cnrtStream_t stream);

cnmlStatus_t cnmlCreateBroadcastSubOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputATensor,
               cnmlTensor_t inputBTensor,
               cnmlTensor_t outputTensor);
/* broadcast sub operation end */

/* broadcast add operation begin */
cnmlStatus_t cnmlComputeBroadcastAddOpForward(cnmlBaseOp_t op,
        void *inputA, void *inputB, void *output,
        cnrtFunctionType_t type,
        cnrtStream_t stream
        );

cnmlStatus_t cnmlCreateBroadcastAddOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputATensor,
               cnmlTensor_t inputBTensor,
               cnmlTensor_t outputTensor);
/* broadcast add operation end */

/* broadcast mult operation begin */
cnmlStatus_t cnmlComputeBroadcastMultOpForward(cnmlBaseOp_t op,
        void *inputA, void *inputB, void *output,
        cnrtFunctionType_t type,
        cnrtStream_t stream
        );

cnmlStatus_t cnmlCreateBroadcastMultOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputATensor,
               cnmlTensor_t inputBTensor,
               cnmlTensor_t outputTensor);
/* broadcast mult operation end */

/* sub operation begin */
cnmlStatus_t cnmlComputeSubOpForward(cnmlBaseOp_t op,
                              void *inputA, void *inputB, void *output,
                              cnrtFunctionType_t type,
                              cnrtStream_t stream
                              );

cnmlStatus_t cnmlCreateSubOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputATensor,
               cnmlTensor_t inputBTensor,
               cnmlTensor_t outputTensor);
/* sub operation end */

/* mult operation begin */
cnmlStatus_t cnmlComputeMultOpForward(cnmlBaseOp_t op,
        void *inputA, void *inputB, void *output,
        cnrtFunctionType_t type,
        cnrtStream_t stream
        );

cnmlStatus_t cnmlCreateMultOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputATensor,
               cnmlTensor_t inputBTensor,
               cnmlTensor_t outputTensor);
/* mult operation end */

/* lrn operation start */
struct cnmlLrnOpParam;
typedef struct cnmlLrnOpParam* cnmlLrnOpParam_t;

typedef enum {
  CNML_LRN_V1,  // xi * pow(k + alpha*sum/local_size, -beta), for caffe
  CNML_LRN_V2,  // xi * pow(k + alpha*sum, -beta), for Android NN
} cnmlLrnType_t;

cnmlStatus_t cnmlCreateLrnOpParam(cnmlLrnOpParam_t* param,
                              cnmlLrnType_t type,
                              int local_size,
                              double alpha,
                              double beta,
                              double k);

cnmlStatus_t cnmlDestroyLrnOpParam(cnmlLrnOpParam_t* param);

cnmlStatus_t cnmlCreateLrnOp(cnmlBaseOp_t* op,
                           cnmlLrnOpParam_t param,
                           cnmlTensor_t input,
                           cnmlTensor_t output);

cnmlStatus_t cnmlComputeLrnOpForward(cnmlBaseOp_t op,
                                 void* input,
                                 void* output,
                                 cnrtFunctionType_t type,
                                 cnrtStream_t stream
                                 );

cnmlStatus_t cnmlEnableLrnOpFix8Mode(cnmlBaseOp_t op,
        int inputDataFix8Position, float offset);
/*  lrn operation end  */

/* batch_norm operation start */
cnmlStatus_t cnmlCreateBatchNormOp(cnmlBaseOp_t* op,
                                 cnmlTensor_t input,
                                 cnmlTensor_t output,
                                 cnmlTensor_t mean,
                                 cnmlTensor_t var);

cnmlStatus_t cnmlComputeBatchNormOpForward(cnmlBaseOp_t op,
                                      void *input,
                                      void *output,
                                      cnrtFunctionType_t type,
                                      cnrtStream_t stream
                                      );
/* batch_norm operation end */

/* max operation start */
cnmlStatus_t cnmlCreateMaxOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output,
                             cnmlTensor_t index);

cnmlStatus_t cnmlComputeMaxOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     void *index,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream
                                     );
/* max operation end */

/* reduce max operation start */
cnmlStatus_t cnmlCreateReduceMaxOp(cnmlBaseOp_t* op,
                             cnmlReduceMaxMode_t mode,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeReduceMaxOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream
                                     );
/* reduce max operation end */


/* min operation start */
cnmlStatus_t cnmlCreateMinOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output,
                             cnmlTensor_t index);

cnmlStatus_t cnmlComputeMinOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     void *index,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream
                                     );
/* min operation end */

/* reverse operation start */
cnmlStatus_t cnmlCreateReverseOp(cnmlBaseOp_t* op,
                                 cnmlTensor_t input,
                                 cnmlTensor_t output,
                                 cnmlReverseAxis_t reverse_axis);

cnmlStatus_t cnmlComputeReverseOpForward(cnmlBaseOp_t op,
                                         void *input,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* reverse operation end */

/* transpose operation start */
cnmlStatus_t cnmlCreateTransposeOp(cnmlBaseOp_t* op,
                                 cnmlTensor_t input,
                                 cnmlTensor_t output);

cnmlStatus_t cnmlComputeTransposeOpForward(cnmlBaseOp_t op,
                                         void *input,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream);
/* transepose operation end */

/* resize operation start */
struct cnmlResizeOpParam;
typedef struct cnmlResizeOpParam* cnmlResizeOpParam_t;

cnmlStatus_t cnmlCreateResizeOpParam(cnmlResizeOpParam_t *param,
                                     float height1,
                                     float weight1,
                                     float height2,
                                     float weight2);


cnmlStatus_t cnmlDestroyResizeOpParam(cnmlResizeOpParam_t* param);

cnmlStatus_t cnmlCreateResizeOp(cnmlBaseOp_t* op,
                                cnmlTensor_t input,
                                cnmlTensor_t output,
                                cnmlResizeOpParam_t param);

cnmlStatus_t cnmlComputeResizeOpForward(cnmlBaseOp_t op,
                                        void *input,
                                        void *output,
                                        cnrtFunctionType_t type,
                                        cnrtStream_t stream);
/* resize operation end */


/* interp operation start */
struct cnmlInterpOpParam;
typedef struct cnmlInterpOpParam* cnmlInterpOpParam_t;

cnmlStatus_t cnmlCreateInterpOpParam(cnmlInterpOpParam_t *param,
                                     int output_width,
                                     int output_height,
                                     bool align_corners);

cnmlStatus_t cnmlCreateInterpOpParamByRatio(cnmlInterpOpParam_t *param,
                                            float zoom,
                                            bool align_corners);

cnmlStatus_t cnmlDestroyInterpOpParam(cnmlInterpOpParam_t* param);

cnmlStatus_t cnmlCreateInterpOp(cnmlBaseOp_t* op,
                                cnmlTensor_t input,
                                cnmlTensor_t output,
                                cnmlInterpOpParam_t param);

cnmlStatus_t cnmlComputeInterpOpForward(cnmlBaseOp_t op,
                                        void *input,
                                        void *output,
                                        cnrtFunctionType_t type,
                                        cnrtStream_t stream
                                        );
/* interp operation end */

/* scale operation start */
cnmlStatus_t cnmlCreateScaleOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output,
                             cnmlTensor_t alpha,
                             cnmlTensor_t beta);

cnmlStatus_t cnmlComputeScaleOpForward(cnmlBaseOp_t op,
                                   void* input,
                                   void* output,
                                   cnrtFunctionType_t type,
                                   cnrtStream_t stream);

cnmlStatus_t cnmlComputeScaleOpForwardUltra(cnmlBaseOp_t op,
                                   void* input,
                                   void* alpha,
                                   void* beta,
                                   void* output,
                                   cnrtFunctionType_t type,
                                   cnrtStream_t stream);
/*  scale operation end  */

/* concat operation start */
struct cnmlConcatOpParam;
typedef struct cnmlConcatOpParam* cnmlConcatOpParam_t;

cnmlStatus_t cnmlCreateConcatOpParam(cnmlConcatOpParam_t* param,
                                 int input_num,
                                 int output_num,
                                 cnmlConcatMode_t mode);

cnmlStatus_t cnmlDestroyConcatOpParam(cnmlConcatOpParam_t* param);

cnmlStatus_t cnmlCreateConcatOp(cnmlBaseOp_t* op,
                              cnmlConcatOpParam_t param,
                              cnmlTensor_t* inputs,
                              int input_num,
                              cnmlTensor_t* outputs,
                              int output_num);

cnmlStatus_t cnmlComputeConcatOpForward(cnmlBaseOp_t op,
                                    void* inputs[],
                                    int input_num,
                                    void* outputs[],
                                    int output_num,
                                    cnrtFunctionType_t type,
                                    cnrtStream_t stream
                                    );
/*  concat operation end  */

/* slice operation start */
struct cnmlSplitOpParam;
typedef struct cnmlSplitOpParam* cnmlSplitOpParam_t;

cnmlStatus_t cnmlCreateSplitOpParam(cnmlSplitOpParam_t* param,
                                 int input_num,
                                 int output_num,
                                 cnmlSplitMode_t mode);

cnmlStatus_t cnmlDestroySplitOpParam(cnmlSplitOpParam_t* param);

cnmlStatus_t cnmlCreateSplitOp(cnmlBaseOp_t* op,
                              cnmlSplitOpParam_t param,
                              cnmlTensor_t* inputs,
                              int input_num,
                              cnmlTensor_t* outputs,
                              int output_num);

cnmlStatus_t cnmlComputeSplitOpForward(cnmlBaseOp_t op,
                                    void* inputs[],
                                    int input_num,
                                    void* outputs[],
                                    int output_num,
                                    cnrtFunctionType_t type,
                                    cnrtStream_t stream
                                    );
/* slice operation end  */

/* shuffle channel start */
cnmlStatus_t cnmlCreateShuffleChannelOp(cnmlBaseOp_t* op,
                              cnmlTensor_t* inputs,
                              cnmlTensor_t* outputs,
                              int group);

cnmlStatus_t cnmlComputeShuffleChannelOpForward(cnmlBaseOp_t op,
                                    void *inputs[],
                                    void *outputs[],
                                    cnrtFunctionType_t type,
                                    cnrtStream_t stream);
/* shuffle channel operation end  */

/* not operation start */
cnmlStatus_t cnmlCreateNotOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensor,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeNotOpForward(cnmlBaseOp_t op,
                                         void *input,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* not operation end */

/* blockshift operation start */
cnmlStatus_t cnmlCreateBlockShiftOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeBlockShiftOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* blockshift operation end */

/* and operation start */
cnmlStatus_t cnmlCreateAndOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeAndOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* and operation end */

/* cycleand operation start */
cnmlStatus_t cnmlCreateCycleAndOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleAndOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* cycleand operation end */

/* cyclexor operation start */
cnmlStatus_t cnmlCreateCycleXorOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleXorOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* cyclexor operation end */

/* xor operation start */
cnmlStatus_t cnmlCreateXorOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeXorOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* xor operation end */

/* cycleor operation start */
cnmlStatus_t cnmlCreateCycleOrOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleOrOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* cycleor operation end */

/* or operation start */
cnmlStatus_t cnmlCreateOrOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeOrOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* or operation end */

/* cycleadd operation start */
cnmlStatus_t cnmlCreateCycleAddOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleAddOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* cycleadd operation end */

/* cyclesub operation start */
cnmlStatus_t cnmlCreateCycleSubOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleSubOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* cyclesyb operation end */

/* cyclemult operation end */
cnmlStatus_t cnmlCreateCycleMultOp(cnmlBaseOp_t *op,
                                  cnmlTensor_t inputTensorA,
                                  cnmlTensor_t inputTensorB,
                                  cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleMultOpForward(cnmlBaseOp_t op,
                                          void *inputA,
                                          void *inputB,
                                          void *output,
                                          cnrtFunctionType_t type,
                                          cnrtStream_t stream
                                          );
/* cyclemult operation end */

/* cycle equal operation start */
cnmlStatus_t cnmlCreateCycleEqualOp(cnmlBaseOp_t *op,
                                    cnmlTensor_t inputTensorA,
                                    cnmlTensor_t inputTensorB,
                                    cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleEqualOpForward(cnmlBaseOp_t op,
                                            void *inputA,
                                            void *inputB,
                                            void *output,
                                            cnrtFunctionType_t type,
                                            cnrtStream_t stream
                                            );
/* cycle equal operation end */

/* cycle n equal operation start */
cnmlStatus_t cnmlCreateCycleNEqualOp(cnmlBaseOp_t *op,
                                    cnmlTensor_t inputTensorA,
                                    cnmlTensor_t inputTensorB,
                                    cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleNEqualOpForward(cnmlBaseOp_t op,
                                            void *inputA,
                                            void *inputB,
                                            void *output,
                                            cnrtFunctionType_t type,
                                            cnrtStream_t stream
                                            );
/* cycle n equal operation end */

/* cycle less equal operation start */
cnmlStatus_t cnmlCreateCycleLessEqualOp(cnmlBaseOp_t *op,
                                        cnmlTensor_t inputTensorA,
                                        cnmlTensor_t inputTensorB,
                                        cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleLessEqualOpForward(cnmlBaseOp_t op,
                                                void *inputA,
                                                void *inputB,
                                                void *output,
                                                cnrtFunctionType_t type,
                                                cnrtStream_t stream
                                                );
/* cycle less equal operation end */

/* cycle less operation start */
cnmlStatus_t cnmlCreateCycleLessOp(cnmlBaseOp_t *op,
                                   cnmlTensor_t inputTensorA,
                                   cnmlTensor_t inputTensorB,
                                   cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleLessOpForward(cnmlBaseOp_t op,
                                           void *inputA,
                                           void *inputB,
                                           void *output,
                                           cnrtFunctionType_t type,
                                           cnrtStream_t stream
                                           );
/* cycle less operation end */

/* cycle greater equal operation start */
cnmlStatus_t cnmlCreateCycleGreaterEqualOp(cnmlBaseOp_t *op,
                                           cnmlTensor_t inputTensorA,
                                           cnmlTensor_t inputTensorB,
                                           cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleGreaterEqualOpForward(cnmlBaseOp_t op,
                                                   void *inputA,
                                                   void *inputB,
                                                   void *output,
                                                   cnrtFunctionType_t type,
                                                   cnrtStream_t stream
                                                   );
/* cycle greater equal operation end */

/* cycle greater operation start */
cnmlStatus_t cnmlCreateCycleGreaterOp(cnmlBaseOp_t *op,
                                      cnmlTensor_t inputTensorA,
                                      cnmlTensor_t inputTensorB,
                                      cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeCycleGreaterOpForward(cnmlBaseOp_t op,
                                              void *inputA,
                                              void *inputB,
                                              void *output,
                                              cnrtFunctionType_t type,
                                              cnrtStream_t stream
                                              );
/* cycle greater operation end */

/* equal operation start */
cnmlStatus_t cnmlCreateEqualOp(cnmlBaseOp_t *op,
                               cnmlTensor_t inputTensorA,
                               cnmlTensor_t inputTensorB,
                               cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeEqualOpForward(cnmlBaseOp_t op,
                                       void *inputA,
                                       void *inputB,
                                       void *output,
                                       cnrtFunctionType_t type,
                                       cnrtStream_t stream
                                       );
/* equal operation end */

/* not equal operation start */
cnmlStatus_t cnmlCreateNEqualOp(cnmlBaseOp_t *op,
                                cnmlTensor_t inputTensorA,
                                cnmlTensor_t inputTensorB,
                                cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeNEqualOpForward(cnmlBaseOp_t op,
                                        void *inputA,
                                        void *inputB,
                                        void *output,
                                        cnrtFunctionType_t type,
                                        cnrtStream_t stream
                                        );
/* not equal operation end */

/* less equal operation start */
cnmlStatus_t cnmlCreateLessEqualOp(cnmlBaseOp_t *op,
                                   cnmlTensor_t inputTensorA,
                                   cnmlTensor_t inputTensorB,
                                   cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeLessEqualOpForward(cnmlBaseOp_t op,
                                           void *inputA,
                                           void *inputB,
                                           void *output,
                                           cnrtFunctionType_t type,
                                           cnrtStream_t stream
                                           );
/* less equal operation end */

/* less operation start */
cnmlStatus_t cnmlCreateLessOp(cnmlBaseOp_t *op,
                              cnmlTensor_t inputTensorA,
                              cnmlTensor_t inputTensorB,
                              cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeLessOpForward(cnmlBaseOp_t op,
                                      void *inputA,
                                      void *inputB,
                                      void *output,
                                      cnrtFunctionType_t type,
                                      cnrtStream_t stream
                                      );
/* less operation end */

/* greater equal operation start */
cnmlStatus_t cnmlCreateGreaterEqualOp(cnmlBaseOp_t *op,
                                      cnmlTensor_t inputTensorA,
                                      cnmlTensor_t inputTensorB,
                                      cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeGreaterEqualOpForward(cnmlBaseOp_t op,
                                              void *inputA,
                                              void *inputB,
                                              void *output,
                                              cnrtFunctionType_t type,
                                              cnrtStream_t stream
                                              );
/* greater equal operation end */

/* greater operation start */
cnmlStatus_t cnmlCreateGreaterOp(cnmlBaseOp_t *op,
                                 cnmlTensor_t inputTensorA,
                                 cnmlTensor_t inputTensorB,
                                 cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeGreaterOpForward(cnmlBaseOp_t op,
                                         void *inputA,
                                         void *inputB,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* greater operation end */

/* maxtt operation start */
cnmlStatus_t cnmlCreateMaxTTOp(cnmlBaseOp_t* op,
                                   cnmlTensor_t inputA,
                                   cnmlTensor_t inputB,
                                   cnmlTensor_t output);

cnmlStatus_t cnmlComputeMaxTTOpForward(cnmlBaseOp_t op,
                                           void *inputA,
                                           void *inputB,
                                           void *output,
                                           cnrtFunctionType_t type,
                                           cnrtStream_t stream
                                           );
/* maxtt operation end */

/* maxtc operation start */
cnmlStatus_t cnmlCreateMaxTCOp(cnmlBaseOp_t* op,
                               cnmlTensor_t input,
                               cnmlTensor_t output,
                               float cc);

cnmlStatus_t cnmlComputeMaxTCOpForward(cnmlBaseOp_t op,
                                       void *input,
                                       void *output,
                                       cnrtFunctionType_t type,
                                       cnrtStream_t stream
                                       );
/* maxtc operation end */

/* mintc operation start */
cnmlStatus_t cnmlCreateMinTCOp(cnmlBaseOp_t* op,
                               cnmlTensor_t input,
                               cnmlTensor_t output,
                               float cc);

cnmlStatus_t cnmlComputeMinTCOpForward(cnmlBaseOp_t op,
                                       void *input,
                                       void *output,
                                       cnrtFunctionType_t type,
                                       cnrtStream_t stream
                                       );
/* mintc operation end */

/* clip operation start */
cnmlStatus_t cnmlCreateClipOp(cnmlBaseOp_t* op,
                              cnmlTensor_t inputTensor,
                              cnmlTensor_t outputTensor,
                              double lower_bound,
                              double upper_bound);

cnmlStatus_t cnmlComputeClipOpForward(cnmlBaseOp_t op,
                                      void *input,
                                      void *output,
                                      cnrtFunctionType_t type,
                                      cnrtStream_t stream
                                      );
/* clip operation end */

/* lstm operation begin */
cnmlStatus_t cnmlCreateLstmOp(cnmlBaseOp_t* op,
               cnmlTensor_t input,
               cnmlTensor_t cell_in,
               cnmlTensor_t recurrent,
               cnmlTensor_t cell_out,
               cnmlTensor_t output,
               cnmlTensor_t i2ifilter,
               cnmlTensor_t i2ffilter,
               cnmlTensor_t i2cfilter,
               cnmlTensor_t i2ofilter,
               cnmlTensor_t r2ifilter,
               cnmlTensor_t r2ffilter,
               cnmlTensor_t r2cfilter,
               cnmlTensor_t r2ofilter,
               cnmlTensor_t c2ifilter,
               cnmlTensor_t c2ffilter,
               cnmlTensor_t c2ofilter,
               cnmlTensor_t projectfilter,
               cnmlTensor_t inputbias,
               cnmlTensor_t forgetbias,
               cnmlTensor_t cellbias,
               cnmlTensor_t outputbias,
               cnmlTensor_t projectbias,
               bool use_cifg,
               bool use_peehole,
               bool use_project,
               bool use_project_bias,
               float cell_clip,
               float project_clip,
               cnmlActiveFunction_t fn);

cnmlStatus_t cnmlComputeLstmOpForward(cnmlBaseOp_t op,
                                   void *input,
                                   void *cell_in,
                                   void *recurrent,
                                   void *cell_out,
                                   void *output,
                                   cnrtFunctionType_t type,
                                   cnrtStream_t stream
                                   );

/* lstm operation end */

/* nearest_neighbor operation start */
struct cnmlNearestNeighborOpParam;
typedef struct cnmlNearestNeighborOpParam* cnmlNearestNeighborOpParam_t;

cnmlStatus_t cnmlCreateNearestNeighborOpParam(
                 cnmlNearestNeighborOpParam_t *param,
                 int output_width,
                 int output_height);

cnmlStatus_t cnmlCreateNearestNeighborOpParamByRatio(
                 cnmlNearestNeighborOpParam_t *param,
                 int zoom);

cnmlStatus_t cnmlDestroyNearestNeighborOpParam(
                 cnmlNearestNeighborOpParam_t* param);

cnmlStatus_t cnmlCreateNearestNeighborOp(cnmlBaseOp_t* op,
                                cnmlTensor_t input,
                                cnmlTensor_t output,
                                cnmlNearestNeighborOpParam_t param);

cnmlStatus_t cnmlComputeNearestNeighborOpForward(cnmlBaseOp_t op,
                                        void *input,
                                        void *output,
                                        cnrtFunctionType_t type,
                                        cnrtStream_t stream
                                        );
/* nearest_neighbor operation end */

/* prelu operation start */
cnmlStatus_t cnmlCreatePreluOp(cnmlBaseOp_t* op,
                                 cnmlTensor_t input,
                                 cnmlTensor_t output,
                                 cnmlTensor_t prelu_param);

cnmlStatus_t cnmlComputePreluOpForward(cnmlBaseOp_t op,
                                      void *input,
                                      void *output,
                                      cnrtFunctionType_t type,
                                      cnrtStream_t stream
                                      );
/* prelu operation end */

/* sqrt operation start */
cnmlStatus_t cnmlCreateSqrtOp(cnmlBaseOp_t* op,
                              cnmlTensor_t input,
                              cnmlTensor_t output);

cnmlStatus_t cnmlComputeSqrtOpForward(cnmlBaseOp_t op,
                                      void *input,
                                      void *output,
                                      cnrtFunctionType_t type,
                                      cnrtStream_t stream
                                      );
/* sqrt operation end */

/* Rsqrt operation start */
cnmlStatus_t cnmlCreateRsqrtOp(cnmlBaseOp_t* op,
                               cnmlTensor_t input,
                               cnmlTensor_t output);

cnmlStatus_t cnmlComputeRsqrtOpForward(cnmlBaseOp_t op,
                                       void *input,
                                       void *output,
                                       cnrtFunctionType_t type,
                                       cnrtStream_t stream
                                       );
/* rsqrt operation end */

/* exp operation start */
cnmlStatus_t cnmlCreateExpOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeExpOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream
                                     );
/* exp operation end */

/* softmax operation start */
cnmlStatus_t cnmlCreateSoftmaxOp(cnmlBaseOp_t* op,
                                 cnmlSoftmaxDim_t dim,
                                 cnmlTensor_t input,
                                 cnmlTensor_t output);

cnmlStatus_t cnmlComputeSoftmaxOpForward(cnmlBaseOp_t op,
                                         void *input,
                                         void *output,
                                         cnrtFunctionType_t type,
                                         cnrtStream_t stream
                                         );
/* softmax operation end */

/* log operation start */
cnmlStatus_t cnmlCreateLogOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeLogOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream
                                     );
/* log operation end */

/* floor operation begin */
cnmlStatus_t cnmlCreateFloorOp(cnmlBaseOp_t *op,
               cnmlTensor_t input,
               cnmlTensor_t output);

cnmlStatus_t cnmlComputeFloorOpForward(cnmlBaseOp_t op,
               void *input,
               void *output,
               cnrtFunctionType_t type,
               cnrtStream_t stream
               );
/* floor operation end */

/* power operation start */
cnmlStatus_t cnmlCreatePowerOp(cnmlBaseOp_t* op,
                                 cnmlTensor_t input,
                                 cnmlTensor_t output,
                                 float power_c);

cnmlStatus_t cnmlComputePowerOpForward(cnmlBaseOp_t op,
                                      void *input,
                                      void *output,
                                      cnrtFunctionType_t type,
                                      cnrtStream_t stream
                                      );
/* power operation end */

/* unarySelect operation begin */
cnmlStatus_t cnmlCreateUnarySelectOp(cnmlBaseOp_t *op,
                cnmlTensor_t inputTensor1,
                cnmlTensor_t inputTensor2,
                cnmlTensor_t count_cnml,
                cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeUnarySelectOpForward(cnmlBaseOp_t op,
                void *input1,
                void *input2,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );

/* dyadicSelect operation end */

/* dyadicSelect operation begin */
cnmlStatus_t cnmlCreateDyadicSelectOp(cnmlBaseOp_t *op,
                cnmlTensor_t inputTensor1,
                cnmlTensor_t inputTensor2,
                cnmlTensor_t inputTensor3,
                cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeDyadicSelectOpForward(cnmlBaseOp_t op,
                void *input1,
                void *input2,
                void *input3,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );

/* dyadicSelect operation end */

/* svdf operation begin */
cnmlStatus_t cnmlCreateSvdfOp(cnmlBaseOp_t* op,
               cnmlTensor_t inputTensor,
               cnmlTensor_t weightsFeature,
               cnmlTensor_t weightsTime,
               cnmlTensor_t biasTensor,
               cnmlTensor_t stateIn,
               cnmlTensor_t stateOut,
               cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeSvdfOpForward(cnmlBaseOp_t op,
                                   void *input,
                                   void *stateIn,
                                   void *stateOut,
                                   void *output,
                                   cnrtFunctionType_t type,
                                   cnrtStream_t stream);
/* svdf operation end */

/* abs operation start */
cnmlStatus_t cnmlCreateAbsOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeAbsOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream);
/* abs operation end */

/* softplus operation start */
cnmlStatus_t cnmlCreateSoftplusOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeSoftplusOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream);
/* softplus operation end */

/* minus operation start */
cnmlStatus_t cnmlCreateMinusOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeMinusOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream);
/* minus operation end */

/* fakequant operation start */
cnmlStatus_t cnmlCreateFakeQuantOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output,
                             float scale,
                             int offset);

cnmlStatus_t cnmlComputeFakeQuantOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream);
/* fakequant operation end */

/* avg operation start */
cnmlStatus_t cnmlCreateAvgOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeAvgOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream
                                     );
/* avg operation end */

/* vector2norm operation start */
cnmlStatus_t cnmlCreateVector2NormOp(cnmlBaseOp_t* op,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeVector2NormOpForward(cnmlBaseOp_t op,
                                     void *input,
                                     void *output,
                                     cnrtFunctionType_t type,
                                     cnrtStream_t stream
                                     );
/* vector2norm operation end */

/* cast operation begin */
cnmlStatus_t cnmlCreateCastOp(cnmlBaseOp_t *op,
               cnmlCastType_t cast_type,
               cnmlTensor_t input,
               cnmlTensor_t output);

cnmlStatus_t cnmlComputeCastOpForward(cnmlBaseOp_t op,
               void *input,
               void *output,
               cnrtFunctionType_t type,
               cnrtStream_t stream
               );

void cnmlDumpCastTensor2File(const char* filename, cnmlCpuTensor_t tensor,
        cnmlTensorType_t type, void *output_addr, bool app);
/* cast operation end */

/* proposal operation begin */
struct cnmlProposalOpParam;
typedef cnmlProposalOpParam *cnmlProposalOpParam_t;

cnmlStatus_t cnmlCreateProposalOpParam(cnmlProposalOpParam_t *param,
                 int H, int W, int A, int im_h, int im_w,
                 int min_h, int min_w, int out_size,
                 float filter_scale, float nms_thresh, float nms_scale,
                 int feat_stride = 32);

cnmlStatus_t cnmlDestroyProposalOpParam(cnmlProposalOpParam_t *param);

cnmlStatus_t cnmlCreateProposalOp(cnmlBaseOp_t* op,
                                  cnmlProposalOpParam_t param,
                                  cnmlTensor_t bbox_deltas,
                                  cnmlTensor_t scores,
                                  cnmlTensor_t output);

cnmlStatus_t cnmlComputeProposalOpForward(cnmlBaseOp_t op,
                void *bbox_deltas, void* scores,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );
/* proposal operation end */

/* nms operation begin */
struct cnmlNmsOpParam;
typedef cnmlNmsOpParam *cnmlNmsOpParam_t;

cnmlStatus_t cnmlCreateNmsOpParam(cnmlNmsOpParam_t *param,
                                  int box_size, int out_size,
                                  float nms_thresh, float nms_scale,
                                  float score_thresh,
                                  bool filter_scores, bool normalized_bbox);

cnmlStatus_t cnmlDestroyNmsOpParam(cnmlNmsOpParam_t *param);

cnmlStatus_t cnmlCreateNmsOp(cnmlBaseOp_t* op,
                             cnmlNmsOpParam_t param,
                             cnmlTensor_t input,
                             cnmlTensor_t output);

cnmlStatus_t cnmlComputeNmsOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );
/* nms operation end */

/* roi pooling operation begin */
struct cnmlRoiPoolOpParam;
typedef struct cnmlRoiPoolOpParam *cnmlRoiPoolOpParam_t;

cnmlStatus_t cnmlCreateRoiPoolOpParam(cnmlRoiPoolOpParam_t *param,
        int pooled_h,
        int pooled_w,
        float spatial_scale);
cnmlStatus_t cnmlDestroyRoiPoolOpParam(cnmlRoiPoolOpParam_t *param);

cnmlStatus_t cnmlCreateRoiPoolOp(cnmlBaseOp_t* op,
               cnmlRoiPoolOpParam_t param,
               cnmlTensor_t inputTensor,
               cnmlTensor_t rois,
               cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputeRoiPoolOpForward(cnmlBaseOp_t op,
                                   void *input,
                                   void *rois,
                                   void *output,
                                   cnrtFunctionType_t type,
                                   cnrtStream_t stream
                                   );
/* roi pooling operation end */

/* image_detect operation begin */
struct cnmlImageDetectOpParam;
typedef cnmlImageDetectOpParam *cnmlImageDetectOpParam_t;

cnmlStatus_t cnmlCreateImageDetectOpParam(cnmlImageDetectOpParam_t *param,
                int num_class, int box_size_per_class, int im_h, int im_w,
                float score_thresh, float nms_thresh, float nms_scale);

cnmlStatus_t cnmlDestroyImageDetectOpParam(cnmlImageDetectOpParam_t *param);

cnmlStatus_t cnmlCreateImageDetectOp(cnmlBaseOp_t* op,
                                  cnmlImageDetectOpParam_t param,
                                  cnmlTensor_t bbox_deltas,
                                  cnmlTensor_t scores,
                                  cnmlTensor_t anchors,
                                  cnmlTensor_t output);

cnmlStatus_t cnmlComputeImageDetectOpForward(cnmlBaseOp_t op,
                void *bbox_deltas, void* scores, void* anchors,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );

/* image_detect operation end */

/* grep operation begin */
struct cnmlGrepOpParam;
typedef struct cnmlGrepOpParam *cnmlGrepOpParam_t;

cnmlStatus_t cnmlCreateGrepOpParam(cnmlGrepOpParam_t* param,
                 int startIndexOfN, int startIndexOfH, int startIndexOfW,
                 float space_number);

cnmlStatus_t cnmlDestroyGrepOpParam(cnmlGrepOpParam_t* param);

cnmlStatus_t cnmlCreateGrepOp(cnmlBaseOp_t* op,
                              cnmlGrepOpParam_t param,
                              cnmlTensor_t input_tensor,
                              cnmlTensor_t output_tensor);

cnmlStatus_t cnmlComputeGrepOpForward(cnmlBaseOp_t op,
                               void *input,
                               void *output,
                               cnrtFunctionType_t type,
                               cnrtStream_t stream
                               );
/* grep operation end */

/* argmax operation begin */
cnmlStatus_t cnmlGetArgmaxOpOutputDim(cnmlArgmaxAxis_t argmax_axis,
    int, int, int, int, int *, int *, int *, int *);

cnmlStatus_t cnmlCreateArgmaxOp(cnmlBaseOp_t *op,
                cnmlArgmaxAxis_t argmax_mode,
                cnmlTensor_t input,
                cnmlTensor_t output);

cnmlStatus_t cnmlComputeArgmaxOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );
/* argmax operation end */

/* reorg operation begin */
struct cnmlReorgOpParam;
typedef struct cnmlReorgOpParam *cnmlReorgOpParam_t;

cnmlStatus_t cnmlGetReorgOpOutputDim(int reorg_h, int reorg_w,
    int, int, int, int, int*, int*, int*, int*);

cnmlStatus_t cnmlCreateReorgOpParam(cnmlReorgOpParam_t* param,
                                 int reorg_h, int reorg_w);

cnmlStatus_t cnmlDestroyReorgOpParam(cnmlReorgOpParam_t* param);

cnmlStatus_t cnmlCreateReorgOp(cnmlBaseOp_t* op,
                              cnmlReorgOpParam_t param,
                              cnmlTensor_t input_tensor,
                              cnmlTensor_t output_tensor);

cnmlStatus_t cnmlComputeReorgOpForward(cnmlBaseOp_t op,
                               void *input,
                               void *output,
                               cnrtFunctionType_t type,
                               cnrtStream_t stream
                               );
/* reorg operation end */

/* yuv_to_rgb operation start */
cnmlStatus_t cnmlCreateYUVtoRGBOp(cnmlBaseOp_t* op,
                            cnmlTensor_t input,
                            cnmlTensor_t output);

cnmlStatus_t cnmlComputeYUVtoRGBOpForward(cnmlBaseOp_t op,
                                    void *input,
                                    void *output,
                                    cnrtFunctionType_t type,
                                    cnrtStream_t stream
                                    );
/* log operation end */

/* reshape operation begin */
struct cnmlReshapeOpParam;
typedef cnmlReshapeOpParam *cnmlReshapeOpParam_t;
/*
 * when create cpu tensor, the DataOrder is up to you.
 * when create mlu tensor, the DataOrder always is NHWC.
 * no, co, ho, wo is the output tensor shape.
 * df in reshape param is the same as cpu tensor DataOrder.
 */
cnmlStatus_t cnmlCreateReshapeOpParam(cnmlReshapeOpParam_t *param,
        int no,
        int co,
        int ho,
        int wo,
        cnmlDataOrder_t df);
cnmlStatus_t cnmlDestroyReshapeOpParam(cnmlReshapeOpParam_t *param);


cnmlStatus_t cnmlCreateReshapeOp(cnmlBaseOp_t *op,
                cnmlReshapeOpParam_t param,
                cnmlTensor_t input,
                cnmlTensor_t output);

cnmlStatus_t cnmlComputeReshapeOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );
/* reshape operation end */

/* space2batch operation begin */
cnmlStatus_t cnmlCreateSpace2batchOp(cnmlBaseOp_t *op,
                int w_block_size,
                int h_block_size,
                cnmlTensor_t input,
                cnmlTensor_t output);

cnmlStatus_t cnmlComputeSpace2batchOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );
/* space2batch operation end */

/* batch2space operation begin */
cnmlStatus_t cnmlCreateBatch2spaceOp(cnmlBaseOp_t *op,
                int w_block_size,
                int h_block_size,
                cnmlTensor_t input,
                cnmlTensor_t output);

cnmlStatus_t cnmlComputeBatch2spaceOpForward(cnmlBaseOp_t op,
                void *input,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream
                );
/* batch2space operation end */

/* unpool operation start */
struct cnmlUnpoolOpParam;
typedef cnmlUnpoolOpParam *cnmlUnpoolOpParam_t;

cnmlStatus_t cnmlCreateUnpoolOpParam(cnmlUnpoolOpParam_t *param,
                int window_height,
                int window_width,
                int stride_height,
                int stride_width,
                cnmlUnpoolMode_t unpool_mode);

cnmlStatus_t cnmlDestroyUnpoolOpParam(cnmlUnpoolOpParam_t *param);

cnmlStatus_t cnmlCreateUnpoolOp(cnmlBaseOp_t* op,
                                cnmlTensor_t input,
                                cnmlTensor_t index,
                                cnmlTensor_t output,
                                cnmlUnpoolOpParam_t unpool_param);

cnmlStatus_t cnmlComputeUnpoolOpForward(cnmlBaseOp_t op,
                                           void *input,
                                           void *index,
                                           void *output,
                                           cnrtFunctionType_t type,
                                           cnrtStream_t stream
                                           );
/* unpool operation end */

/* top_k operation begin */
cnmlStatus_t cnmlCreateTopkOp(cnmlBaseOp_t* op,
                              int k,
                              cnmlTensor_t input,
                              cnmlTensor_t output,
                              cnmlTensor_t index);

cnmlStatus_t cnmlComputeTopkOpForward(cnmlBaseOp_t op,
                                      void* input,
                                      void* output,
                                      void* index,
                                      cnrtFunctionType_t type,
                                      cnrtStream_t stream
                                      );
/* top_k operation end */

/* where operation begin */
cnmlStatus_t cnmlCreateWhereOp(cnmlBaseOp_t* op,
        cnmlTensor_t input, cnmlTensor_t output, cnmlTensor_t count);

cnmlStatus_t cnmlComputeWhereOpForward(cnmlBaseOp_t op,
        void* input, void* output, void* count,
        cnrtFunctionType_t type, cnrtStream_t stream);
/* where operation end */

/* ssd_detection_output operation begin */
struct cnmlSsdDetectionOutputOpParam;
typedef cnmlSsdDetectionOutputOpParam* cnmlSsdDetectionOutputOpParam_t;

cnmlStatus_t
cnmlCreateSsdDetectionOutputOpParam(cnmlSsdDetectionOutputOpParam_t* param,
                                    int num_classes,
                                    bool share_location,
                                    int background_label_id,
                                    int code_type,
                                    bool variance_encoded_in_target,
                                    float confidence_threshold,
                                    float nms_threshold,
                                    int nms_topk,
                                    int keep_topk,
                                    cnmlDataOrder_t input_layout);

cnmlStatus_t
cnmlDestroySsdDetectionOutputOpParam(cnmlSsdDetectionOutputOpParam_t* param);

cnmlStatus_t
cnmlCreateSsdDetectionOutputOp(cnmlBaseOp_t* op,
                               cnmlTensor_t pred_loc,
                               cnmlTensor_t conf,
                               cnmlTensor_t priors,
                               cnmlTensor_t output,
                               cnmlSsdDetectionOutputOpParam_t param);

cnmlStatus_t
cnmlComputeSsdDetectionOutputOpForward(cnmlBaseOp_t op,
                                       void* pred_loc,
                                       void* conf,
                                       void* priors,
                                       void* output,
                                       cnrtFunctionType_t type,
                                       cnrtStream_t stream);
/* ssd_detection_output operation end */

/* ssd_detection operation begin */
struct cnmlSsdDetectionOpParam;
typedef cnmlSsdDetectionOpParam* cnmlSsdDetectionOpParam_t;

cnmlStatus_t
cnmlCreateSsdDetectionOpParam(cnmlSsdDetectionOpParam_t* param,
                              int num_classes,
                              bool share_location,
                              int background_label_id,
                              int code_type,
                              bool variance_encoded_in_target,
                              float confidence_threshold,
                              float nms_threshold,
                              int nms_topk,
                              int keep_topk,
                              cnmlDataOrder_t input_layout);

cnmlStatus_t
cnmlDestroySsdDetectionOpParam(cnmlSsdDetectionOpParam_t *param);

cnmlStatus_t cnmlCreateSsdDetectionOp(cnmlBaseOp_t* op,
                                      cnmlTensor_t loc_a,
                                      cnmlTensor_t loc_b,
                                      cnmlTensor_t loc_c,
                                      cnmlTensor_t loc_d,
                                      cnmlTensor_t loc_e,
                                      cnmlTensor_t loc_f,
                                      cnmlTensor_t conf_a,
                                      cnmlTensor_t conf_b,
                                      cnmlTensor_t conf_c,
                                      cnmlTensor_t conf_d,
                                      cnmlTensor_t conf_e,
                                      cnmlTensor_t conf_f,
                                      cnmlTensor_t priors,
                                      cnmlTensor_t output,
                                      cnmlSsdDetectionOpParam_t param);

cnmlStatus_t cnmlComputeSsdDetectionOpForward(cnmlBaseOp_t op,
                                              void* loc_a,
                                              void* loc_b,
                                              void* loc_c,
                                              void* loc_d,
                                              void* loc_e,
                                              void* loc_f,
                                              void* conf_a,
                                              void* conf_b,
                                              void* conf_c,
                                              void* conf_d,
                                              void* conf_e,
                                              void* conf_f,
                                              void* priors,
                                              void* output,
                                              cnrtFunctionType_t type,
                                              cnrtStream_t stream);
/* ssd_detection operation end */

/* roialign operation begin */
struct cnmlRoiAlignOpParam;
typedef cnmlRoiAlignOpParam *cnmlRoiAlignOpParam_t;

cnmlStatus_t cnmlCreateRoiAlignOpParam(cnmlRoiAlignOpParam_t *param,
                int pooled_w,
                int pooled_h,
                float scale,
                float sampling_ratio);

cnmlStatus_t cnmlDestroyRoiAlignOpParam(cnmlRoiAlignOpParam_t *param);

cnmlStatus_t cnmlCreateRoiAlignOp(cnmlBaseOp_t *op,
                cnmlRoiAlignOpParam_t param,
                cnmlTensor_t input,
                cnmlTensor_t rois,
                cnmlTensor_t output);

cnmlStatus_t cnmlComputeRoiAlignOpForward(cnmlBaseOp_t op,
                void *input,
                void *rois,
                void *output,
                cnrtFunctionType_t type,
                cnrtStream_t stream);
/* roialign operation end */

/* Fractional pool operation begin */
struct cnmlFractionalPoolOpParam;
typedef cnmlFractionalPoolOpParam* cnmlFractionalPoolOpParam_t;

cnmlStatus_t cnmlCreateFractionalPoolOpParam(cnmlFractionalPoolOpParam_t *param,
                               cnmlPoolMode_t mode,
                               int* row_sequence,
                               int row_num,
                               int* col_sequence,
                               int col_num,
                               bool overlapping);

cnmlStatus_t cnmlDestroyFractionalPoolOpParam(cnmlFractionalPoolOpParam_t *param);

cnmlStatus_t cnmlCreateFractionalPoolOp(cnmlBaseOp_t *op,
                               cnmlFractionalPoolOpParam_t param,
                               cnmlTensor_t input,
                               cnmlTensor_t output);

cnmlStatus_t cnmlComputeFractionalPoolOpForward(cnmlBaseOp_t op,
                               void *input,
                               void *output,
                               cnrtFunctionType_t type,
                               cnrtStream_t stream);
/* Fractional pool operation end */

/* Log Softmax operation begin */
cnmlStatus_t cnmlCreateLogSoftmaxOp(cnmlBaseOp_t *op,
                                cnmlTensor_t input,
                                cnmlTensor_t output,
                                cnmlDimension_t dim);

cnmlStatus_t cnmlComputeLogSoftmaxOpForward(cnmlBaseOp_t op,
                               void *input,
                               void *output,
                               cnrtFunctionType_t type,
                               cnrtStream_t stream);
/* Log Softmax operation end */

/* roi pooling operation begin */
struct cnmliPsRoiPoolOpParam;
typedef struct cnmlPsRoiPoolOpParam *cnmlPsRoiPoolOpParam_t;

cnmlStatus_t cnmlCreatePsRoiPoolOpParam(cnmlPsRoiPoolOpParam_t *param,
        int pooled_w,
        int pooled_h,
        float spatial_scale,
        int roi_num,
        int output_dim);
cnmlStatus_t cnmlDestroyPsRoiPoolOpParam(cnmlPsRoiPoolOpParam_t *param);

cnmlStatus_t cnmlCreatePsRoiPoolOp(cnmlBaseOp_t* op,
               cnmlPsRoiPoolOpParam_t param,
               cnmlTensor_t inputTensor,
               cnmlTensor_t rois,
               cnmlTensor_t outputTensor);

cnmlStatus_t cnmlComputePsRoiPoolOpForward(cnmlBaseOp_t op,
                                   void *input,
                                   void *rois,
                                   void *output,
                                   cnrtFunctionType_t type,
                                   cnrtStream_t stream);
/* roi pooling operation end */
/* Strided Slice operation begin */
struct cnmlStridedSliceOpParam;
typedef cnmlStridedSliceOpParam* cnmlStridedSliceOpParam_t;

cnmlStatus_t cnmlCreateStridedSliceOpParam(cnmlStridedSliceOpParam_t *param,
                                   int nb, int cb, int hb, int wb,
                                   int ne, int ce, int he, int we,
                                   int ns, int cs, int hs, int ws);

cnmlStatus_t cnmlDestroyStridedSliceOpParam(cnmlStridedSliceOpParam_t *param);

cnmlStatus_t cnmlCreateStridedSliceOp(cnmlBaseOp_t *op,
                                cnmlStridedSliceOpParam_t param,
                                cnmlTensor_t input,
                                cnmlTensor_t output);

cnmlStatus_t cnmlComputeStridedSliceOpForward(cnmlBaseOp_t op,
                               void *input,
                               void *output,
                               cnrtFunctionType_t type,
                               cnrtStream_t stream);
/* Strided Slice operation end */

/* Yolo Detect operation begin */
struct cnmlYoloDetectOpParam;
typedef cnmlYoloDetectOpParam* cnmlYoloDetectOpParam_t;

cnmlStatus_t cnmlCreateYoloDetectOpParam(cnmlYoloDetectOpParam_t *param,
                           int side, int num_class,
                           int num_box, float confidence_threshold,
                           float nms_thresh, float* biases, int bias_num);

cnmlStatus_t cnmlDestroyYoloDetectOpParam(cnmlYoloDetectOpParam_t *param);

cnmlStatus_t cnmlCreateYoloDetectOp(cnmlBaseOp_t *op,
                                cnmlYoloDetectOpParam_t param,
                                cnmlTensor_t input,
                                cnmlTensor_t output);

cnmlStatus_t cnmlComputeYoloDetectOpForward(cnmlBaseOp_t op,
                               void *input,
                               void *output,
                               cnrtFunctionType_t type,
                               cnrtStream_t stream);
/* Yolo Detect operation end */

////////////////////////////////// copy functions ///////////////////////////////
cnmlStatus_t cnmlMemcpyTensorToDevice(cnmlCpuTensor_t cpu_tensor,
        void *src, cnmlTensor_t cnml_tensor, void *dst);

cnmlStatus_t cnmlMemcpyTensorToHost(cnmlTensor_t cnml_tensor,
        void *src, cnmlCpuTensor_t cpu_tensor, void *dst);

cnmlStatus_t cnmlMemcpyBatchTensorToDevice(cnmlCpuTensor_t cpu_tensor,
        void *src, cnmlTensor_t cnml_tensor, void *dst, int batch_size);

cnmlStatus_t cnmlMemcpyBatchTensorToHost(cnmlTensor_t cnml_tensor,
        void *src, cnmlCpuTensor_t cpu_tensor, void *dst, int batch_size);

////////////////////// cpu operation /////////////////////
cnmlStatus_t cnmlCpuComputePoolOpForward(cnmlPoolOpParam_t param,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output);

cnmlStatus_t cnmlCpuComputeL2_PoolOpForward(cnmlPoolOpParam_t param,
                                            cnmlCpuTensor_t input_tensor,
                                            void *input,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeRoiAlignOpForward(cnmlRoiAlignOpParam_t param,
                                             cnmlCpuTensor_t input_tensor,
                                             void *input,
                                             cnmlCpuTensor_t rois_tensor,
                                             void *rois,
                                             cnmlCpuTensor_t output_tensor,
                                             void *output);

cnmlStatus_t cnmlCpuComputePsRoiPoolOpForward(cnmlPsRoiPoolOpParam_t param,
                                             cnmlCpuTensor_t input_tensor,
                                             void *input,
                                             cnmlCpuTensor_t rois_tensor,
                                             void *rois,
                                             cnmlCpuTensor_t output_tensor,
                                             void *output);

cnmlStatus_t cnmlCpuComputeAddPadOpForward(cnmlAddPadOpParam_t param,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output);

cnmlStatus_t cnmlCpuComputeScaleOpForward(cnmlCpuTensor_t input_tensor,
                              void * input,
                              cnmlCpuTensor_t output_tensor,
                              void * output,
                              cnmlCpuTensor_t alpha_tensor,
                              void * alpha,
                              cnmlCpuTensor_t beta_tensor,
                              void * beta);

cnmlStatus_t cnmlCpuComputeSubOpForward(cnmlCpuTensor_t input_tensor_1,
             void * input_1,
             cnmlCpuTensor_t input_tensor_2,
             void * input_2,
             cnmlCpuTensor_t output_tensor,
             void * output);

cnmlStatus_t cnmlCpuComputeMultOpForward(cnmlCpuTensor_t input_tensor_1,
             void * input_1,
             cnmlCpuTensor_t input_tensor_2,
             void * input_2,
             cnmlCpuTensor_t output_tensor,
             void * output);


cnmlStatus_t cnmlCpuComputeSplitOpForward(cnmlSplitOpParam_t param,
                                            cnmlCpuTensor_t* inputs_tensor_ptr,
                                            void* inputs[],
                                            int input_num,
                                            cnmlCpuTensor_t* outputs_tensor_ptr,
                                            void* outputs[],
                                            int output_num);
cnmlStatus_t cnmlCpuComputeShuffleChannelOpForward(cnmlCpuTensor_t* inputs_tensor_ptr,
                                            void* inputs[],
                                            cnmlCpuTensor_t* outputs_tensor_ptr,
                                            void* outputs[],
                                            int group);

cnmlStatus_t cnmlCpuComputeConcatOpForward(cnmlConcatOpParam_t param,
                                           cnmlCpuTensor_t* inputs_tensor_ptr,
                                           void* inputs[],
                                           int input_num,
                                           cnmlCpuTensor_t* outputs_tensor_ptr,
                                           void* outputs[],
                                           int output_num);

cnmlStatus_t cnmlCpuComputeAddOpForward(cnmlCpuTensor_t input_tensor_1,
             void * input_1,
             cnmlCpuTensor_t input_tensor_2,
             void * input_2,
             cnmlCpuTensor_t output_tensor,
             void * output);

cnmlStatus_t cnmlCpuComputeRealDivOpForward(cnmlCpuTensor_t input_tensor_1,
             void * input_1,
             cnmlCpuTensor_t input_tensor_2,
             void * input_2,
             cnmlCpuTensor_t output_tensor,
             void * output);

cnmlStatus_t cnmlCpuComputeBroadcastAddOpForward(cnmlCpuTensor_t input_tensor_1,
             void * input_1,
             cnmlCpuTensor_t input_tensor_2,
             void * input_2,
             cnmlCpuTensor_t output_tensor,
             void * output);

cnmlStatus_t cnmlCpuComputeBroadcastSubOpForward(cnmlCpuTensor_t input_tensor_1,
             void * input_1,
             cnmlCpuTensor_t input_tensor_2,
             void * input_2,
             cnmlCpuTensor_t output_tensor,
             void * output);

cnmlStatus_t cnmlCpuComputeBroadcastMultOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void * input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void * input_2,
                 cnmlCpuTensor_t output_tensor,
                 void * output);

cnmlStatus_t cnmlCpuComputeActiveOpForward(cnmlActiveFunction_t fn,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output);

cnmlStatus_t cnmlCpuComputeMlpOpForward(cnmlCpuTensor_t input_tensor,
                                        void *input,
                                        cnmlCpuTensor_t output_tensor,
                                        void *output,
                                        cnmlCpuTensor_t filter_tensor,
                                        void *filter,
                                        cnmlCpuTensor_t bias_tensor,
                                        void *bias);

cnmlStatus_t cnmlCpuComputeXwPlusBOpForward(cnmlCpuTensor_t input_tensor,
                                        void *input,
                                        cnmlCpuTensor_t output_tensor,
                                        void *output,
                                        cnmlCpuTensor_t filter_tensor,
                                        void *filter,
                                        cnmlCpuTensor_t bias_tensor,
                                        void *bias);

cnmlStatus_t cnmlCpuComputeBasicRNNOpForward(cnmlCpuTensor_t input_tensor,
                 void *input,
                 cnmlCpuTensor_t output_tensor,
                 void *output,
                 cnmlCpuTensor_t weight_tensor,
                 void *weight,
                 cnmlCpuTensor_t state_input_tensor,
                 void *state_input,
                 cnmlCpuTensor_t state_output_tensor,
                 void *state_output,
                 cnmlCpuTensor_t state_weight_tensor,
                 void *state_weight,
                 cnmlCpuTensor_t bias_tensor,
                 void *bias,
                 cnmlActiveFunction_t active_func);


cnmlStatus_t cnmlCpuComputeConvGroupOpForward(cnmlConvOpParam_t param,
                                             cnmlCpuTensor_t input_tensor,
                                             void *input,
                                             cnmlCpuTensor_t filter_tensor,
                                             void *filter,
                                             cnmlCpuTensor_t output_tensor,
                                             void *output,
                                             cnmlCpuTensor_t bias_tensor,
                                             void *bias,
                                             int group);

cnmlStatus_t cnmlCpuComputeDeconvOpForward(cnmlDeconvOpParam_t param,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t filter_tensor,
                                         void *filter,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output,
                                         cnmlCpuTensor_t bias_tensor,
                                         void *bias);

cnmlStatus_t cnmlCpuComputeConvOpForward(cnmlConvOpParam_t param,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t filter_tensor,
                                         void *filter,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output,
                                         cnmlCpuTensor_t bias_tensor,
                                         void *bias);

cnmlStatus_t cnmlCpuComputeConvFirstOpForward(cnmlConvFirstOpParam_t param,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t mean_tensor,
                                         void *mean,
                                         cnmlCpuTensor_t filter_tensor,
                                         void *filter,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output,
                                         cnmlCpuTensor_t bias_tensor,
                                         void *bias,
                                         cnmlCpuTensor_t stdt_tensor = nullptr,
                                         void *stdt = nullptr);

cnmlStatus_t cnmlCpuComputeConvDepthwiseOpForward(
                 cnmlConvDepthwiseOpParam_t param,
                 cnmlCpuTensor_t input_tensor,
                 void *input,
                 cnmlCpuTensor_t filter_tensor,
                 void *filter,
                 cnmlCpuTensor_t output_tensor,
                 void *output,
                 cnmlCpuTensor_t bias_tensor,
                 void *bias);

cnmlStatus_t cnmlCpuComputeBatchNormOpForward(cnmlCpuTensor_t input_tensor,
                                         void * input_buf,
                                         cnmlCpuTensor_t output_tensor,
                                         void * output_buf,
                                         cnmlCpuTensor_t mean_tensor,
                                         void * mean_buf,
                                         cnmlCpuTensor_t var_tensor,
                                         void * var_buf);

cnmlStatus_t cnmlCpuComputeOrOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeCycleOrOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeAndOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeCycleAndOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeXorOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeCycleXorOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeNotOpForward(cnmlCpuTensor_t input_tensor,
                                            void *input,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);


cnmlStatus_t cnmlCpuComputeCycleAddOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeCycleSubOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeCycleMultOpForward(cnmlCpuTensor_t input_tensor_1,
                                             void *input_1,
                                             cnmlCpuTensor_t input_tensor_2,
                                             void *input_2,
                                             cnmlCpuTensor_t output_tensor,
                                             void *output);

cnmlStatus_t cnmlCpuComputeMaxOpForward(cnmlCpuTensor_t input_tensor,
                                        void *input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void *output_buf,
                                        cnmlCpuTensor_t index_tensor,
                                        void *index_buf);

cnmlStatus_t cnmlCpuComputeReduceMaxOpForward(cnmlReduceMaxMode_t mode,
                                        cnmlCpuTensor_t input_tensor,
                                        void *input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void *output_buf);


cnmlStatus_t cnmlCpuComputeMinOpForward(cnmlCpuTensor_t input_tensor,
                                        void *input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void *output_buf,
                                        cnmlCpuTensor_t index_tensor,
                                        void *index_buf);

cnmlStatus_t cnmlCpuComputeReverseOpForward(cnmlCpuTensor_t input_tensor,
                                            void *input_buf,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output_buf,
                                            cnmlReverseAxis_t param);

cnmlStatus_t cnmlCpuComputeTransposeOpForward(cnmlCpuTensor_t input_tensor,
                                            void *input_buf,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output_buf);

cnmlStatus_t cnmlCpuComputeTransposeProOpForward(cnmlCpuTensor_t input_tensor,
                                            void *input_buf,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output_buf,
                                            cnmlTransposeOpParam_t param);

cnmlStatus_t cnmlCpuComputeInterpOpForward(cnmlCpuTensor_t input_tensor,
                                           void *input_buf,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output_buf,
                                           cnmlInterpOpParam_t param);

cnmlStatus_t cnmlCpuComputeResizeOpForward(cnmlCpuTensor_t input_tensor,
                                           void *input_buf,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output_buf,
                                           cnmlResizeOpParam_t param);

cnmlStatus_t cnmlCpuComputeLrnOpForward(cnmlLrnOpParam_t param,
                                        cnmlCpuTensor_t input_tensor,
                                        void *input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void *output_buf);

cnmlStatus_t cnmlCpuComputeMaxTTOpForward(cnmlCpuTensor_t inputA_tensor,
                                              void *inputA_buf,
                                              cnmlCpuTensor_t inputB_tensor,
                                              void *inputB_buf,
                                              cnmlCpuTensor_t output_tensor,
                                              void *output_buf);

cnmlStatus_t cnmlCpuComputeUnpoolOpForward(cnmlUnpoolOpParam_t unpool_param,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input_buf,
                                           cnmlCpuTensor_t index_tensor,
                                           void *index_buf,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output_buf);

cnmlStatus_t cnmlCpuComputeMaxTCOpForward(cnmlCpuTensor_t inputA_tensor,
                                          void *inputA_buf,
                                          cnmlCpuTensor_t output_tensor,
                                          void *output_buf,
                                          float cc);

cnmlStatus_t cnmlCpuComputeMinTCOpForward(cnmlCpuTensor_t input_tensor,
                                          void *input_buf,
                                          cnmlCpuTensor_t output_tensor,
                                          void *output_buf,
                                          float cc);

cnmlStatus_t cnmlCpuComputeCycleEqualOpForward(cnmlCpuTensor_t input_tensor_1,
                                               void *input_1,
                                               cnmlCpuTensor_t input_tensor_2,
                                               void *input_2,
                                               cnmlCpuTensor_t output_tensor,
                                               void *output);

cnmlStatus_t cnmlCpuComputeCycleNEqualOpForward(cnmlCpuTensor_t input_tensor_1,
                                               void *input_1,
                                               cnmlCpuTensor_t input_tensor_2,
                                               void *input_2,
                                               cnmlCpuTensor_t output_tensor,
                                               void *output);

cnmlStatus_t cnmlCpuComputeCycleLessEqualOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeCycleLessOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeCycleGreaterOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeCycleGreaterEqualOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeEqualOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeNEqualOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeLessEqualOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeLessOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeGreaterEqualOpForward(
                 cnmlCpuTensor_t input_tensor_1,
                 void *input_1,
                 cnmlCpuTensor_t input_tensor_2,
                 void *input_2,
                 cnmlCpuTensor_t output_tensor,
                 void *output);

cnmlStatus_t cnmlCpuComputeGreaterOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);

cnmlStatus_t cnmlCpuComputeClipOpForward(cnmlCpuTensor_t input_tensor,
                                         void *input_buf,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output_buf,
                                         double lower_bound,
                                         double upper_bound);

cnmlStatus_t cnmlCpuComputeLstmOpForward(cnmlCpuTensor_t input,
                                    void * input_ptr,
                                    cnmlCpuTensor_t cell_in,
                                    void * cell_in_ptr,
                                    cnmlCpuTensor_t recurrent,
                                    void * recurrent_ptr,
                                    cnmlCpuTensor_t cell_out,
                                    void * cell_out_ptr,
                                    cnmlCpuTensor_t output,
                                    void * output_ptr,
                                    cnmlCpuTensor_t i2ifilter,
                                    void * i2ifilter_ptr,
                                    cnmlCpuTensor_t i2ffilter,
                                    void * i2ffilter_ptr,
                                    cnmlCpuTensor_t i2cfilter,
                                    void * i2cfilter_ptr,
                                    cnmlCpuTensor_t i2ofilter,
                                    void * i2ofilter_ptr,
                                    cnmlCpuTensor_t r2ifilter,
                                    void * r2ifilter_ptr,
                                    cnmlCpuTensor_t r2ffilter,
                                    void * r2ffilter_ptr,
                                    cnmlCpuTensor_t r2cfilter,
                                    void * r2cfilter_ptr,
                                    cnmlCpuTensor_t r2ofilter,
                                    void * r2ofilter_ptr,
                                    cnmlCpuTensor_t c2ifilter,
                                    void * c2ifilter_ptr,
                                    cnmlCpuTensor_t c2ffilter,
                                    void * c2ffilter_ptr,
                                    cnmlCpuTensor_t c2ofilter,
                                    void * c2ofilter_ptr,
                                    cnmlCpuTensor_t projectfilter,
                                    void * projectfilter_ptr,
                                    cnmlCpuTensor_t inputbias,
                                    void * inputbias_ptr,
                                    cnmlCpuTensor_t forgetbias,
                                    void * forgetbias_ptr,
                                    cnmlCpuTensor_t cellbias,
                                    void * cellbias_ptr,
                                    cnmlCpuTensor_t outputbias,
                                    void * outputbias_ptr,
                                    cnmlCpuTensor_t projectbias,
                                    void * projectbias_ptr,
                                    bool use_cifg,
                                    bool use_peehole,
                                    bool use_project,
                                    bool use_project_bias,
                                    float cell_clip,
                                    float project_clip,
                                    cnmlActiveFunction_t fn);

cnmlStatus_t cnmlCpuComputeNearestNeighborOpForward(
                 cnmlCpuTensor_t input_tensor,
                 void *input_buf,
                 cnmlCpuTensor_t output_tensor,
                 void *output_buf,
                 cnmlNearestNeighborOpParam_t param);

cnmlStatus_t cnmlCpuComputePreluOpForward(cnmlCpuTensor_t input_tensor,
                                         void * input_buf,
                                         cnmlCpuTensor_t output_tensor,
                                         void * output_buf,
                                         cnmlCpuTensor_t prelu_param_tensor,
                                         void * prelu_param_buf);

cnmlStatus_t cnmlCpuComputeSqrtOpForward(cnmlCpuTensor_t input_tensor,
                                         void * input_buf,
                                         cnmlCpuTensor_t output_tensor,
                                         void * output_buf);

cnmlStatus_t cnmlCpuComputeExpOpForward(cnmlCpuTensor_t input_tensor,
                                         void * input_buf,
                                         cnmlCpuTensor_t output_tensor,
                                         void * output_buf);

cnmlStatus_t cnmlCpuComputeLogOpForward(cnmlCpuTensor_t input_tensor,
                                         void * input_buf,
                                         cnmlCpuTensor_t output_tensor,
                                         void * output_buf);

cnmlStatus_t cnmlCpuComputeRsqrtOpForward(cnmlCpuTensor_t input_tensor,
                                          void * input_buf,
                                          cnmlCpuTensor_t output_tensor,
                                          void * output_buf);

cnmlStatus_t cnmlCpuComputeFloorOpForward(cnmlCpuTensor_t input_tensor,
                                          void *input,
                                          cnmlCpuTensor_t output_tensor,
                                          void *output);

cnmlStatus_t cnmlCpuComputePowerOpForward(cnmlCpuTensor_t input_tensor,
                                         void * input_buf,
                                         cnmlCpuTensor_t output_tensor,
                                         void * output_buf,
                                         float power_c);

cnmlStatus_t cnmlCpuComputeBlockShiftOpForward(cnmlCpuTensor_t input_tensor_1,
                                            void *input_1,
                                            cnmlCpuTensor_t input_tensor_2,
                                            void *input_2,
                                            cnmlCpuTensor_t output_tensor,
                                            void *output);
cnmlStatus_t cnmlCpuComputeDyadicSelectOpForward(cnmlCpuTensor_t input_tensor_1,
             void * input_1,
             cnmlCpuTensor_t input_tensor_2,
             void * input_2,
             cnmlCpuTensor_t input_tensor_3,
             void * input_3,
             cnmlCpuTensor_t output_tensor,
             void * output);

cnmlStatus_t cnmlCpuComputeUnarySelectOpForward(cnmlCpuTensor_t input_tensor_1,
             void * input_1,
             cnmlCpuTensor_t input_tensor_2,
             void * input_2,
             cnmlCpuTensor_t count_cpu,
             void * count_cpu_ptr,
             cnmlCpuTensor_t output_tensor,
             void * output);

cnmlStatus_t cnmlCpuComputeSvdfOpForward(
    cnmlCpuTensor_t inputTensor,
    void *input_ptr,
    cnmlCpuTensor_t weightsFeature,
    void *weights_feature_ptr,
    cnmlCpuTensor_t weightsTime,
    void *weights_time_ptr,
    cnmlCpuTensor_t biasTensor,
    void *bias_ptr,
    cnmlCpuTensor_t stateIn,
    void *state_in_ptr,
    cnmlCpuTensor_t stateOut,
    void *state_out_ptr,
    cnmlCpuTensor_t outputTensor,
    void *output_ptr);

cnmlStatus_t cnmlCpuComputeAbsOpForward(cnmlCpuTensor_t input_tensor,
                                        void * input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void * output_buf);

cnmlStatus_t cnmlCpuComputeSoftplusOpForward(cnmlCpuTensor_t input_tensor,
                                        void * input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void * output_buf);

cnmlStatus_t cnmlCpuComputeMinusOpForward(cnmlCpuTensor_t input_tensor,
                                        void * input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void * output_buf);

cnmlStatus_t cnmlCpuComputeFakeQuantOpForward(cnmlCpuTensor_t input_tensor,
                                             void * input_buf,
                                             cnmlCpuTensor_t output_tensor,
                                             void * output_buf,
                                             float scale,
                                             int offset);

cnmlStatus_t cnmlCpuComputeAvgOpForward(cnmlCpuTensor_t input_tensor,
                                        void * input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void * output_buf);

cnmlStatus_t cnmlCpuComputeVector2NormOpForward(cnmlCpuTensor_t input_tensor,
                                        void * input_buf,
                                        cnmlCpuTensor_t output_tensor,
                                        void * output_buf);

cnmlStatus_t cnmlCpuComputeDeviceMemcpyOpForward(cnmlCpuTensor_t input_tensor,
    void *input,
    cnmlCpuTensor_t output_tensor,
    void *output);

cnmlStatus_t cnmlCpuComputeTransposeOpForward(cnmlCpuTensor_t input_tensor,
    void *input,
    cnmlCpuTensor_t output_tensor,
    void *output);

cnmlStatus_t cnmlCpuComputeCastOpForward(cnmlCastType_t fn,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output);

cnmlStatus_t cnmlCpuComputeSoftmaxOpForward(cnmlSoftmaxDim_t dim,
                                            cnmlCpuTensor_t input_tensor,
                                            void * input_buf,
                                            cnmlCpuTensor_t output_tensor,
                                            void * output_buf);

cnmlStatus_t cnmlCpuComputeGrepOpForward(cnmlGrepOpParam_t param,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output);

cnmlStatus_t cnmlCpuComputeArgmaxOpForward(cnmlArgmaxAxis_t argmax_mode,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output);

cnmlStatus_t cnmlCpuComputeReorgOpForward(cnmlReorgOpParam_t param,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output);

cnmlStatus_t cnmlCpuComputeYUVtoRGBOpForward(cnmlCpuTensor_t input_tensor,
                                       void * input_buf,
                                       cnmlCpuTensor_t output_tensor,
                                       void * output_buf);

cnmlStatus_t cnmlCpuComputeReshapeOpForward(cnmlReshapeOpParam_t param,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output);

cnmlStatus_t cnmlCpuComputeTopkOpForward(int k,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output,
                                         cnmlCpuTensor_t index_tensor,
                                         void *index);

cnmlStatus_t cnmlCpuComputeWhereOpForward(cnmlCpuTensor_t input_tensor,
                                          void *input,
                                          cnmlCpuTensor_t output_tensor,
                                          void *output,
                                          cnmlCpuTensor_t count_tensor,
                                          void *count);

cnmlStatus_t cnmlCpuComputeSpace2batchOpForward(int w_block_size,
                                         int h_block_size,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output);

cnmlStatus_t cnmlCpuComputeBatch2spaceOpForward(int w_block_size,
                                         int h_block_size,
                                         cnmlCpuTensor_t input_tensor,
                                         void *input,
                                         cnmlCpuTensor_t output_tensor,
                                         void *output);

cnmlStatus_t cnmlCpuComputeProposalOpForward(cnmlProposalOpParam_t param,
        cnmlCpuTensor_t bbox_deltas_tensor,
        void *bbox_deltas,
        cnmlCpuTensor_t scores_tensor,
        void *scores,
        cnmlCpuTensor_t output_tensor,
        void *output);


cnmlStatus_t
cnmlCpuNmsOpGenerateRois(void *input, int num_box, int height, int width);

cnmlStatus_t cnmlCpuComputeNmsOpForward(cnmlNmsOpParam_t param,
        cnmlCpuTensor_t input_tensor,
        void *input,
        cnmlCpuTensor_t output_tensor,
        void *output);

cnmlStatus_t cnmlCpuComputeFractionalPoolOpForward(cnmlFractionalPoolOpParam_t param,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output);

cnmlStatus_t cnmlCpuComputeLogSoftmaxOpForward(cnmlDimension_t dim,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output);

cnmlStatus_t cnmlCpuComputeStridedSliceOpForward(cnmlStridedSliceOpParam_t param,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output);

cnmlStatus_t cnmlCpuComputeYoloDetectOpForward(cnmlYoloDetectOpParam_t param,
                                           cnmlCpuTensor_t input_tensor,
                                           void *input,
                                           cnmlCpuTensor_t output_tensor,
                                           void *output);

////////////////////////////////// offline API ///////////////////////////////
struct cnmlModel;
typedef struct cnmlModel *cnmlModel_t;

/**
 * @brief Create a offline model handler.
 * @param model[out] Model pointer.
 * @param name[in] Model name.
 * @return CNML_STATUS_SUCCESS if success.
 *         otherwise the error code is returned.
 */
cnmlStatus_t cnmlCreateModel(cnmlModel_t *model, const char *name);

/**
 * @brief Destroy a offline model handler.
 * @param model[in] Model pointer
 * @return CNML_STATUS_SUCCESS if success.
 *         otherwise the error code is returned.
 */
cnmlStatus_t cnmlDestroyModel(cnmlModel_t model);

/**
 * @brief Add a compiled cnmlBaseOp in to given model. You should give this operation
 *        a symbol name for sake of the future reference. If given op has not
 *        been compiled, error code will be returned.
 * @param model[in] Model pointer
 * @param op[in] Pointer to a base operation.
 * @param symbol[in] Name of added base operation.
 * @return CNML_STATUS_SUCCESS if success.
 *         otherwise the error code is returned.
 */
cnmlStatus_t cnmlAddBaseOpToModel(cnmlModel_t model,
    cnmlBaseOp_t op, const char *symbol);

/**
 * @brief Add a compiled cnmlFusionOp in to given model. You should give this operation
 *        a symbol name for sake of the future reference.
 * @param model[in] Model pointer
 * @param op[in] Pointer to a fusion operation.
 * @param symbol[in] Name of added base operation.
 * @return CNML_STATUS_SUCCESS if success.
 *         otherwise the error code is returned.
 */
cnmlStatus_t cnmlAddFusionOpToModel(cnmlModel_t model,
    cnmlFusionOp_t op, const char *symbol);

/**
 * @brief Save a model to the file whose name is fname. If there is
 *        no operation in model, error code is returned.
 * @param model[in] Model pointer
 * @param fname[in] Offline model file name.
 * @return CNML_STATUS_SUCCESS if success.
 *         otherwise the error code is returned.
 */
cnmlStatus_t cnmlSaveModel(cnmlModel_t model, const char *fname);

/**
 * @brief Save a model to specific memory space start with ptr. If there is
 *        no operation in model or memroy is not efficient, error code is returned.
 * @param model[in] Model pointer
 * @param ptr[in] Memory space pointer.
 * @param len[in] Memory space size.
 * @param size[out] actual size of model.
 * @return CNML_STATUS_SUCCESS if success.
 *         otherwise the error code is returned.
 */
cnmlStatus_t cnmlSaveModelToMem(cnmlModel_t model, void *ptr,
    uint64_t len, uint64_t *size);

////////////////////////////////// test tools ///////////////////////////////

/* @brief get fix8 position from a data array
 * (NOTICE: This function is just for test program, user should get fix8 position
 *  by training in the real network !)
 * @param buffer[in], data array
 * @param size[in], data number
 * @return fix8 position of this data array
 */
int getFix8Position(float *buffer, int size);
int16_t float2half(float x);
int8_t float2fix8(float x, int position, int offset);

/* @brief diff file mlu_result and cpu_result
 * @param mlu_res[in], file name of mlu output
 * @param cpu_res[in], file name of cpu output
 * @return cnmlStatus_t
 */
cnmlStatus_t cnmlDiffFiles(const char* mlu_res, const char* cpu_res);

#if defined (__cplusplus)
}
#endif

#endif  // CNML_H_
