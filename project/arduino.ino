/* 
Date: 11-7-26
[Incomplete Commit] Working through the tutorial but playing around to see what I can move around and not do exactly the same. 

Parts:
Arduino UNO R3
ADXL345

Used tutorial linked:
https://controllerstech.com/adxl345-arduino-i2c-tutorial/
*/

#include <Wire.h>

#define ADXL345_ADDR 0x53
float scaleFactor = 256.0; // For ±2g full-resolution mode
float xOffset = 0.0, yOffset = 0.0, zOffset = 0.0;

int16_t xRaw = 0, yRaw = 0, zRaw = 0;
int samples = 200;

long xSum = 0, ySum = 0, zSum = 0;
long xNew = 0, yNew = 0, zNew = 0; 

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

  delay(200);
  Serial.println("Calibrating...");

    for (int i = 0; i <samples; i++) {
    
      // Select starting register
    Wire.beginTransmission(ADXL345_ADDR);
    Wire.write(0x32);  
    Wire.endTransmission(false);

      // Read 6 bytes for X, Y, Z
    Wire.requestFrom(ADXL345_ADDR, 6, true);

    xRaw = (Wire.read() | (Wire.read() << 8));
    yRaw = (Wire.read() | (Wire.read() << 8));
    zRaw = (Wire.read() | (Wire.read() << 8));

    xSum += xRaw;
    ySum += yRaw;
    zSum += zRaw;

    delay(5);
  }

  Serial.println("Calibration Complete:");
  Serial.print("X Offset: "); Serial.println(xOffset);
  Serial.print("Y Offset: "); Serial.println(yOffset);
  Serial.print("Z Offset: "); Serial.println(zOffset);

  Serial.println("ADXL345 Initialized...");
}

void loop() {
  

  // Establish offset based on averages that will be re-defined during the loop function
  float xOffset = xSum / samples;
  float yOffset = ySum / samples;
  float zOffset = (zSum / samples) - scaleFactor;

  // Subtract offsets from Future Readings
  float xNew = xRaw - xOffset;
  float yNew = yRaw - yOffset;
  float zNew = zRaw - zOffset;
  // Convert to g
  float xg = xNew / scaleFactor;
  float yg = yNew / scaleFactor;
  float zg = zNew / scaleFactor;

  Serial.print("X (g): "); Serial.print(xg);
  Serial.print("  | Y (g): "); Serial.print(yg);
  Serial.print("  | Z (g): "); Serial.println(zg);

  delay(300);
}
