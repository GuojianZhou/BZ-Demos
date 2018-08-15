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

/*****************************************************
# Author:            : liwei
# Last modified      : 2018-03-14 11:20
# Email              : liwei@cambricon.com
# Filename           : tests.hpp
# Description        :
*****************************************************/

#ifndef TESTS_TESTS_H_
#define TESTS_TESTS_H_

#include "lib/cnml.h"

int active_test();
int addpad_test();
int conv_fix8_test();
int mlp_fix8_test();
int pool_test();

/* @brief diff file mlu_result and cpu_result
 * @param mlu_res[in], file name of mlu output
 * @param cpu_res[in], file name of cpu output
 * @return cnmlStatus_t, almost be success, haha
 */
cnmlStatus_t cnmlDiffFiles(const char* mlu_res, const char* cpu_res);

#endif  // TESTS_TESTS_H_
