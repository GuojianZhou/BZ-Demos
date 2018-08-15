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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib/cnml.h"
#include "src/include/tests/tests.h"

/* @brief get fix8 position from a data array
 * (NOTICE: This function is just for test program, user should get fix8 position
 *  by training in the real network !)
 * @param buffer[in], data array
 * @param size[in], data number
 * @return fix8 position of this data array
 */

/* @brief diff file mlu_result and cpu_result
 * @param mlu_res[in], file name of mlu output
 * @param cpu_res[in], file name of cpu output
 * @return cnmlStatus_t
 */
cnmlStatus_t cnmlDiffFiles(const char* mlu_res, const char* cpu_res) {
  int mlu_index = 0, cpu_index = 0;
  int mlu_buffer_size = 100000, cpu_buffer_size = 100000;

  FILE* mlu = fopen(mlu_res, "r");
  FILE* cpu = fopen(cpu_res, "r");

  if (mlu == NULL || cpu == NULL) {
    perror("File open failed, sad");
    return CNML_STATUS_INVALIDPARAM;
  }
  float* mlu_buffer = (float *)malloc(sizeof(float) * mlu_buffer_size);
  float* cpu_buffer = (float *)malloc(sizeof(float) * mlu_buffer_size);

  float tmp;
  // read mlu result
  while (fscanf(mlu, "%f", &tmp) != EOF) {
    if (mlu_index >= mlu_buffer_size) {
      mlu_buffer_size += mlu_buffer_size;
      mlu_buffer = (float *)realloc(mlu_buffer, sizeof(float) * mlu_buffer_size);
    }
    mlu_buffer[mlu_index++] = tmp;
  }
  // read cpu result
  while (fscanf(cpu, "%f", &tmp) != EOF) {
    if (cpu_index >= cpu_buffer_size) {
      cpu_buffer_size += cpu_buffer_size;
      cpu_buffer = (float *)realloc(cpu_buffer, sizeof(float) * cpu_buffer_size);
    }
    cpu_buffer[cpu_index++] = tmp;
  }

  if (mlu_index != cpu_index) {
    fclose(mlu);
    fclose(cpu);
    free(mlu_buffer);
    free(cpu_buffer);
    return CNML_STATUS_OUTOFRANGE;
  }

  // compute err rate
  float sumAbs = 0.0, sumB = 0.0, errRate = 0.0;
  float sumSqure_delta = 0.0, sumSqure = 0.0, errRateMSE = 0.0;
  for (int i = 0; i < mlu_index; i++) {
    if (mlu_buffer[i] >= 65504.0 || mlu_buffer[i] <= -65504.0) {
      printf("output overflow or underflow\n");
      fclose(mlu);
      fclose(cpu);
      free(mlu_buffer);
      free(cpu_buffer);
      return CNML_STATUS_OVERFLOWERR;
    }
    float delta = fabsf(mlu_buffer[i] - cpu_buffer[i]);
    sumAbs += delta;
    sumB   += fabsf(cpu_buffer[i]);
    sumSqure_delta += delta * delta;
    sumSqure += fabsf(cpu_buffer[i]) * fabsf(cpu_buffer[i]);
  }
  errRate = sumAbs / sumB;
  errRateMSE = sqrtf(sumSqure_delta / sumSqure);
  printf("\nerrRate is %f%%\n", errRate * 100);
  printf("errRateMSE is %f%%\n", errRateMSE * 100);

  fclose(mlu);
  fclose(cpu);
  free(mlu_buffer);
  free(cpu_buffer);

  return CNML_STATUS_SUCCESS;
}

