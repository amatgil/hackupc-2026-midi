/**
 * Fragment extret de https://github.com/mackron/miniaudio/blob/master/examples/simple_capture.c
 */
#include "miniaudio.c"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <queue>


using namespace std;

vector<float> microphone_buffer;

void print_vec(vector<float> v);

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

void start_recording();

void stop_recording();