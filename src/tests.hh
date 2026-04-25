#pragma once

#include "parsing.hh"
#include <stdlib.h>
#include <complex.h>
#include <fftw3.h>
#include "waving.hh"

void parse_header_from_file() {
  //printf("Testing parse del header del fitxer simple \n");
  FILE* fptr = fopen("../assets/testfiles/SimpleMidi.mid", "r");
  assert(fptr != NULL);
  int i = 0, c;
  unsigned char* s = (unsigned char*)malloc(10000);
  while ((c = fgetc(fptr)) != EOF) {
    s[i] = c;
    i++;
    printf("i = %d\n", i);
  }
  printf("FInished while!\n");
  Header h = parse_header(s, 12);
  printf("Finished parsing\n");
  assert(h.length == 6);
  assert(h.format == 1);
  assert(h.num_tracks == 2);
  assert(h.division == 480);
}

void test_variable_length_quantity() {
  unsigned char val1[2] = {0x81, 0x00};
  unsigned char val2[2] = {0xC0, 0x00};
  unsigned char val3[3] = {0xC0, 0x80, 0x00};
  unsigned char val4[4] = {0x81, 0x80, 0x80};
  unsigned int x;
  uint64_t res1 = read_variable_length_quantity(val1, x);
  uint64_t res2 = read_variable_length_quantity(val2, x);
  uint64_t res3 = read_variable_length_quantity(val3, x);
  uint64_t res4 = read_variable_length_quantity(val4, x);

  printf("%lx, %lx, %lx, %lx\n", res1, res2, res3, res4);

  assert(res1 == 0x80);
  assert(res2 == 0x2000);
  assert(res3 == 0x100000);
  assert(res4 == 0x200000);
}

void test_FFT_samples() {
  Wave la = LoadWave("../assets/testfiles/guillem_whistling.wav");
  float* samples = LoadWaveSamples(la);

  assert(la.channels == 1 || la.channels == 2);
  int N = la.frameCount;
  fftw_complex *in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  fftw_complex *out = in; // in-place
  fftw_plan p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  if (la.channels == 1) {
    for (int i = 0; i < N; ++i) {
      in[i][0] = samples[i];
      in[i][1] = 0.0;
    }
  } else if (la.channels == 2) {
    for (int i = 0; i < N; ++i) {
      in[i][0] =
          (double)((samples[2 * i] + samples[2 * i + 1]) * 0.5); // Average
      in[i][1] = 0.0;
    }
  }
  fftw_execute(p);
  for (unsigned int i = 0; i < N / 2; i++) {
    double freq = (double)i * la.sampleRate / (double)N;
    double re = out[i][0];
    double im = out[i][1];
    printf("%f, %f\n", freq, 2 * sqrt(re * re + im * im) / N);
  }
  fftw_destroy_plan(p);
  fftw_free(in);

  UnloadWaveSamples(samples);
}


void printvector(const vector<float>& v) {
  for (auto x: v) {
    printf("%f, ", x);
  }
  printf("\n");
}
void printvector(const vector<unsigned int>& v) {
  for (auto x: v) {
    printf("%d, ", x);
  }
  printf("\n");
}

void test_full_parse() {
  printf("Testing parse del header del fitxer simple \n");
  FILE* fptr = fopen("../assets/testfiles/SimpleMidi.mid", "r");
  assert(fptr != NULL);
  unsigned int i = 0, c;
  unsigned char* s = (unsigned char*)malloc(10000);

  while ((c = fgetc(fptr)) != EOF) {
    s[i] = c;
    i++;
  }

  Sheet sheet = parse_midi(s, i, 120);
  printvector(sheet.timestamps_start);
  printvector(sheet.durations);
  printvector(sheet.attack_velocities);
  printvector(sheet.pitch);
}
/*
void test_FFT_samples_calling() {
  Wave la = LoadWave("../assets/testfiles/guillem-doublenote.wav");
  float* ret = extreu_fft_from_wav(&la);
  int N = la.frameCount;
  for (int i = 0; i < N; ++i) {
    double freq = (double)i * la.sampleRate / (double)N;
    printf("%f %f\n", freq, ret[i]);

  }
}*/

void test_FFT_with_chunking_yay() {
  Wave la = LoadWave("../assets/testfiles/guillem-doublenote.wav");
  assert(la.channels == 1 || la.channels == 2);

  float *samples_interleaved = LoadWaveSamples(la);
  float* samples = (float*)malloc(la.frameCount*sizeof(float));
  int N = la.frameCount;
  if (la.channels == 1) {
    samples = samples_interleaved;
  } else if (la.channels == 2) {
    for (int i = 0; i < N; ++i) {
      samples[i] = (double)((samples_interleaved[2 * i] + samples_interleaved[2 * i + 1]) * 0.5); // Average
    }
    free(samples_interleaved);
  }

  double* pitches = which_pitch_is_playing_at_each_time_instance(samples, la.frameCount, la.sampleRate);
  for (int i = 0; i < la.frameCount / FFT_CHUNK_SIZE; ++i) {
    printf("%i %f\n", i, pitches[i]);
  }

  free(pitches);

  free(samples);

  UnloadWave(la);
}
