#pragma once
#include <vector>

using namespace std;

typedef struct {
  vector<float> timestamps_start;
  vector<float> durations;
  // Un enter menut que s'associa amb una frequencia
  vector<unsigned int> pitch;
  vector<unsigned int> attack_velocities;
  unsigned int total_duration; // en segons
} Sheet;
