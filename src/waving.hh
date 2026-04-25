#pragma once
#include "complex.h"
#include "sheet.hh"
#include "fftw3.h"
#include "assert.h"
#include "raylib.h"

// Per tal que, per cada segon, a 44100Hz, n'hi hagui approx 20
#define FFT_CHUNK_SIZE 2048
#define MIN_NOTE_SIZE 3
#define MAX_BEFORE_CLIPPING 4
// Half of root(2; 12)
#define MAX_FREQ_RATIO_THRESHOLD 1.03
#define SMOOTHING_WINDOW_SIZE (FFT_CHUNK_SIZE/4)

float *extreu_fft_from_samples(float *samples, size_t sample_length, float sampleRate);

// Per passar de i a frequencia:
// double freq = (double)i * la.sampleRate / (double)N;
float* extreu_fft_from_wav(Wave* la);

double *which_pitch_is_playing_at_each_time_instance(float *samples,
                                                    size_t sample_length,
                                                    float sampleRate);

Sheet pitches_to_sheet(float *pitches, size_t number_pitches, float sampleRate,
                       float timestamp_start);

Sheet read_sheet_from_samples(float *samples, size_t sample_length,
                              float sampleRate);
