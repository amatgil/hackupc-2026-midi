#include "parsing.hh"
#include "assert.h"

Sheet *parse_midi(char *text, unsigned int text_length) {
  // TODO
  return NULL;
}

Header parse_headers(char* text, unsigned int text_length) {
  // TODO: De moment assumim que el fitxer és prou gran. Potser
  // no és el cas i tenim UB

  if (text[0] != 'M' || text[1] != 'T' ||
      text[2] != 'h' || text[3] != 'd') {
    assert(0); // TODO: Retorna un error de manera millor
  }
  return (Header) {
    .length = read_big_endian_4_bytes(text + 4),
    .format = read_big_endian_2_bytes(text + 4 + 4),
    .num_tracks = read_big_endian_2_bytes(text + 4 + 4 + 2),
    .division = read_big_endian_2_bytes(text + 4 + 4 + 2 + 2)
  };
}

uint32_t read_big_endian_4_bytes(char *text) {
  return (uint32_t)text[0] << 24 |
           (uint32_t)text[1] << 16 |
           (uint32_t)text[2] << 8 |
           (uint32_t)text[3] << 0;
}

uint16_t read_big_endian_2_bytes(char *text) {
  return (uint16_t)text[2] << 8 |
           (uint16_t)text[3] << 0;
}
