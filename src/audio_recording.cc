/**
 * Fragment extret de https://github.com/mackron/miniaudio/blob/master/examples/simple_capture.c
 */
#include "audio_recording.hh"


using namespace std;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    float* p = (float*)pInput;
    while(p < pInput+frameCount) {
        microphone_buffer.push_back(*p);
        p = p+1;
    }

    (void)pOutput;
}

ma_result result;
ma_encoder_config encoderConfig;
ma_encoder encoder;
ma_device_config deviceConfig;
ma_device device;

void start_recording() {
    encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 44100);

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format   = encoder.config.format;
    deviceConfig.capture.channels = encoder.config.channels;
    deviceConfig.sampleRate       = 44100;
    deviceConfig.dataCallback     = data_callback;
    deviceConfig.pUserData        = &encoder;

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
    ma_encoder_uninit(&encoder);
}