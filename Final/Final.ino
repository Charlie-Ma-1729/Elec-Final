#define C3 131
#define C3s 139
#define D3 147
#define D3s 155.5
#define E3 165
#define F3 175
#define F3s 185.5
#define G3 196
#define G3s 207.5
#define A 220
#define A3s 234
#define B3 247
#define C4 262
#define D4 294
#define E4 330
#include <DIYables_IRcontroller.h>
#define IR_RECEIVER_PIN 19 // The ESP32 pin GPIO19 connected to IR controller

DIYables_IRcontroller_21 irController(IR_RECEIVER_PIN, 200); // debounce time is 200ms

int melody[162] = {
  E3, F3, G3, A, 0, E3, 0, G3, 0, G3s, A, 0, E3, F3, G3, A, 0, B3, 0, B3, 0, A3s, A, 0, D3, E3, F3, G3, 0, D3, 0, F3, 0, F3s, G3, 0, D3, E3, F3, G3, 0, A, 0, A, 0, G3s, G3, 0, E3, F3, G3, A, 0, E3,0, G3, 0, G3s, A, 0, E3, F3, G3, A, A3s, A, 0, G3s, A, B3, C4, D4, 0, A, B3, C4, D4, E4, C4,0, G3, E3, 0, G3, E3, 0, G3, E3, 0, G3, G3, A, G3, E3, F3, 0, F3, D3, 0, F3, D3, 0, F3, D3, 0, F3, A, B3, A, F3, E3, 0, G3, E3, 0, G3, E3, 0, G3, E3, 0, G3, C4, D4, C4, A3s, A, 0, C4, G3s, 0, C4, G3s, 0, C4, G3s, 0, G3s, G3s, C4, A3s, G3s, G3, 0, E3, F3, G3, A, E3, E3, F3, 0, E3, F3, 0, E3, G3,0, E3, D3s, D3, C3  
};

int nD[162] = {
6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,2,5,5,5,5,5,5,1,6,6,6,6,6,6,6,6,6,6,2,3,1,1,6,3,6,6,3,6,6,3,6,6,6,6,6,6,2,1,6,3,6,6,3,6,6,3,6,6,6,6,6,6,2,1,6,3,6,6,3,6,6,3,6,6,6,6,6,6,2,1,6,3,6,6,3,6,6,3,6,6,6,6,6,6,2,6,6,6,6,2,2,6,3,6,6,3,6,6,3,6,6,6,3,1
};
 

void m() {
  for (int tN = 0; tN < 162; tN++) {
    Key21 key = irController.getKey();
    if(key == Key21::NONE)
    {
    int noteD = 1000 / nD[tN];
    tone(5, melody[tN], noteD);
    int pBN = noteD * 1.3;
    delay(pBN);
    noTone(5);
    }
    
    else if(key!=Key21::KEY_PREV)
    {
      Serial.println("音樂被中斷");
      break;
    }
  }

}

void setup() {
  Serial.begin(9600);
  irController.begin();
}



void loop() {
  Key21 key = irController.getKey();
  if (key != Key21::NONE) {
    switch (key) {
      case Key21::KEY_CH_MINUS:
        Serial.println("POWER");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_CH:
        Serial.println("MODE");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_CH_PLUS:
        Serial.println("MUTE");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_PREV:
        Serial.println("⏯️");
        m();
        break;

      case Key21::KEY_NEXT:
        Serial.println("⏮️");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_PLAY_PAUSE:
        Serial.println("⏭️");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_VOL_MINUS:
        Serial.println("EQ");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_VOL_PLUS:
        Serial.println("-");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_EQ:
        Serial.println("+");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_100_PLUS:
        Serial.println("🔄️");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_200_PLUS:
        Serial.println("U/SD");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_0:
        Serial.println("0");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_1:
        Serial.println("1");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_2:
        Serial.println("2");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_3:
        Serial.println("3");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_4:
        Serial.println("4");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_5:
        Serial.println("5");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_6:
        Serial.println("6");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_7:
        Serial.println("7");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_8:
        Serial.println("8");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_9:
        Serial.println("9");
        // TODO: YOUR CONTROL
        break;

      default:
        Serial.println("WARNING: undefined key:");
        break;
    }
  }
}
