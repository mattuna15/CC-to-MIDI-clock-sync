#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);



// Let's define the MIDI channel
#define MIDI_CHANNEL 1
// The LED we're going to light
//#define LED 13
// The PCs we'll handle, can be changed

// The CC we'll handle, can be changed
#define CC_TOGGLE 127
//The CC values we'll handle, can be changed
#define CC_TOGGLE_ON 127
#define CC_TOGGLE_OFF 0

int pc;
int cc;
int ccValue;

////
#define STOP_TIMEOUT 2000
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
int started=0; //1= in the playing state. 0= in the st


void setup() {
  // Starting to listen on the specified MIDI channel
  MIDI.begin(MIDI_CHANNEL);
 // pinMode(LED, OUTPUT);
//////////
 timediff=millis();
  lasttrig=millis();
}

void loop() {
  
  if (MIDI.read()) {
    // Get the type of the message we caught
    switch(MIDI.getType()) {
      // If it is a Control Change
      case ControlChange:
        cc = MIDI.getData1();
        if (cc == CC_TOGGLE) {
          ccValue = MIDI.getData2();
           if (ccValue == CC_TOGGLE_ON) {
           
            
            if(trigger==1){
      if (started==0){
        //if we're stopped, start!
        syncSignal(0xFA);
        started=1;
        digitalWrite(13, HIGH);
      }
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
        if ((now-lasttrig) > STOP_TIMEOUT){
          //if it's been too long since we've seen a trigger, stop!
          syncSignal(0xFC);
          started=0;
          digitalWrite(13, LOW); 
        }
      }
    }
          }
        }
        break;
      default:
        break;
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
