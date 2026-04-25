# Compilation
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cd build
make -j8
```


Only piano (for now)
- Polyphonic

# TODO
- [ ] Parsing MIDI to Sheet
- [ ] Exporting Sheet to MIDI
- [ ] Export Sheet to audio (using samples?)
- [ ] Drawing Sheet to screen
- [ ] Editing Sheet from screen
- [ ] BPM setting through keyboard
- [ ] Show noms del acords que es toquen alhora
  - [ ] On hover
  - [ ] Toggle with key
- [ ] Show pedals? Toggle on keyboard
- [ ] Lo del whistling (read microphone, decode notes, write)
- [ ] Show notes with faster attack velocity with more opaque colors
