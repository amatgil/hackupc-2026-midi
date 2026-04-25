#include "parsing.hh"
#include "assert.h"

Sheet *parse_midi(unsigned char *text, unsigned int text_length) {
  // TODO
  return NULL;
}

Header parse_headers(unsigned char* text, unsigned int text_length) {
  // TODO: De moment assumim que el fitxer és prou gran. Potser
  // no és el cas i tenim UB

  // TODO: Error out more gracefully if tag is wrong
  assert(text[0] == 'M');
  assert(text[1] == 'T');
  assert(text[2] == 'h');
  assert(text[3] == 'd');
  text = text + 4;

  return (Header) {
    .length = read_big_endian_4_bytes(text),
    .format = read_big_endian_2_bytes(text + 4),
    .num_tracks = read_big_endian_2_bytes(text + 4 + 2),
    .division = read_big_endian_2_bytes(text + 4 + 2 + 2 )
  };
}

uint32_t read_big_endian_4_bytes(unsigned char *text) {
  return (uint32_t)text[0] << 24 |
           (uint32_t)text[1] << 16 |
           (uint32_t)text[2] << 8 |
           (uint32_t)text[3] << 0;
}

uint16_t read_big_endian_2_bytes(unsigned char *text) {
  return (uint16_t)text[0] << 8 |
           (uint16_t)text[1] << 0;
}
