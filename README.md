# CC-to-MIDI-clock-sync
Reads a specific MIDI channel, CC#, and value every 1/16 note, counts the times, divides by 6, and return MIDI sync messages at 24ppqn
Change the parameters: Channel , CCnum, and CCval to your liking.     CODE_OVERHEAD_TIME could prob be tweaked a bit if things seems a little off.    

hardware:   Arduino of choice w/ appropriate MIDI In and Out circutry
