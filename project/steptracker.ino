/* 
[steptrackerv2]
Date: 11-26-26
Happy Thanksgiving! I was able to get readings using the Seeed Studio Xiao ESP32C3 board. Next step is to turn this acceleration data into actual "steps taken" data.  

Parts:
Arduino UNO R3 -> Seeed Studio XIAO ESP32C3
ADXL345
ELEGOO 0.96 OLED Screen
Buttons

Version Overview:
------ Version 1 [x] ------
Getting used to the Arduino UNO R3 and structure of code based on examples and tutorials (attached in References)

------ Version 2 [WIP] ------ 
Prototyping the step tracker on the breadboard to gather data and display the step data on the OLED screen. 

------ Version 3 [ ] ------
Making it mobile. Getting the step tracker on a soldered breadboard with the 3.7V LiPo battery. Getting BLE setup to a mobile app.

------ Version 4 [ ] ------
Finalize physical design. Streamline the code and make app run efficiently.

References:
https://controllerstech.com/adxl345-arduino-i2c-tutorial/
https://mjrovai.github.io/XIAO_Big_Power_Small_Board-ebook/chapter_1-1.html#task-1-run-blink-to-make-xiaos-led-flash
https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/#pin-map

*/

#include <iostream>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==========================================
// PIN DEFINITIONS (SuperMini Safe Pins)
// ==========================================
#define I2C_SDA          6
#define I2C_SCL          7

#define OLED_SCREEN_WIDTH  128
#define OLED_SCREEN_HEIGHT 64
#define OLED_RESET         -1
#define OLED_I2C_ADDR      0x3C

// ==========================================
// BLE UUIDs (Standard 16-bit Service/Char)
// ==========================================
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26aae"

// ==========================================
// DRIVER OBJECT INITIALIZATION
// ==========================================
Adafruit_SSD1306 display(OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/*BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
unsigned long lastBleSend = 0;

/ Connection callback handlers
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnected(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Mobile App Connected via BLE!");
    }

    void onDisconnected(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Mobile App Disconnected. Restarting advertising...");
      pServer->getAdvertising()->start(); 
    }
};*/

void setup() {
  Serial.begin(115200);
  delay(2000);

  // 1. Initialize I2C Bus on Pins 8 & 9
  Wire.begin(I2C_SDA, I2C_SCL);
  delay(100);

  // 2. Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
    Serial.println("SSD1306 allocation failed!");
  } else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Smart Watch BLE Init");
    display.display();
    delay(1000);
  }

  // 3. Initialize ADXL345 Accelerometer
  if (!accel.begin(0x53)) {
    Serial.println("No ADXL345 detected!");
  } else {
    accel.setRange(ADXL345_RANGE_2_G);
    Serial.println("ADXL345 initialized successfully.");
  }

  /*
  // 4. Initialize BLE Server & Service
  BLEDevice::init("ESP32C3_StepTracker");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  / Start Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x06);  // Helps with iPhone connection stability
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE Device active and advertising...");
  */
}

void loop() {
  // Read Accelerometer Event
  sensors_event_t event;
  accel.getEvent(&event);

  float xAcc = event.acceleration.x;
  float yAcc = event.acceleration.y;
  float zAcc = event.acceleration.z;

  /*/ Format data string for mobile app (e.g., "X:1.96,Y:2.04,Z:2.12")
  char dataPayload[64];
  snprintf(dataPayload, sizeof(dataPayload), "X:%.2f,Y:%.2f,Z:%.2f", xAcc, yAcc, zAcc);

  // Send via BLE Notification if a phone is connected (every 100ms)
  if (deviceConnected && (millis() - lastBleSend >= 100)) {
    lastBleSend = millis();
    pCharacteristic->setValue(dataPayload);
    pCharacteristic->notify();
  }*/

  // Update Local OLED Display UI
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("== ESP32-C3 WATCH =="));

  display.setCursor(0, 16);
  display.print(F("X:")); display.print(xAcc, 1);
  display.print(F(" Y:")); display.print(yAcc, 1);
  display.print(F(" Z:")); display.println(zAcc, 1);

  //Serial.println("No ADXL345 detected!");
  Serial.print(F("X:")); Serial.print(xAcc, 1);
  Serial.print(F(" Y:")); Serial.print(yAcc, 1);
  Serial.print(F(" Z:")); Serial.println(zAcc, 1);


  display.setCursor(0, 36);
  //display.print(F("BLE: "));
  //display.print(deviceConnected ? F("Connected") : F("Advertising"));

  display.setCursor(0, 52);
  display.println(F("Status: Active"));

  display.display();

  delay(100);
}
