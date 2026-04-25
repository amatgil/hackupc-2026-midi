#include "note_player.hh"

Sound note_sounds[88];

void load_note_sounds()
{
    for (int i = 0; i < 88; ++i)
    {
        char filepath[256];

        sprintf(filepath, "../assets/samples/Note_%02d.mp3", i);
        note_sounds[i] = LoadSound(filepath);
    }
}

void unload_note_sounds()
{
    for (int i = 0; i < 88; ++i)
    {
        //if (note_sounds_pp[i]) UnloadSound(note_sounds_pp[i]);

        UnloadSound(note_sounds[i]);
    }
}

void play_note_sound(unsigned int pitch, unsigned int velocity)
{
    if (pitch >= 88) return;

    Sound* sound_to_play = &note_sounds[pitch];

    if (sound_to_play != nullptr)
    {
        StopSound(*sound_to_play);
        PlaySound(*sound_to_play);
    }
}

void stop_note_sound(unsigned int pitch)
{
    if (pitch >= 88) return;

    StopSound(note_sounds[pitch]);
}