# Midily

A midi visualization and editing tool built for HackUPC2026.

# Images 



# Core Features

## Whistle-to-Note Synthesis 

The standout feature of Midily. Using a Fast Fourier Transform (FFT), the application captures live audio from your microphone, isolates the dominant frequency of your whistle or hum, and maps it to the nearest piano key in real-time.

## Midi editor

A useful view, which allows you to:

  - **Import**: Load any standard `.mid` file into the workspace
  - **Edit**: Click and drag notes to change their pitch and timing
  - **Compose**: Manually add new notes or delete them using simple controls.

## Midi visualizer

A playback mode where Notes "fall" toward a virtual keyboard, as seen in every viral piano performance ever.

# Compilation

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cd build
make -j8
```

# Team members

- Amat Gil
- Bernat Cuixart
- Gerard Félez
- Guillem Baldi


# TODO-list
- [x] Parsing MIDI to Sheet
- [ ] Exporting Sheet to MIDI
- [ ] Export Sheet to audio (using samples?)
- [x] Drawing Sheet to screen
- [x] Editing Sheet from screen
- [ ] BPM setting through keyboard
- [ ] Have pedals Toggle on keyboard
  - [x] Implement pedals
  - [ ] Display pedal status to the user
- [x] Whistling recognition (read microphone, decode notes, write)
  - This one was a substantial portion of the work
- [x] Show notes with faster attack velocity with more opaque colors
