#include <SimpleDHT.h> 

#include <BluetoothSerial.h> 
BluetoothSerial SerialBT; 

#include <WiFi.h>
#include <HTTPClient.h>
#include <SimpleDHT.h>

//請修改以下參數--------------------------------------------
const char ssid[]     = "CMC"; //ssid:網路名稱
const char password[] = "11223344"; //password:網路密碼
//將原本ThingSpeak的網址換成IFTTT網址
String url = "http://maker.ifttt.com/trigger/codo/with/key/i2q8B7hk"; //改成事件名稱和APIKEY

//---------------------------------------------------------


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

#define ROW1 16
#define ROW2 4
#define ROW3 8
#define ROW4 18
#define ROW5 7
#define ROW6 21
#define ROW7 3
#define ROW8 2
#define COL1 19
#define COL2 1
#define COL3 5
#define COL4 22
#define COL5 15
#define COL6 17 
#define COL7 0
#define COL8 6

const int row[] = {
  ROW1, ROW2, ROW3, ROW4, ROW5, ROW6, ROW7, ROW8
 };

const int col[] = {
  COL1,COL2, COL3, COL4, COL5, COL6, COL7, COL8
 };

byte scan[8][8] = {
{1,0,0,0,0,0,0,0},
{0,1,0,0,0,0,0,0},
{0,0,1,0,0,0,0,0},
{0,0,0,1,0,0,0,0},
{0,0,0,0,1,0,0,0},
{0,0,0,0,0,1,0,0},
{0,0,0,0,0,0,1,0},
{0,0,0,0,0,0,0,1}
};

byte music[8][8] = {
{1,1,1,1,0,1,1,1},
{1,1,1,1,0,0,1,1},
{1,1,1,1,0,1,0,1},
{1,1,1,1,0,1,0,1},
{1,1,1,1,0,1,1,1},
{1,1,0,0,0,1,1,1},
{1,0,0,0,0,1,1,1},
{1,1,0,0,1,1,1,1}};

byte smile[8][8] = {
{1,1,1,1,1,1,1,1},
{1,1,0,1,1,0,1,1},
{1,1,0,1,1,0,1,1},
{1,1,0,1,1,0,1,1},
{1,1,1,1,1,1,1,1},
{1,0,1,1,1,1,0,1},
{1,1,0,0,0,0,1,1},
{1,1,1,1,1,1,1,1}};

byte cold[8][8] = {
{0,0,0,1,1,0,0,0},
{0,0,1,0,0,1,0,0},
{0,1,0,1,1,0,1,0},
{1,0,1,0,0,1,0,1},
{1,0,1,0,0,1,0,1},
{0,1,0,1,1,0,1,0},
{0,0,1,0,0,1,0,0},
{0,0,0,1,1,0,0,0}};

byte hot[8][8] = {
{1,1,1,1,1,1,1,1},
{1,1,0,1,0,1,1,1},
{1,1,1,0,1,0,1,1},
{1,1,0,1,0,1,1,1},
{1,1,1,0,1,0,1,1},
{1,0,0,1,0,1,0,1},
{0,1,1,1,1,1,1,0},
{1,0,0,0,0,0,0,1}};

int melody[162] = {
  E3, F3, G3, A, 0, E3, 0, G3, 0, G3s, A, 0, E3, F3, G3, A, 0, B3, 0, B3, 0, A3s, A, 0, D3, E3, F3, G3, 0, D3, 0, F3, 0, F3s, G3, 0, D3, E3, F3, G3, 0, A, 0, A, 0, G3s, G3, 0, E3, F3, G3, A, 0, E3,0, G3, 0, G3s, A, 0, E3, F3, G3, A, A3s, A, 0, G3s, A, B3, C4, D4, 0, A, B3, C4, D4, E4, C4,0, G3, E3, 0, G3, E3, 0, G3, E3, 0, G3, G3, A, G3, E3, F3, 0, F3, D3, 0, F3, D3, 0, F3, D3, 0, F3, A, B3, A, F3, E3, 0, G3, E3, 0, G3, E3, 0, G3, E3, 0, G3, C4, D4, C4, A3s, A, 0, C4, G3s, 0, C4, G3s, 0, C4, G3s, 0, G3s, G3s, C4, A3s, G3s, G3, 0, E3, F3, G3, A, E3, E3, F3, 0, E3, F3, 0, E3, G3,0, E3, D3s, D3, C3  
};

int nD[162] = {
6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,2,3,6,6,6,6,6,6,6,6,6,6,2,5,5,5,5,5,5,1,6,6,6,6,6,6,6,6,6,6,2,3,1,1,6,3,6,6,3,6,6,3,6,6,6,6,6,6,2,1,6,3,6,6,3,6,6,3,6,6,6,6,6,6,2,1,6,3,6,6,3,6,6,3,6,6,6,6,6,6,2,1,6,3,6,6,3,6,6,3,6,6,6,6,6,6,2,6,6,6,6,2,2,6,3,6,6,3,6,6,3,6,6,6,3,1
};

int pinDHT11=36;
SimpleDHT11 dht11;
byte temperature = 0;
byte humidity = 0;


void setup() {
  Serial.begin(9600);
  irController.begin();
  SerialBT.begin("Stuubid");
  Serial.print("開始連線到無線網路SSID:");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
}

void loop() {
  control();
}


void control() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  Key21 key = irController.getKey();
  if (key != Key21::NONE) {
    switch (key) {
      case Key21::KEY_CH_MINUS:
        Serial.println("POWER");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_CH:
        Serial.println("MODE");
        d();
        
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_CH_PLUS:
        Serial.println("MUTE");
        // TODO: YOUR CONTROL
        break;

      case Key21::KEY_PREV:
        Serial.println("⏯️");
        d();
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

void m() {
  for (int tN = 0; tN < 162; tN++) {
    Key21 key = irController.getKey();
    if(key == Key21::NONE)
    {
    showPattern(music);
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

void d(){
  while(1){
  Key21 key = irController.getKey();
  if(key == Key21::NONE){
    DHT();
    if(temperature>30)showPattern(hot);
    else if(temperature<10)showPattern(cold);
    else showPattern(smile);
    delay(100);
  }
  else if(key!=Key21::KEY_PREV)
  {
    Serial.println("停止偵測");
    break;
  }
  }
  
    
}

void showPattern(byte matrix[8][8]){
  for(byte i = 0; i < 8; i++){
    for(byte j = 0; j < 8; j++){
      digitalWrite(row[j], 1 - scan[i][j]);
      digitalWrite(col[j], 1 - matrix[i][j]);
    }
  for(byte j = 0; j < 8; j++){
    digitalWrite(row[j], HIGH);
    digitalWrite(col[j], LOW);
    }
   }
}

void DHT() {
    // start working...
    Serial.println("=============");   
    Serial.println("Sample DHT11 ……");   
    // read without samples

    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
       Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
       return;
    }
    Serial.print("Sample OK: ");   
    Serial.print("Temperature = ");   
    Serial.print((int)temperature);   
    Serial.println("*C, ");
    Serial.print("Humidity = ");   
    Serial.print((int)humidity);   
    Serial.print("%.");   
    //delay(3000);  //每3秒顯示一次
}
