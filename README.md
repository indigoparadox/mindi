# mindi

Extremely minimal MIDI library, for old or otherwise resource-limited systems.

Currently a work in progress and very volatile.

## Dependencies

- Be sure to check out submodules! maug and mbeep are required for building.
- Check framework, for running tests: `sudo apt install check`
- ALSA development libraries for beep playback on Linux (not required for DOS, see below): `sudo apt install libasound2-dev`

## DOS MIDI Player

(WARNING: Experimental, extremely funny-sounding. Needs work! If you try it right now on a real machine, you will probably have to reboot that machine!)

midibeep is a simple one-track MIDI player that can be cross-compiled for DOS from a modern UNIX-like system. Given a type 1 MIDI file and a track number, it will attempt to beep out that track on the PC speaker. It requires the OpenWatcom compiler to compile.

- Ensure that `wcc` and `wcl` are in your PATH.
- Ensure that the `WATCOM` environment variable is set to your watcom installation directory (e.g. `/opt/watcom`).
- Ensure that the `INCLUDE` environment variable is set to the `h` subdirectory of your watcom installation directory (e.g. `/opt/watcom/h`).
- Execute `make midibeep.exe` in the project root.

## References

- https://web.archive.org/web/20140216225718/http://www.sonicspot.com/guide/midifiles.html
- https://ccrma.stanford.edu/~craig/articles/linuxmidi/misc/essenmidi.html
- http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html#BM3\_1
- https://www.recordingblogs.com/wiki/musical-instrument-digital-interface-midi

Documentation available at: https://indigoparadox.github.io/mindi/
