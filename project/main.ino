/* 
Date Finished: 11-4-26
This code gets the raw sensor data from the ADXL345 and multiplies it by the scale factor
to obtain values in terms of g's.

Parts:
Arduino UNO R3
ADXL345

Uses tutorial linked:
https://controllerstech.com/adxl345-arduino-i2c-tutorial/
*/

#include <Wire.h>

#define ADXL345_ADDR 0x53
float scaleFactor = 256.0; // For ±2g full-resolution mode


void setup() {
  Serial.begin(9600);
  Wire.begin();

  // --- Put ADXL345 into Measurement Mode ---
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x2D);     // POWER_CTL register
  Wire.write(0x08);     // Measurement mode ON
  Wire.endTransmission();

  // --- Set Full Resolution Mode (10-bit to 13-bit depending on g-range) ---
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x31);     // DATA_FORMAT register
  Wire.write(0x08);     // Full Resolution + ±2g range
  Wire.endTransmission();

  // --- Set Data Rate to 100 Hz ---
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x2C);     // BW_RATE register
  Wire.write(0x0A);     // 100 Hz output data rate
  Wire.endTransmission();

  Serial.println("ADXL345 Initialized...");
}

void loop() {
  int16_t xRaw, yRaw, zRaw;

  // Select starting register
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x32);  
  Wire.endTransmission(false);

  // Read 6 bytes for X, Y, Z
  Wire.requestFrom(ADXL345_ADDR, 6, true);

  xRaw = (Wire.read() | (Wire.read() << 8));
  yRaw = (Wire.read() | (Wire.read() << 8));
  zRaw = (Wire.read() | (Wire.read() << 8));

  Serial.print("X Raw: "); Serial.print(xRaw);
  Serial.print("  | Y Raw: "); Serial.print(yRaw);
  Serial.print("  | Z Raw: "); Serial.println(zRaw);

  // Convert to g
  float xg = xRaw / scaleFactor;
  float yg = yRaw / scaleFactor;
  float zg = zRaw / scaleFactor;

  Serial.print("X (g): "); Serial.print(xg);
  Serial.print("  | Y (g): "); Serial.print(yg);
  Serial.print("  | Z (g): "); Serial.println(zg);


  delay(300);
}
