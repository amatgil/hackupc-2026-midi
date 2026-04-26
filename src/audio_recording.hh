#pragma once
/**
 * Fragment extret de https://github.com/mackron/miniaudio/blob/master/examples/simple_capture.c
 */
#include "miniaudio.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include "raylib.h"

#define AUDIO_RECORDING_SPEED 44100*2

using namespace std;

extern vector<float> microphone_buffer;
static int last_processed_index = 0;

void print_vec(vector<float> v);

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

void start_recording();

void stop_recording();
