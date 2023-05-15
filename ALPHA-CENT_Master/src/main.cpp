#include <Arduino.h>
#include <ArduinoJson.h>
// #include "RFID_command.h"
// #include "Unit_UHF_RFID.h"
#include "FS.h"
#include <LITTLEFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HTTPClient.h>


// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
char ssid[15];  // = "EMxxx";
const char *password = "1234abcd";
const char *SW_VERSION = "v1.0";

bool SEND_DATA_TO_SERVERF = false;

#define LED_ON 1
#define LED_OFF 0

#define ENABLE_DEBUG6

#define SW_DEFAULT    39  // Input 3
#define BUILTIN_LED   2

#define RLON LOW
#define RLOFF HIGH

#define RL1 15
#define RL2 4
#define RL3 26
#define RL4 13
 

bool CONFIG_WEBF = false;

unsigned int SEC_Network, MIN_Network;

#include "global.h"
#include "littlefs_do.h"
#include "webserver_do.h"
#include "TFT_do.h"


String strs_old[10];
String strs[10];
int StringCount = 0;

void split_string(String str) {
  // Split the string into substrings
  while (str.length() > 0) {
    int index = str.indexOf(',');
    if (index == -1) // No comma found
    {
      strs[StringCount++] = str;
      break;
    }
    else
    {
      strs[StringCount++] = str.substring(0, index);
      str = str.substring(index+1);
    }
  }

  // Show the resulting substrings
    for (int i = 0; i < StringCount; i++) {
      Serial.print(i);
      Serial.print(": \"");
      Serial.print(strs[i]);
      Serial.println("\"");
    }

    StringCount = 0;
    bool f = 0;

    if (strs_old[0] != strs[0]) { strs_old[0] = strs[0]; f = 1; }
    if (strs_old[1] != strs[1]) { strs_old[1] = strs[1]; f = 1; }
    if (strs_old[2] != strs[2]) { strs_old[2] = strs[2]; f = 1; }
    if (strs_old[3] != strs[3]) { strs_old[3] = strs[3]; f = 1; }
    if (strs_old[4] != strs[4]) { strs_old[4] = strs[4]; f = 1; }
    if (strs_old[5] != strs[5]) { strs_old[5] = strs[5]; f = 1; }
    if (strs_old[6] != strs[6]) { strs_old[6] = strs[6]; f = 1; }
    if (strs_old[7] != strs[7]) { strs_old[7] = strs[7]; f = 1; }

    if (f == 1) {
      f = 0;
      config.out1on = strs[0].toFloat();
      config.out2on = strs[1].toFloat();
      config.out3on = strs[2].toFloat();
      config.out4on = strs[3].toFloat();
      config.out1off = strs[4].toFloat();
      config.out2off = strs[5].toFloat();
      config.out3off = strs[6].toFloat();
      config.out4off = strs[7].toFloat();
      Serial.println("Write setpoint");
      WriteConfig();
    }
}


void get_server_data() {
  if(WiFi.status()== WL_CONNECTED){
    //WiFiClient client;
    HTTPClient http;

    //http.begin(HOST_NAME + PATH_NAME + queryString); //HTTP       --> this code is OK
    http.begin(config.server_get_address + "?id=" + config.uuid); //HTTP
    
    int httpCode = http.GET();

    Serial.println(config.server_get_address + "?id=" + config.uuid);

    // httpCode will be negative on error
    if(httpCode > 0) {
      // file found at server
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT) {    // 200 or 204
        String payload = http.getString();
        Serial.println(payload);

        split_string(payload);

      } else {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      }
      
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
      
    // Free resources
    http.end();
  }
}



void post_data(String queryString) {
  if(WiFi.status()== WL_CONNECTED){
    //WiFiClient client;
    HTTPClient http;

    //http.begin(HOST_NAME + PATH_NAME + queryString); //HTTP       --> this code is OK
    http.begin(config.server_address + "?token=SumTechIoT&id=" + config.uuid + "&" + queryString); //HTTP
    
    int httpCode = http.GET();

    Serial.println(config.server_address + "?token=SumTechIoT&id=" + config.uuid + "&" + queryString);

    // httpCode will be negative on error
    if(httpCode > 0) {
      // file found at server
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_NO_CONTENT) {    // 200 or 204
        String payload = http.getString();
        Serial.println(payload);
      } else {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      }
      
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    
    //Serial.print("HTTP Response code: ");
    //Serial.println(httpResponseCode);
      
    // Free resources
    http.end();

    delay(2000);
    get_server_data();
  }
}


void send_data_to_server (float tempv) {

  String httpRequestData = "TEMP=" + String(tempv);
          httpRequestData += "&HT1=";
          if (digitalRead(RL1) == RLON) httpRequestData += "1"; else httpRequestData += "0";
          httpRequestData += "&HT2=";
          if (digitalRead(RL2) == RLON) httpRequestData += "1"; else httpRequestData += "0";
          httpRequestData += "&HT3=";
          if (digitalRead(RL3) == RLON) httpRequestData += "1"; else httpRequestData += "0";
          httpRequestData += "&HT4=";
          if (digitalRead(RL4) == RLON) httpRequestData += "1"; else httpRequestData += "0";

    post_data(httpRequestData);
}


void update_TFT(float tempv) {
  tft.setTextSize(2);
  tft.setCursor(335, 2);
  tft.setTextColor(TFT_LIGHTGREY);
  tft.print(ssid);

  tft.fillRect(2, 2, 320, 15, TFT_WHITE);

  if (WiFi.status() == WL_CONNECTED) {
    tft.setCursor(2, 2);
    tft.setTextColor(TFT_GREEN);
    tft.print("Connected to WiFi");
  } else {
    tft.setCursor(2, 2);
    tft.setTextColor(TFT_ORANGE);
    tft.print("WiFi not connect!");
  }

  tft.setTextSize(7);
  

  tft.fillRect(130, 103, 252, 78, TFT_WHITE);
  tft.setTextColor(TFT_SKYBLUE);
  tft.setCursor(165, 103);                          // height-1
  tft.print(tempv);


}


//TaskHandle_t Get_Tall_Handle;


void relay_on(uint8_t relay) {
  switch(relay) {
    case 1: digitalWrite(RL1, RLON); 
            tft.fillRect(17, 256, 90, 48, TFT_WHITE);
            PNG_XPOS = 17;
            PNG_YPOS = 256;
            drawPNGimage("/heater_on_90px.png");
          break;

    case 2: digitalWrite(RL2, RLON); 
            tft.fillRect(134, 256, 90, 48, TFT_WHITE);
            PNG_XPOS = 134;
            PNG_YPOS = 256;
            drawPNGimage("/heater_on_90px.png");
          break;
    case 3: 
            digitalWrite(RL3, RLON);
            tft.fillRect(251, 256, 90, 48, TFT_WHITE);
            PNG_XPOS = 251;
            PNG_YPOS = 256;
            drawPNGimage("/heater_on_90px.png");
            break;
    case 4: 
            digitalWrite(RL4, RLON);
            tft.fillRect(369, 256, 90, 48, TFT_WHITE);
            PNG_XPOS = 369;
            PNG_YPOS = 256;
            drawPNGimage("/heater_on_90px.png");
            break;
  }

  SEND_DATA_TO_SERVERF = true;
}

void relay_off(uint8_t relay) {
  switch(relay) {
    case 1: 
            digitalWrite(RL1, RLOFF);
            tft.fillRect(17, 256, 90, 48, TFT_WHITE);
            PNG_XPOS = 17;
            PNG_YPOS = 256;
            drawPNGimage("/heater_off_90px.png");
            break;
    case 2: 
            digitalWrite(RL2, RLOFF);
            tft.fillRect(134, 256, 90, 48, TFT_WHITE);
            PNG_XPOS = 134;
            PNG_YPOS = 256;
            drawPNGimage("/heater_off_90px.png");
            break;
    case 3: 
            digitalWrite(RL3, RLOFF);
            tft.fillRect(251, 256, 90, 48, TFT_WHITE);
            PNG_XPOS = 251;
            PNG_YPOS = 256;
            drawPNGimage("/heater_off_90px.png");
            break;
    case 4: 
            digitalWrite(RL4, RLOFF);
            tft.fillRect(369, 256, 90, 48, TFT_WHITE);
            PNG_XPOS = 369;
            PNG_YPOS = 256;
            drawPNGimage("/heater_off_90px.png");
            break;
  }

  SEND_DATA_TO_SERVERF = true;
}



// void Check_Temperature_Task(void *pvParameters) {
//   while(1) {

    
//     #if defined(ENABLE_DEBUG)
//       Serial.print("Distant1 = ");
//       Serial.print(duration1);
//       Serial.print("\t\tDistant2 = ");
//       Serial.println(duration2);
//     #endif

//     vTaskDelay(500 / portTICK_PERIOD_MS); // work
//   }
// }

// void Send_Server_Task(void *pvParameters) {
//   while(1) {

//     if (digitalRead(WATER_BIT) == LOW && COUNT_FLOWF == true) {
//         COUNT_FLOWF = false;
//         PULSE_COUNT++;
//         digitalWrite(WATER_FLOW_LED, HIGH);
//     }  else if (digitalRead(WATER_BIT) == HIGH) {
//         COUNT_FLOWF = true;
//         digitalWrite(WATER_FLOW_LED, LOW);
//     }
    
//     #if defined(ENABLE_DEBUG)
//       Serial.print("Pulse count = ");
//       Serial.println(PULSE_COUNT);
//       +
//     #endif

//     vTaskDelay(500 / portTICK_PERIOD_MS); // work
//   }
// }


void checkSWDefault() {
  int NN = 0;

  if (digitalRead(SW_DEFAULT) == LOW) {
    while(digitalRead(SW_DEFAULT) == LOW) {
      Serial.print("#");
      NN++;
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(500);
      if (NN == 8) {                      // LED Blink 5 times
        NN = 0;
        
        //SetDefault();

        while(!digitalRead(SW_DEFAULT)) { 
          digitalWrite(LED_BUILTIN, LED_ON);
        }
        digitalWrite(LED_BUILTIN, LED_OFF);
      }
    }
  }
}


void setup() {

  pinMode(SW_DEFAULT, INPUT_PULLUP);
  pinMode(BUILTIN_LED, OUTPUT);

  pinMode(RL1, OUTPUT);
  pinMode(RL2, OUTPUT);
  pinMode(RL3, OUTPUT);
  pinMode(RL4, OUTPUT);

  relay_off(1);
  relay_off(2);
  relay_off(3);
  relay_off(4);

  digitalWrite(BUILTIN_LED, LOW);

  Serial.begin(115200);

  InitFilesystem();
  ReadConfig();

  getEfuseMac();

  sensors.begin();
  sensors.getTempCByIndex(0);


  getEfuseMac();

  tft.init();
  //tft.initDMA();

  // Set the rotation before we calibrate
  tft.setRotation(1);

  // Calibrate the touch screen and retrieve the scaling factors
  //touch_calibrate();

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.print("CPU start...");
  delay(1000);

  
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(2);
  tft.setCursor(350, 2);
  tft.setTextColor(TFT_LIGHTGREY);
  tft.print(ssid);

  PNG_XPOS = 0;
  PNG_YPOS = 0;
  drawPNGimage("/16_pig_heater.png");

  PNG_XPOS = 17;
  PNG_YPOS = 256;
  drawPNGimage("/heater_off_90px.png");

  PNG_XPOS = 134;
  PNG_YPOS = 256;
  drawPNGimage("/heater_off_90px.png");

  PNG_XPOS = 251;
  PNG_YPOS = 256;
  drawPNGimage("/heater_off_90px.png");

  PNG_XPOS = 369;
  PNG_YPOS = 256;
  drawPNGimage("/heater_off_90px.png");


  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
 
  Serial.print("ESP32 IP as soft AP: ");
  Serial.println(WiFi.softAPIP());
 
  Serial.print("ESP32 IP on the WiFi network: ");
  Serial.println(WiFi.localIP());
 
  server.begin();


  scanNetwork();					// สแกนทิ้งก่อนครั้งนึง ครั้งต่อไปถึงจะสแกนเจอ
  ConfigureServer();


   if (ConfigureWifi()) {
     Serial.println("ConfigWifi OK WIFI_AP_STA");
     //sendNotification(0);						// message_id = 0 --> "Controller restart!"
   } else {
  //   Serial.println("ConfigWifi ???");
  //   //LINE.notify(config.m2);
   }


}


void checkNetwork() {
  //if (COUNTER < COUNTEROK) COUNTER++;

  SEC_Network++;

  if (SEC_Network == 60) {
			SEC_Network = 0;
			MIN_Network++;
			if (MIN_Network >= 1) {									// 1 minute re-connect WiFi
				MIN_Network = 0;
				
        if (WiFi.status() != WL_CONNECTED) {			// recheck wifi / ethernet
          //COUNTER = 0;
          ConfigureWifi();
        }
				
			} // end if MIN
		} // end if SEC
}


void check_temp(float tempv) {
  if (tempv <= config.out1on && digitalRead(RL1) == RLOFF) relay_on(1);
  else if (tempv >= config.out1off && digitalRead(RL1) == RLON) relay_off(1);

  if (tempv <= config.out2on && digitalRead(RL2) == RLOFF) relay_on(2);
  else if (tempv >= config.out2off && digitalRead(RL2) == RLON) relay_off(2);

  if (tempv <= config.out3on && digitalRead(RL3) == RLOFF) relay_on(3);
  else if (tempv >= config.out3off && digitalRead(RL3) == RLON) relay_off(3);

  if (tempv <= config.out4on && digitalRead(RL4) == RLOFF) relay_on(4);
  else if (tempv >= config.out4off && digitalRead(RL4) == RLON) relay_off(4);
}


unsigned long currentMillisRunTime, previousMillisRunTime;
uint8_t OUT_RANGE_CC = 0;
int NN;
long SEC_COUNT = 0; 


void loop() {
  
  float tempv;
  currentMillisRunTime = millis();
    
  if (currentMillisRunTime - previousMillisRunTime >= 1000) {
      previousMillisRunTime = currentMillisRunTime;

      //checkSWDefault();

      //get_Distant();
      //checkNetwork();
      
      tempv = sensors.getTempCByIndex(0);
      sensors.requestTemperatures(); //อ่านข้อมูลจาก library
      Serial.print("Temperature is: "); 
      Serial.println(tempv); // แสดงค่า อูณหภูมิ sensor 0

      // NN++;
      // if (NN > 8) NN = 1;                  // loop for test relay

      // if (NN == 1) relay_on(1);
      // else if (NN == 2) relay_on(2);
      // else if (NN == 3) relay_on(3);
      // else if (NN == 4) relay_on(4);
      // else if (NN == 5) relay_off(1);
      // else if (NN == 6) relay_off(2);
      // else if (NN == 7) relay_off(3);
      // else if (NN == 8) relay_off(4);

      check_temp(tempv);

      update_TFT(tempv);


      if (SEC_COUNT++ >= config.SEND_DATA_EVERY || SEND_DATA_TO_SERVERF == true) {
          SEND_DATA_TO_SERVERF = false;
          SEC_COUNT = 0;
          Serial.println("Send data to server...");
          send_data_to_server(tempv);   
      }


      // if (SEC_COUNT++ >= 5) {                           // Test
      //     SEC_COUNT = 0;
      //     Serial.println("Get data from server...");
      //     get_server_data();   
      // }


  }
}