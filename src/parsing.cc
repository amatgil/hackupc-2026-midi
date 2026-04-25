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

  Sheet sheet;

  vector<int> keys_held_start    = vector<int>(128, 0);
  vector<int> keys_held_velocity = vector<int>(128, 0);

  // TODO: Error out more gracefully if tag is wrong
  assert(text[0] == 'M');
  assert(text[1] == 'T');
  assert(text[2] == 'r');
  assert(text[3] == 'k');

  uint32_t length = read_big_endian_4_bytes(text+4);

  unsigned char *current = text + 8;
  unsigned char *end_of_chunk = current + length;

  unsigned char running_status;
  unsigned int current_time = 0;
  while(current < end_of_chunk) {
    // 1. Llegir v-time
    unsigned int vtime_size = 0;
    uint64_t vtime = read_variable_length_quantity(current, vtime_size);
    current += vtime_size;
    current_time += vtime;

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
              // --- Standard MIDI Event ---
      unsigned char event_type = status >> 4; // Top 4 bits dictate the event type
      unsigned char channel = status & 0x0F;
      
      // Program Change (0xC) and Channel Pressure (0xD) only have 1 data byte.
      // All other standard MIDI events have 2 data bytes.
      if (event_type == 0xC || event_type == 0xD) {
          unsigned char data1 = *current++;
          // We'll ignore both of them :D
      } else {
          // Note Off (0x8), Note On (0x9), Poly Pressure (0xA), 
          // Control Change (0xB), Pitch Bend (0xE)
          unsigned char data1 = *current++;
          unsigned char data2 = *current++;
          
          if (event_type == 0x9) {
            /* Note On */
            unsigned int key_index = data1;
            keys_held_start[data1] = current_time;
            keys_held_velocity[data1] = data2;
          }

          if (event_type == 0x8) {
            sheet.timestamps_start.push_back(keys_held_start[data1]); 
            sheet.durations.push_back(vtime); 
            sheet.pitch.push_back(data1); 
            sheet.attack_velocities.push_back(data2); 
          }
      }
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

