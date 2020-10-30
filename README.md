# JUCE Metronome

Audio player application. Simple open/play/stop interface for controlling the playback of audio files. Supports WAV, AIFF and MP3. Built using the [JUCE](https://github.com/juce-framework/JUCE) 
audio application framework.

Based on the excellent tutorials by [The Audio Programmer](https://github.com/TheAudioProgrammer/metronome) and [JUCE](https://docs.juce.com/master/tutorial_playing_sound_files.html).

## Build Environment
* JUCE v6.0.4
* Ubuntu 18.04.5 LTS

## Build
```bash
cd Builds/LinuxMakefile
make
```

## Run
```bash
cd Builds/LinuxMakefile/build
./AudioPlayer
```