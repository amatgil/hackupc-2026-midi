#include "parsing.hh"
#include <stdlib.h>

void parse_header_from_file() {
  printf("Testing parse del header del fitxer simple \n");
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
  
  uint64_t res1 = read_variable_length_quantity(val1);
  uint64_t res2 = read_variable_length_quantity(val2);
  uint64_t res3 = read_variable_length_quantity(val3);
  uint64_t res4 = read_variable_length_quantity(val4);

  printf("%lx, %lx, %lx, %lx\n", res1, res2, res3, res4);

  assert(res1 == 0x80);
  assert(res2 == 0x2000);
  assert(res3 == 0x100000);
  assert(res4 == 0x200000);
}

void test_FFT_samples() {
  // parse_header_from_file();
  Wave la = LoadWave("../assets/samples/Mf A4.wav");
  printf("Got wav\n");
  float* samples = LoadWaveSamples(la);
  int N = la.frameCount * la.channels;
  printf("Got samples\n");
  FFT(samples, N);

  for (unsigned int i = 0; i < N; i++) {
    printf("%i -> %f\n", i, samples[i]);
  }    

  UnloadWaveSamples(samples);  
}