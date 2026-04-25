#pragma once
#include "complex.h"
#include "fftw3.h"
#include "assert.h"
#include "raylib.h"

#define FFT_CHUNK_SIZE TODO

float *extreu_fft_from_samples(float *samples, size_t sample_length, float sampleRate);

// Per passar de i a frequencia:
// double freq = (double)i * la.sampleRate / (double)N;
float* extreu_fft_from_wav(Wave* la);


