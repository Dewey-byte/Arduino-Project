// Include required libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the I2C LCD with the address (commonly 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the flame sensor and buzzer pins
const int flameSensorPin = A0;
const int buzzerPin = 9; // Connect the buzzer to digital pin 9

// Threshold for flame detection (adjust as needed)
const int flameThreshold = 500;

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Set up the flame sensor and buzzer pins
  pinMode(flameSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Display a startup message
  lcd.setCursor(0, 0);
  lcd.print("Flame Detector   "); // Pad with spaces to clear line
  lcd.setCursor(0, 1);
  lcd.print("Emman & Jhian    "); // Pad with spaces to clear line
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read the value from the flame sensor
  int sensorValue = analogRead(flameSensorPin);

  // Display the sensor value on the serial monitor
  Serial.print("Flame Sensor Value: ");
  Serial.println(sensorValue);

  // Check if the sensor value is below the threshold
  if (sensorValue < flameThreshold) {
    // Flame detected
    lcd.setCursor(0, 0);
    lcd.print("Flame Detected!  "); // Pad with spaces to clear line
    lcd.setCursor(0, 1);
    lcd.print("Warning: Fire!   "); // Pad with spaces to clear line
    digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
  } else {
    // No flame detected
    lcd.setCursor(0, 0);
    lcd.print("No Flame         "); // Pad with spaces to clear line
    lcd.setCursor(0, 1);
    lcd.print("All Clear ^_^    "); // Pad with spaces to clear line
    digitalWrite(buzzerPin, LOW); // Turn the buzzer off
  }

  // Wait for a short period before the next reading
  delay(500);
}
