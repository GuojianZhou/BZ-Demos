#include "cnrt.h"
#include <pthread.h>
#include <sys/time.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace cv;
using namespace std;

#define ERROR_CHECK(ret) if (ret != CNRT_RET_SUCCESS) {\
    printf("error occur, func: %s, line: %d\n", __func__, __LINE__);\
    exit(0);\
}\

bool file_clear = 0;
int img_processed = 0;
vector<string> filename;
vector<string> filename_use;
uint8_t* dataBuffer;
float* output_cpu;
float* means;

uint32_t in_n,in_c,in_h,in_w;
uint32_t out_n,out_c,out_h,out_w;
int count_batch, count_use;
string file_name,names;

string get_string(string names, int index){
  ifstream files(names.c_str(), std::ios::in);
  string file;
  for(int i = 0; i < index; i++){
    getline(files, file);
  }
  getline(files, file);
  files.close();
  return file;
}

void* softmax_print(int outputSize){
    for(int n = 0; n < count_use; n++){
      int max_index[5] = {0};
      double max_num[5] = {0};

      for(int i = 0; i < out_c * out_h * out_w; i++){
        double tmp = output_cpu[i + n * out_c * out_w * out_h];
        int tmp_index = i;
        for(int j = 0; j < 5; j++){
          if(tmp > max_num[j]){
             tmp_index += max_index[j];
             max_index[j] = tmp_index - max_index[j];
             tmp_index -= max_index[j];
             tmp += max_num[j];
             max_num[j] = tmp-max_num[j];
             tmp -= max_num[j];
          }
        }
      }
      cout << "------------------------detection for " << filename_use[n] << "---------------------------" << endl;
      for(int i = 0; i < 5; i++){
        cout << fixed << setprecision(4) << max_num[i] << "  -  " << get_string(names,max_index[i]) << endl;
      }
    }
    return NULL;
}

void get_img(){
    string file;
    filename.clear();
    int tmp = img_processed;
    ifstream files(file_name.c_str(), std::ios::in);
    while(tmp>0&&getline(files,file)){
       --tmp;
    }
    if(tmp>0||(!getline(files,file))) {
        count_batch = 0;
        file_clear = 1;
        count_use = count_batch;
        filename_use = filename;
        return;
    }
    vector<vector<cv::Mat> > input_imgs;
    filename.push_back(file);
    Mat img_resized;
    cv::Mat img = imread(file, -1);
    if(img.empty()) {
        cout << "error ! no such file : " << file << endl;
        count_batch = 0;
        file_clear = 1;
        return;
    }
    cv::Mat sample;
    if (img.channels() == 3 && in_c == 1)
      cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
    else if (img.channels() == 4 && in_c == 1)
      cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
    else if (img.channels() == 4 && in_c == 3)
      cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
    else if (img.channels() == 1 && in_c == 3)
      cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
    else if (img.channels() == 3 && in_c == 4)
      cv::cvtColor(img, sample, cv::COLOR_BGR2BGRA);
    else if (img.channels() == 1 && in_c == 4)
      cv::cvtColor(img, sample, cv::COLOR_GRAY2BGRA);
    else
      sample = img;

    uint8_t *input_data = dataBuffer;
    for(int i=0;i<in_n;i++){
        input_imgs.push_back(std::vector<cv::Mat> ());
        for(int j=0;j<in_c;j++){
           cv::Mat channel(in_h, in_w, CV_8UC1, input_data);
           input_imgs[i].push_back(channel);
           input_data += in_h * in_w;
        }
    }

    count_batch = 1;
    if(img.rows != in_h || img.cols != in_w || img.channels() != in_c) {
      cout << "warining! image " << file << " size is wrong!" << endl;
      cout << "input size should be :" << in_c << " " << in_h << " " << in_w << endl;
      cout << "now input size is :" << img.channels() << " " << img.rows<<" " << img.cols << endl;
      cout << "img is going to resize!" << endl;
    }
    if(img.rows != in_h || img.cols != in_w )
      resize(sample, img_resized, cv::Size(in_h, in_w));
    else
      img_resized = sample;

    cv::Mat img_float;
    if(in_c == 3)
       img_resized.convertTo(img_float, CV_8UC3);
    else if(in_c == 4)
       img_resized.convertTo(img_float, CV_8UC4);
    else
       img_resized.convertTo(img_float, CV_8UC1);
    cv::split(img_float, (input_imgs)[count_batch-1]);
    while(count_batch != in_n&&getline(files,file) ) {
     cout << "count_batch = " <<count_batch<<" in" << __FUNCTION__<<endl;
      Mat img_resized1;
      cv::Mat img1 = imread(file, -1);
      cv::Mat sample1;
      if (img.channels() == 3 && in_c == 1)
        cv::cvtColor(img1, sample1, cv::COLOR_BGR2GRAY);
      else if (img1.channels() == 4 && in_c == 1)
        cv::cvtColor(img1, sample1, cv::COLOR_BGRA2GRAY);
      else if (img1.channels() == 4 && in_c == 3)
        cv::cvtColor(img1, sample1, cv::COLOR_BGRA2BGR);
      else if (img1.channels() == 1 && in_c == 3)
        cv::cvtColor(img1, sample1, cv::COLOR_GRAY2BGR);
      else if (img1.channels() == 1 && in_c == 4)
        cv::cvtColor(img1, sample1, cv::COLOR_GRAY2BGRA);
      else if (img1.channels() == 3 && in_c == 4)
        cv::cvtColor(img1, sample1, cv::COLOR_BGR2BGRA);
      else
        sample1 = img1;
      if(img1.empty())  break;
      filename.push_back(file);
      if(img1.rows != in_h ||
         img1.cols != in_w ||
         img1.channels() != in_c) {
        cout << "warining! image " << file << " size is wrong!" << endl;
        cout << "input size should be :" << in_c << " " << in_h << " " << in_w << endl;
        cout << "now input size is :" << img1.channels() << " " << img1.rows << " " << img1.cols << endl;
        cout << "img is going to resize!" << endl;
      }
      if(img1.rows != in_h ||
         img1.cols != in_w)
        resize(sample1, img_resized1, cv::Size(in_h, in_w));
      else
        img_resized1 = sample1;

      count_batch++;
      cv::Mat img_float1;
      img_resized1.convertTo(img_float1, CV_8UC4);
      cv::split(img_float1, (input_imgs)[count_batch-1]);
    }
    img_processed += count_batch;
    count_use = count_batch;
    filename_use = filename;
}

int main(int argc, char* argv[]) {
  if(argc != 4) {
    printf("wrong args\n");
    std::cerr << "Usage: " << argv[0]
              << " offline.cambricon image_list_file"
              << " labels.txt"  << std::endl;
    exit(0);
  }
  std::cout<<"in"<<std::endl;
  names = (string)argv[3];
  int elapse = 0;
  struct timeval ts,te;
  gettimeofday(&ts, NULL);
  cnrtInit(0);
  file_name = (string)(argv[2]);

  // 1. init runtime_lib and device
  unsigned dev_num;
  cnrtGetDeviceCount(&dev_num);
  if (dev_num == 0){
    std::cout<<"no device found"<<std::endl;
    exit(-1);
  }
  cnrtDev_t dev;
  cnrtGetDeviceHandle(&dev, 0);
  cnrtSetCurrentDevice(dev);
  // 2. load model and get function
  cnrtModel_t model;
  string fname = (string)argv[1];
  printf("load file: %s\n", fname.c_str());
  cnrtLoadModel(&model, fname.c_str());
  cnrtFunction_t function;
  string name="subnet0";
  cnrtCreateFunction(&function);
  cnrtExtractFunction(&function, model, name.c_str());
  cnrtInitFunctionMemory(function, CNRT_FUNC_TYPE_BLOCK);
  // 3. get function's I/O DataDesc
  int inputNum, outputNum;
  cnrtDataDescArray_t inputDescS, outputDescS;
  cnrtGetInputDataDesc (&inputDescS , &inputNum , function);
  cnrtGetOutputDataDesc(&outputDescS, &outputNum, function);
  if(inputNum != 1){
    std::cout<<"error, not classfiy model!"<<std::endl;
    exit(-1);
  }
  if(outputNum != 1){
    std::cout<<"error, not classfiy model!"<<std::endl;
    exit(-1);
  }
  // 4. allocate I/O data space on CPU memory and prepare Input data
  void** inputCpuPtrS  = (void**) malloc (sizeof(void*) * inputNum);
  void** outputCpuPtrS = (void**) malloc (sizeof(void*) * outputNum);
  int in_count;
  cnrtDataDesc_t inputDesc = &(inputDescS[0]);
  cnrtSetHostDataLayout(inputDesc, CNRT_UINT8, CNRT_NCHW);
  cnrtGetHostDataCount(inputDesc, &in_count);
  cnrtGetDataShape(inputDesc, &in_n, &in_c, &in_h, &in_w);
  dataBuffer = (uint8_t*) malloc(sizeof(uint8_t) * in_count);
  inputCpuPtrS[0] = (void*)dataBuffer;

  int out_count;
  cnrtDataDesc_t outputDesc = &(outputDescS[0]);
  cnrtSetHostDataLayout(outputDesc, CNRT_FLOAT32, CNRT_NCHW);
  cnrtGetHostDataCount(outputDesc, &out_count);
  cnrtGetDataShape(outputDesc, &out_n, &out_c, &out_h, &out_w);
  output_cpu = (float*) malloc (sizeof(float) * out_count);

  outputCpuPtrS[0] = (void*)output_cpu;
  // 5. allocate I/O data space on MLU memory and copy Input data
  void **inputMluPtrS;
  void **outputMluPtrS;
  cnrtMallocByDescArray(&inputMluPtrS ,  inputDescS,  inputNum);
  cnrtMallocByDescArray(&outputMluPtrS, outputDescS, outputNum);
  // 6. create stream and run function
  void *param[2] = {inputMluPtrS[0], outputMluPtrS[0]};
  cnrtDim3_t dim = {1, 1, 1};
  cnrtStream_t stream;
  cnrtCreateStream(&stream);
  // 7. copy back Output data and write it to file

  gettimeofday(&te, NULL);
  elapse = 1000000 * (te.tv_sec - ts.tv_sec) + te.tv_usec - ts.tv_usec;
  cout<<"init time: " << elapse << endl;


  int count = 1;
  float time_use;
  struct timeval tpstart, tpend;
  gettimeofday(&tpstart, NULL);

  while(!file_clear) {
      if(count > 0) {
        get_img();
        if(!file_clear){
          cnrtMemcpyByDescArray(inputMluPtrS, inputCpuPtrS, inputDescS, inputNum, CNRT_MEM_TRANS_DIR_HOST2DEV);
          cnrtInvokeFunction(function, dim, param, CNRT_FUNC_TYPE_BLOCK, stream, NULL);
          cnrtSyncStream(stream);
          cout<<"after cnrtSyncStream"<<endl;
          cnrtMemcpyByDescArray(outputCpuPtrS, outputMluPtrS, outputDescS, outputNum, CNRT_MEM_TRANS_DIR_DEV2HOST);
          softmax_print(out_count);
          gettimeofday(&tpend, NULL);
          time_use = 1000000 * (tpend.tv_sec - tpstart.tv_sec) + tpend.tv_usec - tpstart.tv_usec;
          if(count >= 1 )
            cout<<" iter " << count - 1 << " execution time: "<<time_use<<endl;
          gettimeofday(&tpstart, NULL);
          count++;
         }
      }
  }
  free(means);
  free(dataBuffer);
  free(output_cpu);
  // 8. free memory space
  free(inputCpuPtrS);
  free(outputCpuPtrS);
  cnrtFreeArray(inputMluPtrS, inputNum);
  cnrtFreeArray(outputMluPtrS, outputNum);
  cnrtDestroyStream(stream);
  cnrtDestroyFunction(function);
  cnrtUnloadModel(model);
  cnrtDestroy();

  return 0;
}
