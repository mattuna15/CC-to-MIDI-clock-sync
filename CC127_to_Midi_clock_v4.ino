/*   CC to MIDI Clock Sync.    Reads a specific Midi CC # every 1/16, counts the time, divides it by 6 and returns midi sync commands
derived from : http://www.glacialcommunications.com/news/2009/12/arduino-trigger-to-midi-converter/   
Charles Hobbs 2016  */



#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
MIDI_CREATE_DEFAULT_INSTANCE();


int Channel = 5;  // MIDI Channel = #1-#16
int CCnum = 127;  //  CC Number   = CC#1-CC#127
int CCval = 15;  // CC value to trigger   0-126 = #1-#127 

/* for 16th note triggers (4ppqn, like a DR-110 hitting accent on every step),
this should be 6. adjust accordingly. PPQN*DIVIDE_SLICE=24 */
#define DIVIDE_SLICE 6
/* attempting to account for code execution, MIDI clock slowness on the receiver end,
Arduino clock and/or cosmic rays. This setting lost about 10ms over 5 minutes using trigger from a DR-110
MIDI Clock in on a Yamaha RY-10, with an RBBB using an ATMega168 and resonator clock. YM_W_V.
*/
#define CODE_OVERHEAD_TIME 10

volatile int trigger=0;
volatile unsigned long triggertime=0;
unsigned long lasttrig=0;
unsigned long nextsync=0;
unsigned long timediff=0;
unsigned long now=0;
int timeslice=0;
int started=0; //1= in the playing state. 0= in the stopped state,


void HandleControlChange (byte channel, byte number, byte value){   
  if ((number == CCnum) && (value >= CCval)){
      trigger=1;
      triggertime=millis();
  }
}

void sendStartA(){
     started=1;
     digitalWrite(13, HIGH); 
}

void sendStopA(){
     started=0;
     digitalWrite(13, LOW); 
}

void setup() {
  MIDI.begin(Channel);
  MIDI.setHandleControlChange (HandleControlChange);
  MIDI.setHandleStart(sendStartA);
  MIDI.setHandleStop(sendStopA);
  timediff=millis();
  lasttrig=millis();

}

void loop() {
  MIDI.read();

  if(trigger==1){
     
       timediff=triggertime-lasttrig;
       lasttrig=triggertime;
       timeslice=timediff/DIVIDE_SLICE;
       nextsync=millis()+timeslice-CODE_OVERHEAD_TIME;
       trigger=0;
    }
    
    if (started==1){  
     //in the world of STOP_TIMEOUT, is the next line needed?
     // if(timeslice > 1000) {timeslice=1000;}
      now=millis();  //I'm not convinved millis is an atomic-ish operation...
      
      if (now >= nextsync){
        syncSignal(0xF8);
        nextsync=now+timeslice;
       }
    }
}


void syncSignal(char cmd){
 // if(debug){
 // Serial.print("sync:");
 // Serial.println(millis());
  //} else {
  Serial.print(cmd);// Serial.print(cmd, BYTE);   =original line
 //} 
}  
