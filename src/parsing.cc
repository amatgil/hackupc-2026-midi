#include "parsing.hh"
#include "assert.h"
#include <cstdint>

// Don't forget to free the Sheet when you're done
// https://ccrma.stanford.edu/~craig/14q/midifile/MidiFileFormat.html#track_event
Sheet *parse_midi(unsigned char *text, unsigned int text_length) {
  // TODO
  return NULL;
}

Header parse_header(unsigned char* text, unsigned int text_length) {
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


void parse_track_chunk(unsigned char *text, unsigned int text_length) {
  // TODO: Error out more gracefully if tag is wrong
  assert(text[0] == 'M');
  assert(text[1] == 'T');
  assert(text[2] == 'r');
  assert(text[3] == 'k');

  uint32_t length = read_big_endian_4_bytes(text+4);

  unsigned char *current = text + 8;
  unsigned char *end_of_chunk = current + length;

  unsigned char running_status;
  while(current < end_of_chunk) {
    // 1. Llegir v-time
    unsigned int vtime_size = 0;
    uint64_t vtime = read_variable_length_quantity(current, vtime_size);
    current += vtime_size;

    // 2. Llegir l'event status
    unsigned char status = *current;
    if (status < 0x80) {
      // Event data
      status = running_status;
    }
    else {
      current++;
      // Canals de midi event
      if (status < 0xF0) {
        running_status = status;
      }
    }

    if (status == 0xFF) {
      // Meta Events
      unsigned char meta_type = *current;
      current++;

      unsigned int len_bytes = 0;
      uint64_t v_length = read_variable_length_quantity(text, len_bytes);
      current += len_bytes;

      // TODO: READ THE META-DATA!!

      current += v_length;
    }

    if (status == 0xFF || status == 0xF7) {
      unsigned int len_bytes = 0;
      uint64_t sysex_length = read_variable_length_quantity(current, len_bytes);
      current += len_bytes;
      // System exclusive event (currenly ignored :D)
      current += sysex_length;
    }
    else {
      // MIDIEvents: Note on, Note off, ...
    }
  }
  

  // track event time:  
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

uint64_t read_variable_length_quantity(unsigned char *text, unsigned int& size) {
  int i = 0;
  uint64_t data = 0;

  while(text[i] & 0x80) {
    // Em quedo amb els 7 bits menys significatius
    char byte = text[i] & 0x7F;
    data = data << 7;
    data |= byte;
    ++i;
  } // Si el MSB és 0, parem 

  char byte = text[i] & 0x7F;
  data = data << 7;
  data |= byte;
  size = i;
  return data;
}

