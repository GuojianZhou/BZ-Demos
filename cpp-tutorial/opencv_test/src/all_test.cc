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

/* @brief basic test for sopa api
 *
 */
#include "src/include/tests/tests.h"
#include "iostream"

int main() {
  cnmlInit(0);
  unsigned dev_num;
  cnrtGetDeviceCount(&dev_num);
  if (dev_num == 0) return CNRT_RET_ERR_NODEV;
  cnrtDev_t dev;
  cnrtGetDeviceHandle(&dev, 0);
  cnrtSetCurrentDevice(dev);

  int op_code;
  printf("please choose which op to test:\n");
  printf("*******************************\n");
  printf("1.  active\n");
  printf("2.  pool_test\n");
  printf("3.  addpad\n");
  printf("4.  conv fix8\n");
  printf("5.  mlp fix8\n");
  printf("*******************************\n");
  std::cin >> op_code;

  switch (op_code) {
    case 1:
      active_test(); break;
    case 2:
      pool_test(); break;
    case 3:
      addpad_test(); break;
    case 4:
      conv_fix8_test(); break;
    case 5:
      mlp_fix8_test(); break;
    default:
      printf("unsupport operation!\n");
      return 0;
  }
  cnmlExit();
  cnmlDiffFiles("mlu_output", "cpu_output");

  return 0;
}
