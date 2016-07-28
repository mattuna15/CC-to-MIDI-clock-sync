# CC-to-MIDI-clock-sync
Add MIDI clock sync to LSDJ/ Arduinoboy MidiOut!  

Reads a specific MIDI channel, CC#, and value every 1/16 note, counts the times, divides by 6, and return MIDI sync messages at 24ppqn
Change the parameters: Channel , CCnum, and CCval to your liking.     CODE_OVERHEAD_TIME could prob be tweaked a bit if things seems a little off.   As-is, it seems to work fine to my ear even after 10-15 mins. 

hardware:   Arduino of choice w/ appropriate MIDI In and Out circutry
