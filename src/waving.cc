#include "waving.hh"
#include "sheet.hh"
#include "fftw3.h"
#include <cstdint>
#include <cstdio>
#include <ratio>





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

float interpolatedMax(float* images, double* frequencies) {
    double alpha = (images[1] - images[0])/(frequencies[1] - frequencies[0]);
    double beta = (images[2] - images[0] - alpha*(frequencies[2]- frequencies[0]))/(frequencies[2] - frequencies[0])*(frequencies[2]- frequencies[1]);
    return (frequencies[1] + frequencies[0])/2. - alpha/2.*beta;
}

// La mida de retorn és sample_length / FFT_CHUNK_SIZE
double *which_pitch_is_playing_at_each_time_instance(float *samples,
                                                    size_t sample_length,
                                                    float sampleRate) {
  double* ret = (double*)malloc(sample_length / FFT_CHUNK_SIZE * sizeof(double));
  for (int i = 0; i < sample_length / FFT_CHUNK_SIZE; i++) {
    // TODO: Pass in memory (from this stack) instead of mallocing five quadspillion times
    float* chunk_de_samples = samples + FFT_CHUNK_SIZE*i;
    float* fft_of_chunk = extreu_fft_from_samples(chunk_de_samples, FFT_CHUNK_SIZE, sampleRate);

     // for (int j = 0; j < FFT_CHUNK_SIZE/2; j++) {
     //     double freq = (double)(j * sampleRate) / (double)FFT_CHUNK_SIZE;
     //     printf("%f, %f\n", freq, fft_of_chunk[j]);
     // }
     // printf("===============");

    float max_freq = 0;
    float max_freq_amplitude = 0.00001;
    int freqi = 0;

    for (int j = 0; j < FFT_CHUNK_SIZE/2; ++j) {
      double freq = (double)(j * sampleRate) / (double)FFT_CHUNK_SIZE;
      if (fft_of_chunk[j] > max_freq_amplitude) {
        max_freq_amplitude = fft_of_chunk[j];
        max_freq = freq;
        freqi = j;
      }
    }


    // new addition
    //double freqs[] = {(double)((freqi-1) * sampleRate) / (double)FFT_CHUNK_SIZE, max_freq, (double)((freqi+1) * sampleRate) / (double)FFT_CHUNK_SIZE};
    //float interpolated_freq = interpolatedMax(fft_of_chunk+freqi-1, freqs);

    free(fft_of_chunk);

    printf("%f\n",max_freq);
    ret[i] = max_freq;
  }
  return ret;
}

unsigned int frequency_to_pitch(float freq) {
  // doubling here -> adding 12 there
  // 440Hz -> 48

  float logarithm = log2(freq/440);
  return 12 * logarithm + 48;
}

void add_note_to_sheet(Sheet *s, float offset, unsigned int past_note_start_i,
                       float sampleRate, unsigned int i,
                       float past_note_frequency) {
                           if(past_note_frequency < 25) return;
  s->timestamps_start.push_back(offset +
                               past_note_start_i * FFT_CHUNK_SIZE / sampleRate);
  s->durations.push_back((i - past_note_start_i) * FFT_CHUNK_SIZE / sampleRate);
  s->pitch.push_back(frequency_to_pitch(past_note_frequency));
  s->attack_velocities.push_back(0); // No s'utilitza
}

// Always >= 1
float ratio_between(float a, float b) {
  return max(a, b) / min(a, b);
}

void smooth_out_pitches(double *pitches, size_t number_pitches) {
    size_t smoothing_window_size = min(5ul, number_pitches);
  double* temp = (double*)malloc(number_pitches * sizeof(double));
  for (int i = smoothing_window_size/2; i < number_pitches - smoothing_window_size/2; ++i) {
    double sum = 0;
    for (int j = i - smoothing_window_size/2; j < i + smoothing_window_size/2; ++j) {
        sum += pitches[j];
    }
    temp[i] = sum/(double)smoothing_window_size;
  }
  printf("Changing!\n");
  for (int i = smoothing_window_size/2; i < number_pitches - smoothing_window_size/2; ++i) pitches[i] = temp[i];
  printf("Changed!\n");

  free(temp);
}

// Takes samples at sample_rate and return Sheet
Sheet pitches_to_sheet(double *pitches, size_t number_pitches, float sampleRate,
                       float offset) {
  smooth_out_pitches(pitches, number_pitches);
  Sheet s;
double past_note_frequency = 0;
if(number_pitches > 0)
     past_note_frequency = pitches[0]; // Notes below 25Hz do not exist
  unsigned int past_note_start_i = 0;

  double candidate_note_frequency = 0; // Notes below 25Hz do not exist
  unsigned int candidate_note_start_i = 0;

  for (unsigned int i = 0; i < number_pitches; ++i) {
      if (ratio_between(pitches[i], past_note_frequency) >
          MAX_FREQ_RATIO_THRESHOLD) {
              if(ratio_between(candidate_note_frequency, pitches[i]) > MAX_FREQ_RATIO_THRESHOLD) {
                  candidate_note_frequency = pitches[i];
                  candidate_note_start_i = i;
              } else if(i - candidate_note_start_i > MAX_BEFORE_CLIPPING) {
                  add_note_to_sheet(&s, offset, past_note_start_i, sampleRate, candidate_note_start_i,
                                    past_note_frequency);
                  past_note_start_i = candidate_note_start_i;
                  past_note_frequency = candidate_note_frequency;
                  candidate_note_frequency = 0;
                  candidate_note_start_i = i;
              } else {
                  candidate_note_frequency += pitches[i] / (i - candidate_note_start_i+1) -
                                         candidate_note_frequency / (i-candidate_note_start_i+1);
              }

      } else {
        past_note_frequency += pitches[i] / (i - past_note_start_i+1) -
                               past_note_frequency / (i-past_note_start_i+1);
        candidate_note_frequency = 0;
        candidate_note_start_i = i;
      }

  }
  return s;
}

Sheet read_sheet_from_samples(float *samples, size_t sample_length,
                          float sampleRate) {

  double *pitches_per_time =
      which_pitch_is_playing_at_each_time_instance(samples, sample_length,
                                                   sampleRate);

  Sheet ret = pitches_to_sheet(pitches_per_time, sample_length / FFT_CHUNK_SIZE, sampleRate, 0.0);
  free(pitches_per_time);
  return ret;
}
