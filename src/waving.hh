#pragma once
#include "fft.hh"

#define FFT_CHUNK_SIZE TODO

float *extreu_fft_from_samples(float *samples, size_t sample_length,
                               float sampleRate) {
  int N = sample_length;
  fftw_complex *in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  fftw_complex *out = in; // in-place
  fftw_plan p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  for (int i = 0; i < N; ++i) {
    in[i][0] = samples[i];
    in[i][1] = 0.0;
  }    

  fftw_execute(p);

  float* ret = (float*)malloc(N * sizeof(float));  
  for (unsigned int i = 0; i < N / 2; i++) {
    double re = out[i][0];
    double im = out[i][1];
    ret[i] = 2 * sqrt(re * re + im * im) / N;
  }
  fftw_destroy_plan(p);
  fftw_free(in);

  return ret;  
}  

// Per passar de i a frequencia:
// double freq = (double)i * la.sampleRate / (double)N;
float* extreu_fft_from_wav(Wave* la) {
  assert(la != NULL);
  assert(la->channels == 1 || la->channels == 2);

  float *samples_interleaved = LoadWaveSamples(*la);
  float* samples = (float*)fftw_malloc(la->frameCount*sizeof(float));
  int N = la->frameCount;
  if (la->channels == 1) {
    samples = samples_interleaved;
  } else if (la->channels == 2) {
    for (int i = 0; i < N; ++i) {
      samples[i] = (double)((samples_interleaved[2 * i] + samples_interleaved[2 * i + 1]) * 0.5); // Average
    }
  }
  return extreu_fft_from_samples(samples, la->frameCount, la->sampleRate);
}

