In this manual, we will walk you through the setup of your Arduino and accompanying components for plant identification and health assessment. By utilizing a moisture sensor, a weather API, and AI vision, we can determine a plant's need for water. The results are visualized on an LED strip, where a green light indicates the plant is well-hydrated, orange suggests it might need watering, and red signals an immediate need for water.

***

### Table of Contents

1. [Introduction](#introduction)
2. [Requirements](#requirements)
3. [Drivers](#drivers)
4. [Setup your board](#setup-your-board)
5. [Set up the hardware](#set-up-the-hardware)
6. [Skip to the results](#skip-to-the-results)
7. [Weather API](#weather-api)
8. [Grove - Vision AI](#grove-vision-ai)
9. [Capacitive Soil Moisture Sensor](#capacitive-soil-moisture-sensor)
11. [Combining the Codes](#combining-the-codes)
12. [Result](#result)
13. [Sources](#sources)


***


### Requirements
**Hardware**
* Arduino, for this manual I am using the NodeMCU https://www.seeedstudio.com/NodeMCU-v2-Lua-based-ESP8266-development-kit.html

* Grove - Vision AI Module. https://www.seeedstudio.com/Grove-Vision-AI-Module-p-5457.html

* Capacitive Soil Moisture Sensor https://www.tinytronics.nl/shop/en/sensors/liquid/capacitive-soil-moisture-sensor-module-with-cable

* Ledstrip 

* USB C - USB C cable

* Wires to connect


**Accounts**
* Roboflow account (optional)

* Google Account/ Google drive account 

* OpenWeather account 
***




### Drivers
**MAC**
1. Download the driver from this link: [Silicon Labs CP210x USB-to-UART Bridge VCP Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads).

1. After the driver is downloaded, unzip the VCP file.

1. Open the unpacked file with the .dmg extension. This will open a folder.

1. Inside the opened folder, you will find the installation program for the CP210x VCP Driver. Start the installation by double-clicking it.

1. During the installation, a dialog box may open asking for permission to install. Enter your administrator password to proceed.

1. Upon completion of the installation, go to "System Preferences" on your Mac.

1. Navigate to "Security & Privacy" and click on the "General" tab.

1. If the driver installation was successful, you should see a message indicating that the CP210x VCP Driver is blocked because it's from an unidentified developer.

1. Click the "Allow" button to permit the driver and ensure it can be used.

**WINDOWS**
1. Download the "CP210x VCP Windows" driver from this link: [Silicon Labs CP210x USB-to-UART Bridge VCP Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads).

1. After the download is complete, unzip the downloaded file.

1. Determine whether you need the 64-bit or 32-bit version of the driver. To check if your Windows is 32-bit or 64-bit, follow these steps:

* Right-click on "This PC" or "My Computer" on your desktop or in the Start menu.
* Select "Properties" to open the System properties window.
* Under the "System" section, you'll find the system type (32-bit or 64-bit).
* Once you've determined your system type, run the appropriate ".exe" file included in the downloaded driver package.

4. Follow the installation procedure as prompted.

5. After the installation is complete, you can verify whether the drivers were successfully installed:

* Connect the NodeMCU (via USB) to your laptop.
* Open "Start" and go to "Settings" > "Devices" > "Bluetooth & other devices."
* Under the "Other devices" section, you should see the 'Silicon Labs...' device listed. Make a note of the COM port used.
***

### Setup your board
1. Download and install the Arduino IDE if you do not have it installed already.
2. Go to Preferences > Additional Boards Manager URLs and add the following URL: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`. You can add multiple URLs by separating them with commas.
3. Click "OK."
4. Navigate to the menu Tools > Board > Boards Manager.
5. Look for "esp8266" by ESP8266 Community in the list.
6. Click on "Install."
7. Once the installation is complete, click "OK."


***

### Set up the hardware.

| NodeMCU | Grove - Vision AI module | Ledstrip | Moisture Sensor |
|--------------|--------------|--------------|--------------|
| ![NodeMCU](https://github.com/D0nnaz/DET-IoT/assets/92280247/8ec811e7-76d1-4658-a11b-281f331331c7) [Source image](https://quartzcomponents.com/products/nodemcu-development-board) | ![Grove - Vision AI module](https://github.com/D0nnaz/DET-IoT/assets/92280247/76ccf4bc-f00f-484f-8adb-36e13875b214) [Source image](https://wiki.seeedstudio.com/Grove-Vision-AI-Module/) | ![Ledstrip](https://github.com/D0nnaz/DET-IoT/assets/92280247/33feea9e-ab72-448f-80b4-e5bd5c5aa803) [Source image](https://www.circuitgeeks.com/ws2812b-addressable-rgb-led-strip-with-arduino/) | <img src="https://github.com/D0nnaz/DET-IoT/assets/92280247/c71c666c-25f2-4456-96b2-4c5db7321210" width="5000" alt="Moisture Sensor" /> [Source](https://arduino-projekte.info/produkt/kapazitiver-hygrometer-bodenfeuchtesensor-v2-0-moisture-sensor/)
***


### Skip to the results
In the next sections, you can join me as I walk you through my process. But if you're just looking for the completed code and want to copy and paste it, feel free to skip ahead to [Full Code](#combining-the-codes)).

***

### Weather API
![image](https://github.com/D0nnaz/DET-IoT/assets/92280247/ae6e9a94-097b-47b2-abdd-113584318e43)


1. Open a web browser and go to https://openweathermap.org/appid/.

1. Click on the "Sign up" button and create a free account.

1. Visit this link: https://home.openweathermap.org/api_keys to obtain your API key.

1. On the API keys tab, you will find a default key (highlighted within a red rectangle in the figure above); this is a unique key required for fetching data from the website. Copy and paste this key somewhere, as you'll need it shortly.

1. To retrieve weather information for your chosen location, input the following URL:
1. http://api.openweathermap.org/data/2.5/weather?q=yourCityName,yourCountryCode&APPID=yourUniqueAPIkey
1. Replace yourCityName with the city you want data for, yourCountryCode with the country code for that city, and yourUniqueAPIkey with the unique API key from step 4. For instance, the updated API URL for the city of Amsterdam, NL, would be:
1. http://api.openweathermap.org/data/2.5/weather?q=Amsterdam,NL&APPID=801d2603e9f2e1c70e042e4f5f6e0---

1. Copy and paste your URL into your web browser, and the API will return a wealth of information pertaining to your local weather. We received the following information about the weather in Amsterdam, Netherlands, on the day this tutorial was written.

10. Open a new sketch and paste the following code:

````
/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-http-get-open-weather-map-thingspeak-arduino/
*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include "Seeed_Arduino_GroveAI.h"
#include <Wire.h>

GroveAI ai(Wire);
uint8_t state = 0;

const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "REPLACE_WITH_YOUR_OPEN_WEATHER_MAP_API_KEY";
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



void loop() {
  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

````

11. Fill in these pieces:
* const char* ssid     = "REPLACE_WITH_YOUR_SSID";
* const char* password = "REPLACE_WITH_YOUR_PASSWORD";
* String openWeatherMapApiKey = "REPLACE_WITH_YOUR_OPEN_WEATHER_MAP_API_KEY";

12. Run code
***


$${\color{red}ERROR!}$$

<img width="872" alt="Scherm­afbeelding 2023-10-14 om 14 58 29" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/7ff47e8a-68f7-4303-8ba6-3beadc042084">

If you're encountering a 401 error when trying to access the OpenWeatherMap API, there are several potential reasons why your API key might not be functioning as expected. One key point to note is that it can take up to 2 hours for your API key to become active after obtaining it. Therefore, if you're currently facing this error, you can continue with your work, and by the time you're done, your API key should hopefully be activated and fully functional. If it still doesn't work after 2 hours, double-check your key, ensure you've followed the steps correctly, and verify that you've used the correct country code.

<img width="636" alt="Scherm­afbeelding 2023-10-19 om 02 44 27" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/35560b3a-9117-40dd-adc1-a4efb2cebdc1">

If your output in the serial monitor looks like this, you need to check two things:
1. Verify your Wi-Fi credentials and ensure you are connected to the correct network.
1. Change the baud rate to 115200.

***

### Grove Vision AI
Create a new sketch and paste the following code:

`````
#include "Seeed_Arduino_GroveAI.h"
#include <Wire.h>

GroveAI ai(Wire);
uint8_t state = 0;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  Serial.println("Begin");

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
}

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
      return "Onbekend object";
  }
}

void loop() {
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
        Serial.print("Number of people: ");
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

          String detectedObject = getObjectLabel(data.target);

          if (isPlantHealthy(detectedObject)) {
            Serial.println("De plant is gezond.");
          } else {
            Serial.println("De plant is niet gezond.");
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
}
`````
**IMPORTANT NOTE!!! IF YOU USE A DIFFERENT MODEL THAN MINE YOU WILL NEED TO CHANGE THE LABELS!**


1. Before you continue, please download the ZIP library from https://github.com/Seeed-Studio/Seeed_Arduino_GroveAI. Once the download is finished, open the Arduino IDE and follow these steps:

1. Go to Sketch > Include Library > Add .ZIP Library.
Locate the recently downloaded ZIP file and install it in the Arduino IDE.

1. Insert the USB-C cable into both the Grove - Vision AI and your computer. Press the boot button twice. If everything is done correctly, you will see Grove AI (see image).

| Boot button | Outcome |
|--------------|--------------|
| ![image](https://github.com/D0nnaz/DET-IoT/assets/92280247/94e41c91-8f99-44a0-a60b-e813a6ae8e95) | <img width="900" alt="Scherm­afbeelding 2023-10-09 om 21 31 35" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/3132f831-add4-4792-8f75-4668bfd4accf"> |

**OPTION 1**
Download the following file:

![image](https://github.com/D0nnaz/DET-IoT/assets/92280247/1a4e01fc-0e43-4c16-969a-e4b18d837e73)

1. Drag and drop the model into the AI vision, and once the upload is successful, it will automatically disappear.

1. Upload the .uf2 file to the AI Grove. If done correctly, the Grove AI will disappear within a few seconds.

1. Upload the code to your arduino. Open the serial monitor and set baud rate as 115200 and the result of plants detection should be showed continuously. To see the result, go to this site: https://vision-ai-demo.seeed.cn/. 


Your serial monitor should look something like this: <br>
<img width="374" alt="Scherm­afbeelding 2023-10-19 om 03 43 13" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/ec406fed-2531-4528-a365-27d0114c4b93">


**OPTION 2**
1. Alternatively you can upload you own AI model. 
2. First, visit this link: https://public.roboflow.com/object-detection/plantdoc/2/download/yolov5pytorch (or another datatset of your choice).
3. Next, click "Create Account" to set up your Roboflow account.
4. Click "Download," choose "YOLO v5 PyTorch" as the format, and hit "show download code." Now, click "Continue" to get a code snippet. Keep this window open in the background; we'll need it for Google Colab.

<img width="792" alt="Scherm­afbeelding 2023-10-19 om 03 27 50" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/23a9852b-8055-48fe-b29c-a098e5a3fc66">


5. Now, let's dive into some training action with YOLOv5 on Google Colab.
6. Open this link and follow the steps https://colab.research.google.com/gist/lakshanthad/b47a1d1a9b4fac43449948524de7d374/yolov5-training-for-sensecap-a1101.ipynb
7. Drag and drop the model you just exported into the AI vision, and once the upload is successful, it will automatically disappear.
8. Upload the .uf2 file to the AI Grove. If done correctly, the Grove AI will disappear within a few seconds.
9. Upload the code to your arduino. Open the serial monitor and set baud rate as 115200 and the result of plants detection should be showed continuously. To see the camera result, go to this site: https://vision-ai-demo.seeed.cn/. 

Your serial monitor should look something like this: <br>
<img width="374" alt="Scherm­afbeelding 2023-10-19 om 03 43 13" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/ec406fed-2531-4528-a365-27d0114c4b93">

<img width="1792" alt="Scherm­afbeelding 2023-10-19 om 03 32 15" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/9ae1a38c-cb27-4566-9630-784ba9bdadb8">

***

$${\color{red}ERROR!}$$

If the GroveAI doesn't appear when connecting the USB-C cable initially, try pressing the boot button again twice. If that doesn't resolve the issue, reinsert the cable. The same steps apply if the camera doesn't show up in the browser.

If the camera doesn't display an image, attempt the following steps:
1. Check the hardware connections.
1. Press the boot button four times.
1. Reinsert the cable.
1. Re-upload the code.
1. As a last resort, consider restarting your computer.

<img width="800" alt="Scherm­afbeelding 2023-10-19 om 03 35 26" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/9e135d34-9487-44d5-821c-41bed05d51a2">


Something I've noticed is a lot of datasets don't have valid in there split. Ypu do need one, one easy way to fix this is to just simply duplicate the train or test folder en rename it. `cp -r /content/datasets/PlantDoc-1/test /content/datasets/PlantDoc-1/valid`

<img width="636" alt="Scherm­afbeelding 2023-10-19 om 02 44 27" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/35560b3a-9117-40dd-adc1-a4efb2cebdc1">

If your output in the serial monitor looks like this, you need to check two things:
1. Verify your Wi-Fi credentials and ensure you are connected to the correct network.
1. Change the baud rate to 115200.

***
### Capacitive Soil Moisture Sensor
Create a new sketch and paste the following code:

```
const int dry = 700; //You can change these values to your own liking
const int wet = 100; 

void setup()
{ 
  Serial.begin(115200);
}

void loop()
{
  int sensorVal = analogRead(A0);
  int percentageHumididy = map(sensorVal, wet, dry, 100, 0); 

  Serial.print(percentageHumididy);
  Serial.println("%");
  
  delay(100);
}
```

The serial monitor output should look something like this: <br>
<img width="164" alt="Scherm­afbeelding 2023-10-19 om 04 02 57" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/8ab54136-eb9a-4a5d-997e-4b6199044575">
***
$${\color{red}ERROR!}$$
<img width="636" alt="Scherm­afbeelding 2023-10-19 om 02 44 27" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/35560b3a-9117-40dd-adc1-a4efb2cebdc1">

If your output in the serial monitor looks like this change the baud rate to 115200.
***

### Combining the codes
For the last time, create a new sketch and paste the following code:

````
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

const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "REPLACE_WITH_YOUR_OPEN_WEATHER_MAP_API_KEY";
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
````

Serial monitor should look something like this <br>

<img width="512" alt="Scherm­afbeelding 2023-10-19 om 04 32 08" src="https://github.com/D0nnaz/DET-IoT/assets/92280247/fdade97a-3891-48f5-ba2a-38a37f735374">

***

### Result

https://github.com/D0nnaz/DET-IoT/assets/92280247/19ae5d1b-6746-44a1-bd7c-00293b74797e

***

### Sources:
* https://wiki.seeedstudio.com/Grove-Vision-AI-Module/
* https://randomnerdtutorials.com/esp8266-nodemcu-http-get-open-weather-map-thingspeak-arduino/
* https://docs.google.com/document/d/11RJf44mJMNgHd__wXg902liQfIFX2FspuLsXG_ZmpMQ/edit
* https://docs.google.com/document/d/1VLVhwIiPBkJHAkmd1BA77yfsYW7vlDimgMjPT_4U68I/edit


