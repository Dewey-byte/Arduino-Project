#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
const int wirePin = A0;
const int finishPin = A2;
const int buzzerPin = 4;
const int redPin = 5;
const int greenPin = 6;

// Sensitivity settings
const int threshold = 120;  // Increase this if false triggers continue
const int sampleSize = 10;  // More samples = more stable reading
const int debounceDelay = 50;  // Delay in milliseconds to confirm a valid touch

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Wire Buzz Game");
  delay(2000);
  lcd.clear();
  lcd.print("Start Playing!");
}

void playTone(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration);
  noTone(buzzerPin);
}

int getStableReading(int pin) {
  long total = 0;
  for (int i = 0; i < sampleSize; i++) {
    total += analogRead(pin);
    delay(5);  // Small delay to remove rapid noise spikes
  }
  return total / sampleSize;  // Return averaged value
}

bool isSteadyContact(int pin) {
  int firstReading = getStableReading(pin);
  delay(debounceDelay);
  int secondReading = getStableReading(pin);
  
  return (firstReading < threshold && secondReading < threshold);
}

void loop() {
  if (isSteadyContact(wirePin)) {  
    playTone(500, 500);
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    delay(5000);
    digitalWrite(redPin, LOW);
    lcd.clear();
    lcd.print("Start Playing!");
  }

  if (isSteadyContact(finishPin)) {  
    playTone(1000, 500);
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Winner!");
    delay(5000);
    digitalWrite(greenPin, LOW);
    lcd.clear();
    lcd.print("Start Playing!");
  }
}
