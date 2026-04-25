#include "parsing.hh"
#include "assert.h"
#include <stdio.h>
#include <cstdint>
#include <cstdlib>

// Don't forget to free the Sheet when you're done
// https://ccrma.stanford.edu/~craig/14q/midifile/MidiFileFormat.html#track_event
Sheet parse_midi(unsigned char *text, unsigned int text_length, float BPM) {
  Header h = parse_header(text, text_length);
  int longitud_llegida;
  Sheet s;
  unsigned char* text_end = text + text_length;
  text += 14;
  while(text < text_end) {
    parse_track_chunk(text, text_length, longitud_llegida, s);
    text += longitud_llegida;
  }
  
  s.total_duration /= (h.division*BPM/60.0f);
  for (int i = 0; i < s.durations.size(); ++i) {
    s.timestamps_start[i] /= (h.division*BPM/60.0f);
    s.durations[i] /= (h.division*BPM/60.0f);
  }
  return s;
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


void parse_track_chunk(unsigned char *text, unsigned int text_length, int& longitud_llegida, Sheet& sheet) {

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
  longitud_llegida = length + 4 + 4;

  unsigned char running_status = 0;
  unsigned int current_time = 0;
  
  while(current < end_of_chunk) {
    // 1. Llegir v-time
    unsigned int vtime_size = 0;
    uint64_t vtime = read_variable_length_quantity(current, vtime_size);
    current += vtime_size;
    current_time += vtime;

    // 2. Llegir l'event status
    unsigned char status = *current;
    
    // Per debugging, pots descomentar això:
    //printf("Status: %x (vtime: %llu, current_time: %u)\n", status, vtime, current_time);
    
    if (status < 0x80) {
      // Event data (Running status)
      status = running_status;
    }
    else {
      current++;
      // Canals de midi event (0x80 a 0xEF)
      if (status < 0xF0) {
        running_status = status;
      }
    }

    // 3. Processar l'event segons el status
    if (status == 0xFF) {
      // --- Meta Events ---
      unsigned char meta_type = *current;
      current++;

      unsigned int len_bytes = 0;
      // CORRECCIÓ: Passar 'current' en lloc de 'text'
      uint64_t v_length = read_variable_length_quantity(current, len_bytes);
      current += len_bytes;

      // TODO: READ THE META-DATA!!
      //printf("Printing metadata: ", current);

      current += v_length;
    }
    // CORRECCIÓ: else if, i detectar SysEx correctament (0xF0 i 0xF7)
    else if (status == 0xF0 || status == 0xF7) {
      // --- System Exclusive ---
      unsigned int len_bytes = 0;
      uint64_t sysex_length = read_variable_length_quantity(current, len_bytes);
      current += len_bytes;
      // System exclusive event (currently ignored :D)
      current += sysex_length;
    }
    else {
      // --- Standard MIDI Event ---
      unsigned char event_type = status >> 4; // Top 4 bits dictate the event type
      unsigned char channel = status & 0x0F;
      
      // Program Change (0xC) and Channel Pressure (0xD) only have 1 data byte.
      if (event_type == 0xC || event_type == 0xD) {
          unsigned char data1 = *current++;
          // We'll ignore both of them :D
      } else {
          // All other standard MIDI events have 2 data bytes.
          unsigned char data1 = *current++;
          unsigned char data2 = *current++;
          
          // Note On (Amb velocitat > 0)
          if (event_type == 0x9 && data2 > 0) {
            //printf("Entered NoteON (Pitch: %d, Vel: %d)\n", data1, data2);
            if (data1 > 87) data1 = 87;
            keys_held_start[data1] = current_time;
            keys_held_velocity[data1] = data2;
          }
          // Note Off (o Note On amb velocitat == 0)
          else if (event_type == 0x8 || (event_type == 0x9 && data2 == 0)) {
            
            //printf("Entered NoteOff (Pitch: %d)\n", data1);
            
            // La duració és el temps actual menys quan va començar
            unsigned int duration = current_time - keys_held_start[data1];
            
            sheet.timestamps_start.push_back(keys_held_start[data1]); 
            sheet.durations.push_back(duration); 
            sheet.pitch.push_back(data1); 
            sheet.attack_velocities.push_back(keys_held_velocity[data1]); 
          }
      }
    }
  }
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
  
  size = i + 1;
  return data;
}

uint32_t read_big_endian_4_bytes(unsigned char *text) {
  return (uint32_t)text[0] << 24   |
           (uint32_t)text[1] << 16 |
           (uint32_t)text[2] << 8  |
           (uint32_t)text[3] << 0;
}

uint16_t read_big_endian_2_bytes(unsigned char *text) {
  return (uint16_t)text[0] << 8  | 
           (uint16_t)text[1] << 0;
}

/*int64_t read_variable_length_quantity(unsigned char *text, unsigned int& size) {
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
  size = i + 1;
  return data;
}*/



Sheet read_midi_file(char* file, float BPM) {
  printf("Testing parse del header del fitxer simple \n");
  
  FILE* fptr = fopen(file, "r");
  assert(fptr != NULL);
  
  unsigned int i = 0, c;
  fseek(fptr, 0L, SEEK_END);
  unsigned int sz = ftell(fptr);
  
  unsigned char* s = (unsigned char*) malloc(sz);
  rewind(fptr);
  printf("Size: %d\n", sz);
  while ((c = fgetc(fptr)) != EOF) {
    s[i] = c;
    i++;
  }
  
  return parse_midi(s, i, BPM);
}
