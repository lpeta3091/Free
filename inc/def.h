#ifndef DEF_H
#define DEF_H

#include <syslog.h>

using namespace std;

struct _mData {
  uint8_t num_cycles = 10;
  uint8_t num_cycles_dist = 1;
  int min_distance = 330;
  int max_distance = 480;
  int min_angle = 10;
  int max_angle = 85;
};

extern _mData mData;

#endif
