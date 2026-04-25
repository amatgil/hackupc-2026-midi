#include "parsing.hh"

Sheet *parse_midi(char *text, unsigned int text_length) {
  
}

Header parse_headers(char* text, unsigned int text_length) {
  int i = 0;
  return (Header) {
  };
  uint32_t l = read_big_endian_4_bytes(text);
  
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
