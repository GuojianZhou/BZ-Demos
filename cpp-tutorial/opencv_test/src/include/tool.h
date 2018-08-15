#ifndef SOPA_API_TOOL_H_
#define SOPA_API_TOOL_H_
#include <cstdint>

int getFix8Position(float *buffer, int size);
int16_t float2half(float x);
int8_t float2fix8(float x, int position, int offset);

#endif  // SOPA_API_TOOL_H_
