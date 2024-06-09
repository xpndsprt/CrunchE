Find Schematics at http://toggleboss.com, this is the Arduno Sketch "OS"

Crunch-E is a keychain form factor music-making platform that is both limited and limitless. The current software supports 4 tracks, 10 synth instruments, and 2 drum machine banks, but who knows where developers will take the open-source software next?

**DIY Build Parts:**

	ESP32 Dev Board
	Max98357 Breakout Board
	Arduino Compatible Keypad
	4 LEDs + Required Resistors

**CrunchOS Usage Instructions:**


Crunch-e is a sampler/tracker inspired by the Mod trackers of the 90s. It's easily built from inexpensive modules that an Arduino enthusiast might have on hand. Each track can play a single note of any instrument at a given time, resulting in 4-voice polyphony. The keypad handles a single press at a time, and any instructions below assume you press the function button first, release it, and then press another button.

**Function Buttons**


Once the function button is pressed, it will light up the LED associated with that button. While the LED is lit (not blinking), you can make your function selection (like change tracks, volume, instrument, etc).

**New Song**


Press F4, then press C or C# to start a new song with short or long patterns (32 and 64 notes per pattern).

**Play/Stop**


Press F4, then press F4 again to stop or resume your song.

**BPM**


Press F4, then press G# through B to change the BPM of your song.

**Instrument**


Press F1, then press any of the note buttons to change the instrument.

**Octave**


Press F1, then press any of the function buttons to change the octave. For bass, choose octave -1 (F1, then F1).

**Track Volume / Overdrive**


Press F2, then press any of the function buttons to change the volume. Press F2, then F4 to overdrive the sound, giving it a bit of distortion.

**Changing Tracks**


Press F3, then press any of the function buttons to change tracks. You might want to record drums on track one, bass on track two, etc. Remember, only one note per track can be played at a time, so for concurrent sounds, you need to record them on separate tracks.

**Clearing Track**


Press F3, then press E# through G to clear your track.

**Instrument Length**


For non-drum kit instruments, the length can be changed by pressing F4, then any other function button.

**Envelope/Delay/Effects**


Press F2 and any buttons in the bottom three rows.

**Copy/Paste Patterns, Switch Patterns, and Play All Patterns in the Song**


You can use up to four patterns. Switching between patterns is done by pressing F3, then E through G. You can copy a pattern by pressing F4, then E, and paste it by pressing F4, then F. For example, if you’re on pattern 1 and make a drum track, copy it, switch to pattern 2 (which will be empty), and then paste your first pattern into the second one. Now you have a second variation of your first pattern. Fill up all four patterns, and you can press F4, then D# to switch between single pattern and multi-pattern “song” modes. Crunch-e will play all patterns sequentially in song mode.

**Samples**


Samples are stored in the “Samples” folder of the Arduino project. They’re uncompressed raw files exported as signed 16-bit int, 22kHz from Audacity. If overwriting samples, be mindful of the extremely limited memory afforded by program space. In the future, we might add functionality for flash storage or using the ESP32’s internal flash, but for now, samples are in program memory.

**Have fun building, exploring, and making music!**
