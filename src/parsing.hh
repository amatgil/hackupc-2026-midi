#pragma once

#include "sheet.hh"
#include <endian.h>
#include <stdint.h>

enum MidiEventType {
  NOTE_OFF,
  NOTE_ON,
  NoteAfterTouch,
  Controller,
  ProgramChange,
  ChannelAftertouch,
  PitchBend,
  // TODO!!
};

enum MetaEventType {
  SequenceNumber,
  TextEvent,
  CopyrightNotice,
  SequenceOrTrackName,
  InstrumentName,
  LyricText,
  MarkerText,
  CuePoint,
  MidiChannelPrefix,
  EndOfTrack,
  TempoSetting,
  SMPTEOffset,
  TimeSignature,
  KeySignature,
  SequencerSpecificEvent
};

typedef struct {
  uint32_t length;
  uint32_t format;
  uint16_t num_tracks;
  uint16_t division;
} Header;

typedef struct {
  MetaEventType meta_type;
  uint32_t meta_length;
  uint8_t *event_data_bytes;
  // midi event:
} MetaEvent;

Sheet parse_midi(unsigned char *text, unsigned int text_length, float BPM);
Header parse_header(unsigned char* text, unsigned int text_length);
void parse_track_chunk(unsigned char *text, unsigned int text_length, int& longitud_llegida, Sheet& sheet);
uint32_t read_big_endian_4_bytes(unsigned char *text);
uint16_t read_big_endian_2_bytes(unsigned char *text);
uint64_t read_variable_length_quantity(unsigned char *text, unsigned int& size);
Sheet read_midi_file(const char* file, float BPM);
