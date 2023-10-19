#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include "Seeed_Arduino_GroveAI.h"
#include <Wire.h>
#include <Adafruit_NeoPixel.h>


#define NUM_LEDS 16
#define LED_PIN D5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


GroveAI ai(Wire);
uint8_t state = 0;

const char* ssid = "Donna's hotspot";
const char* password = "rghr7199";


// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "d8ee78b75b354532839ec85f79cfaf29";
// Example:
//String openWeatherMapApiKey = "bd939aa3d23ff33d3c8f5dd1dd4";

// Replace with your country code and city
String city = "Amsterdam";
String countryCode = "NL";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Begin");
  strip.begin();
  strip.show();

  if (ai.begin(ALGO_OBJECT_DETECTION, (MODEL_INDEX_T)0x01)) {
    Serial.print("Version: ");
    Serial.println(ai.version());
    Serial.print("ID: ");
    Serial.println(ai.id());
    Serial.print("Algo: ");
    Serial.println(ai.algo());
    Serial.print("Model: ");
    Serial.println(ai.model());
    Serial.print("Confidence: ");
    Serial.println(ai.confidence());
    state = 1;
  } else {
    Serial.println("Algo begin failed.");
  }


  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
}

const int dry = 700; 
const int wet = 200; 


bool isPlantHealthy(String detectedObject) {
  String healthyLabels[] = {
    "Tomato leaf",
    "Apple leaf",
    "Blueberry leaf",
    "Cherry leaf",
    "Grape leaf",
    "Peach leaf",
    "Potato leaf",
    "Raspberry leaf",
    "Soyabean leaf",
    "Soybean leaf",
    "Squash Powdery mildew leaf",
    "Strawberry leaf"
  };

  for (int i = 0; i < sizeof(healthyLabels) / sizeof(healthyLabels[0]); i++) {
    if (detectedObject == healthyLabels[i]) {
      return true;
    }
  }

  return false;
}

String getObjectLabel(uint8_t target) {
  switch (target) {
    case 0:
      return "Tomato leaf";
    case 1:
      return "Apple leaf";
    case 2:
      return "Blueberry leaf";
    case 3:
      return "Cherry leaf";
    case 4:
      return "Grape leaf";
    case 5:
      return "Peach leaf";
    case 6:
      return "Potato leaf";
    case 7:
      return "Raspberry leaf";
    case 8:
      return "Soyabean leaf";
    case 9:
      return "Soybean leaf";
    case 10:
      return "Squash Powdery mildew leaf";
    case 11:
      return "Strawberry leaf";
    default:
      return "Unknown object";
  }
}

void loop() {
  String detectedObject = "Unknown object"; 
  if (state == 1) {
    uint32_t tick = millis();
    if (ai.invoke()) {
      while (1) {
        CMD_STATE_T ret = ai.state();
        if (ret == CMD_STATE_IDLE) {
          break;
        }
        delay(20);
      }

      uint8_t len = ai.get_result_len();
      if (len) {
        int time1 = millis() - tick;
        Serial.print("Time consuming: ");
        Serial.println(time1);
        Serial.print("Number of plants: ");
        Serial.println(len);
        object_detection_t data;

        for (int i = 0; i < len; i++) {
          Serial.println("result: detected");
          Serial.print("Detecting and calculating: ");
          Serial.println(i + 1);
          ai.get_result(i, (uint8_t*)&data, sizeof(object_detection_t));

          Serial.print("confidence:");
          Serial.print(data.confidence);
          Serial.println();

          detectedObject = getObjectLabel(data.target);

          if (isPlantHealthy(detectedObject)) {
            Serial.println("The plant is healthy");
          } else {
            Serial.println("The plant is not healthy.");
          }
        }
      } else {
        Serial.println("No identification");
      }
    } else {
      delay(1000);
      Serial.println("Invoke Failed.");
    }
  } else {
    state = 0;
  }

  int sensorVal = analogRead(A0);
  int percentageHumidity = map(sensorVal, wet, dry, 100, 0);

  bool needsWater = false;

  if (percentageHumidity < 30) { 
    needsWater = true;
  }

  if (WiFi.status() == WL_CONNECTED) {
    String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
    String jsonBuffer = httpGETRequest(serverPath.c_str());
    JSONVar myObject = JSON.parse(jsonBuffer);

    if (JSON.typeof(myObject) != "undefined") {
      double humidity = myObject["main"]["humidity"];
      double temperature = myObject["main"]["temp"];

      if (humidity < 30 && temperature > 30) { 
        needsWater = true;
      }
    }
  } else {
    Serial.println("WiFi Disconnected");
  }

  int waterNeedScore = 0;
  if (!isPlantHealthy(detectedObject)) {
    waterNeedScore++;
  }
  if (needsWater) {
    waterNeedScore++;
  }

if (waterNeedScore == 0) {
    Serial.println("Plant is well-hydrated (Green light)");
    setLEDColor(0, 255, 0); 
} else if (waterNeedScore == 1) {
    Serial.println("Plant might need watering soon (Orange light)");
    setLEDColor(255, 69, 0); 
} else {
    Serial.println("Plant needs immediate watering (Red light)");
    setLEDColor(255, 0, 0); 
}
delay(100);
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

void setLEDColor(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}



