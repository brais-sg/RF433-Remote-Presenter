/**
 * Arduino Leonardo media remote controller
 * Control multimedia using a RF 433 MHz controller
 * 
 * By: Brais Solla González
 */


#include <RCSwitch.h>
//#include "HID-Project.h"
#include "Keyboard.h"


RCSwitch RF433 = RCSwitch();

enum Remote_Button {
  REMOTE_BUTTON_UNK = 0,
  REMOTE_BUTTON_A   = 1,
  REMOTE_BUTTON_B   = 2,
  REMOTE_BUTTON_C   = 4,
  REMOTE_BUTTON_D   = 8,

  REMOTE_BUTTON_ALL = 0xf,
  REMOTE_BUTTON_MAX
};

typedef struct {
   uint32_t code;
   uint32_t key;
} DecodeEntry_t;


DecodeEntry_t codes[] = {
  // Generic 433 MHz with antenna ("Digital Remote Controller")
  {6212648, REMOTE_BUTTON_A},
  {6212644, REMOTE_BUTTON_B},
  {6212642, REMOTE_BUTTON_C},
  {6212641, REMOTE_BUTTON_D},
  // Brown wood-ish remote with antenna 
  {9090408, REMOTE_BUTTON_A},
  {9090404, REMOTE_BUTTON_B},
  {9090402, REMOTE_BUTTON_C},
  {9090401, REMOTE_BUTTON_D},
  // New 433 MHz remote (Hard to put battery into)
  {1484328, REMOTE_BUTTON_A},
  {1484324, REMOTE_BUTTON_B},
  {1484322, REMOTE_BUTTON_C},
  {1484321, REMOTE_BUTTON_D},
  // New remote, ABCD, without external antenna (white)
  {11015124, REMOTE_BUTTON_A},
  {11015121, REMOTE_BUTTON_B},
  {11015128, REMOTE_BUTTON_C},
  {11015122, REMOTE_BUTTON_D},
  // New remote, ABCD with antenna and cover (white)
  {1480216, REMOTE_BUTTON_A},
  {1480212, REMOTE_BUTTON_B},
  {1480210, REMOTE_BUTTON_C},
  {1480209, REMOTE_BUTTON_D},
  // New remote, brown with button covers
  {5913912, REMOTE_BUTTON_A},
  {5913908, REMOTE_BUTTON_B},
  {5913906, REMOTE_BUTTON_C},
  {5913905, REMOTE_BUTTON_D},
  // New 3000m remote (9V, high range)
  {6965928, REMOTE_BUTTON_A},
  {6965924, REMOTE_BUTTON_B},
  {6965922, REMOTE_BUTTON_C},
  {6965921, REMOTE_BUTTON_D}
};

const int numCodes = 28;

uint32_t code2key(uint32_t code){
  for(int i = 0; i < numCodes; i++){
    if(code == codes[i].code){
      return (uint32_t) codes[i].key;
    }
  }
  
  return (uint32_t) REMOTE_BUTTON_UNK;
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  // Serial.begin(115200);
  
  // Consumer.begin();
  Keyboard.begin();
  RF433.enableReceive(digitalPinToInterrupt(2)); // Pin 2 on the Arduino Leonardo / Micro (ATMEGA 32U4)
}

uint32_t highLed = 0;
uint32_t ticksOvf = 0;


void loop() {
  if(RF433.available()){
    digitalWrite(LED_BUILTIN, HIGH);
    highLed = millis() + 100;

    // TODO: Debouncing!
    uint32_t key = code2key(RF433.getReceivedValue());
    switch(key){
      case REMOTE_BUTTON_A:
        //Consumer.write(MEDIA_PLAY_PAUSE);
        Keyboard.write(KEY_LEFT_ARROW);
        delay(100);
        break;
      case REMOTE_BUTTON_B:
        //Consumer.write(MEDIA_STOP);
        Keyboard.write(KEY_RIGHT_ARROW);
        delay(100);
        break;
      case REMOTE_BUTTON_C:
        // Consumer.write(MEDIA_PREVIOUS);
        Keyboard.write(KEY_HOME);
        delay(100);
        break;
      case REMOTE_BUTTON_D:
        // Consumer.write(MEDIA_NEXT);
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_F5);
        Keyboard.releaseAll();
        delay(100);
        break;
      default:
        // Unknown key
        break;
    }

    // Serial.println(RF433.getReceivedValue());
    RF433.resetAvailable();  
  }

  ticksOvf = millis();

  if(ticksOvf > highLed){
    digitalWrite(LED_BUILTIN, LOW);
  }

  if(millis() < ticksOvf){
    // Timer overflow
    ticksOvf = 0;
    highLed = 0;
  }
}
