#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD (address 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
const int wirePin = A0;      // Pin connected to the wire sensor
const int finishPin = 8;    // Pin connected to the finish line sensor
const int buzzerPin = 4;     // Pin connected to the buzzer
const int redPin = 5;        // Pin connected to red LED
const int greenPin = 6;      // Pin connected to green LED

// Debounce settings
const int debounceDelay = 5; 
unsigned long lastWireTime = 0;
unsigned long lastFinishTime = 0;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(wirePin, INPUT_PULLUP);
  pinMode(finishPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Wire Buzz Game");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Start Playing!");
}

void playTone(int pin, int frequency, int duration) {
  tone(pin, frequency, duration);
  delay(duration);
  noTone(pin);
}

// Debounce Function - waits for a stable state
bool debounce(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(5); // Wait for 50ms to confirm it's stable
    if (digitalRead(pin) == LOW) return true; // If still LOW, confirm trigger
  }
  return false;
}

void loop() {
  if (debounce(wirePin)) {
    playTone(buzzerPin, 500, 500);
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    delay(5000);
    digitalWrite(redPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start Playing!");
  }

  if (debounce(finishPin)) {
    playTone(buzzerPin, 1000, 500);
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Winner!");
    delay(5000);
    digitalWrite(greenPin, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Start Playing!");
  }
  
  digitalWrite(buzzerPin, LOW);
}
