#pragma once
/**
 * Fragment extret de https://github.com/mackron/miniaudio/blob/master/examples/simple_capture.c
 */
#include "audio_recording.hh"

std::vector<float> microphone_buffer;

using namespace std;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    float* p = (float*)pInput;
    while(p < pInput+frameCount) {
        microphone_buffer.push_back(*p);
        p = p+1;
    }

    //printf("%f\n", *p);

    (void)pOutput;
}

ma_result result;
ma_device_config deviceConfig;
ma_device device;

void start_recording() {

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format   = ma_format_f32;
    deviceConfig.capture.channels = 1;
    deviceConfig.sampleRate       = AUDIO_RECORDING_SPEED;
    deviceConfig.dataCallback     = data_callback;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize capture device.\n");
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
    }

}

void stop_recording() {
    microphone_buffer.clear();
    ma_device_uninit(&device);
}
