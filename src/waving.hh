#pragma once
#include "complex.h"
#include "fftw3.h"
#include "assert.h"
#include "raylib.h"

// Per tal que, per cada segon, a 44100Hz, n'hi hagui approx 20
#define FFT_CHUNK_SIZE 2048 

float *extreu_fft_from_samples(float *samples, size_t sample_length, float sampleRate);

// Per passar de i a frequencia:
// double freq = (double)i * la.sampleRate / (double)N;
float* extreu_fft_from_wav(Wave* la);

float *which_pitch_is_playing_at_each_time_instance(float *samples,
                                                    size_t sample_length,
                                                    float sampleRate);
