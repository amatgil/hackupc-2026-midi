#include "note_player.hh"

Sound note_sounds_pp[88];
Sound note_sounds_mp[88];
Sound note_sounds_mf[88];
Sound note_sounds_ff[88];

void load_note_sounds()
{
    for (int i = 0; i < 88; ++i)
    {
        char filepath[256];

        sprintf(filepath, "../assets/samples/Note_PP_%02d.wav", i);
        note_sounds_pp[i] = LoadSound(filepath);

        sprintf(filepath, "../assets/samples/Note_MP_%02d.wav", i);
        note_sounds_mp[i] = LoadSound(filepath);

        sprintf(filepath, "../assets/samples/Note_MF_%02d.wav", i);
        note_sounds_mf[i] = LoadSound(filepath);

        sprintf(filepath, "../assets/samples/Note_FF_%02d.wav", i);
        note_sounds_ff[i] = LoadSound(filepath);
    }
}

void unload_note_sounds()
{
    for (int i = 0; i < 88; ++i)
    {
        //if (note_sounds_pp[i]) UnloadSound(note_sounds_pp[i]);
        //if (note_sounds_mp[i]) UnloadSound(note_sounds_mp[i]);
        //if (note_sounds_mf[i]) UnloadSound(note_sounds_mf[i]);
        //if (note_sounds_ff[i]) UnloadSound(note_sounds_ff[i]);

        UnloadSound(note_sounds_pp[i]);
        UnloadSound(note_sounds_mp[i]);
        UnloadSound(note_sounds_mf[i]);
        UnloadSound(note_sounds_ff[i]);
    }
}

void play_note_sound(unsigned int pitch, unsigned int velocity)
{
    if (pitch >= 88) return;

    Sound* sound_to_play = nullptr;

    if (velocity == 0) {
        sound_to_play = &note_sounds_pp[pitch];
    }
    else if (velocity == 1) {
        sound_to_play = &note_sounds_mp[pitch];
    }
    else if (velocity == 2) {
        sound_to_play = &note_sounds_mf[pitch];
    }
    else {
        sound_to_play = &note_sounds_ff[pitch];
    }

    if (sound_to_play != nullptr)
    {
        StopSound(*sound_to_play);
        PlaySound(*sound_to_play);
    }
}

void stop_note_sound(unsigned int pitch)
{
    if (pitch >= 88) return;

    StopSound(note_sounds_pp[pitch]);
    StopSound(note_sounds_mp[pitch]);
    StopSound(note_sounds_mf[pitch]);
    StopSound(note_sounds_ff[pitch]);
}