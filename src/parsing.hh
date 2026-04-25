#pragma once

#include "sheet.hh"
#include <endian.h>
#include <stdint.h>

typedef struct {
  uint32_t length;
  uint32_t format;
  uint16_t num_tracks;
  uint16_t division;  
} Header;  

Sheet *parse_midi(char *text, unsigned int text_length);
Header parse_headers(char* text, unsigned int text_length);
uint32_t read_big_endian_4_bytes(char *text);
uint16_t read_big_endian_2_bytes(char *text);
