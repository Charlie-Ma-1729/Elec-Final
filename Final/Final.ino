#include <SimpleDHT.h>

#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

#include <WiFi.h>
#include <HTTPClient.h>
#include <SimpleDHT.h>

//請修改以下參數--------------------------------------------
const char ssid[] = "CMC";           //ssid:網路名稱
const char password[] = "11223344";  //password:網路密碼
//將原本ThingSpeak的網址換成IFTTT網址
String url = "http://maker.ifttt.com/trigger/codo/with/key/WNE8YfGNTdYT1SrpBkz_0";  //改成事件名稱和APIKEY

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
#define IR_RECEIVER_PIN 19  // The ESP32 pin GPIO19 connected to IR controller

DIYables_IRcontroller_21 irController(IR_RECEIVER_PIN, 200);  // debounce time is 200ms


#include <SPI.h> 
#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 

#define SCREEN_WIDTH 128 // OLED 寬度像素
#define SCREEN_HEIGHT 64 // OLED度像素

// 設定OLED
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void testdrawstyles(void) {
  
  display.clearDisplay();
  display.setTextSize(2);          // 設定文字大小
  display.setTextColor(1);         // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  display.setCursor(5,0);          // 設定起始座標
  display.print("Hello OLED");     // 要顯示的字串
  display.setCursor(26,40);        // 設定起始座標
  display.print("MiroTek");        // 要顯示的字串
  display.display();               // 要有這行才會把文字顯示出來
  delay(1000);
}


int melody[162] = {
  E3, F3, G3, A, 0, E3, 0, G3, 0, G3s, A, 0, E3, F3, G3, A, 0, B3, 0, B3, 0, A3s, A, 0, D3, E3, F3, G3, 0, D3, 0, F3, 0, F3s, G3, 0, D3, E3, F3, G3, 0, A, 0, A, 0, G3s, G3, 0, E3, F3, G3, A, 0, E3, 0, G3, 0, G3s, A, 0, E3, F3, G3, A, A3s, A, 0, G3s, A, B3, C4, D4, 0, A, B3, C4, D4, E4, C4, 0, G3, E3, 0, G3, E3, 0, G3, E3, 0, G3, G3, A, G3, E3, F3, 0, F3, D3, 0, F3, D3, 0, F3, D3, 0, F3, A, B3, A, F3, E3, 0, G3, E3, 0, G3, E3, 0, G3, E3, 0, G3, C4, D4, C4, A3s, A, 0, C4, G3s, 0, C4, G3s, 0, C4, G3s, 0, G3s, G3s, C4, A3s, G3s, G3, 0, E3, F3, G3, A, E3, E3, F3, 0, E3, F3, 0, E3, G3, 0, E3, D3s, D3, C3
};

int nD[162] = {
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 3, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 5, 5, 5, 5, 5, 5, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 2, 3, 1, 1, 6, 3, 6, 6, 3, 6, 6, 3, 6, 6, 6, 6, 6, 6, 2, 1, 6, 3, 6, 6, 3, 6, 6, 3, 6, 6, 6, 6, 6, 6, 2, 1, 6, 3, 6, 6, 3, 6, 6, 3, 6, 6, 6, 6, 6, 6, 2, 1, 6, 3, 6, 6, 3, 6, 6, 3, 6, 6, 6, 6, 6, 6, 2, 6, 6, 6, 6, 2, 2, 6, 3, 6, 6, 3, 6, 6, 3, 6, 6, 6, 3, 1
};

int pinDHT11 = 15;  //假設DHT11接在腳位GPIO15
//---------------------------------------------------------
SimpleDHT11 dht11(pinDHT11);  //宣告SimpleDHT11物件
byte temperature = 0;
byte humidity = 0;


void setup() {
    Serial.begin(9600);

  // 偵測是否安裝好OLED了
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 一般1306 OLED的位址都是0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay(); // 清除畫面
  testdrawstyles();    // 測試文字
  delay(1000);
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
        DHT();
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
    if (key == Key21::NONE) {
      int noteD = 1000 / nD[tN];
      tone(5, melody[tN], noteD);
      int pBN = noteD * 1.3;
      delay(pBN);
      noTone(5);
    } else if (key != Key21::KEY_PREV) {
      Serial.println("音樂被中斷");
      break;
    }
  }
}





void DHT() {
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("溫度計讀取失敗，錯誤碼="); Serial.println(err); delay(1000);
    return;
  }
  //讀取成功，將溫濕度顯示在序列視窗
  Serial.print("溫度計讀取成功: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println("%H");
  //開始傳送到IFTTT
  Serial.println("啟動網頁連線");
  HTTPClient http;
  //將溫度及濕度以http get參數方式補入IFTTT網址後方
  String url1 = url + "?value1=" + (int)temperature + "&value2=" + (int)humidity;
  //http client取得網頁內容
  http.begin(url1);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK)      {
    //讀取網頁內容到payload
    String payload = http.getString();
    //將內容顯示出來
    Serial.print("網頁內容=");
    Serial.println(payload);
  } else {
    //讀取失敗
    Serial.println("網路傳送失敗");
  }
  http.end();
  delay(5000);
}
