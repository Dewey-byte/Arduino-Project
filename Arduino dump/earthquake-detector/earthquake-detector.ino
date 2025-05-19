#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define I2C LCD address and size
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the pin for the vibration sensor
int vibrationPin = 2;  // Ang digital input pin nga nakonektar sa vibration sensor

// Define the pin for the buzzer
int buzzerPin = 8;  // Ang pin nga nakonektar sa buzzer

// Define the pin for the LED
int ledPin = 7;  // Ang pin nga nakonektar sa LED

// Variables para sa debouncing
unsigned long lastVibrationTime = 0;  // Mag-store sa last time nga nadetect ang vibration
unsigned long debounceDelay = 100;     // Ang delay sa debounce (sa millisecond)

void setup() {
  // Sugdi ang serial communication
  Serial.begin(9600);
  
  // I-set ang vibration sensor pin as input
  pinMode(vibrationPin, INPUT);
  
  // I-set ang buzzer ug LED pins as output
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // I-initialize ang LCD
  lcd.init();
  lcd.backlight();
  
  // I-display ang "Earthquake" sa unang row
  lcd.setCursor(0, 0);  // I-position ang cursor sa unang row
  lcd.print("Earthquake");
  
  // I-display ang "Detector" sa ikaduhang row
  lcd.setCursor(4, 1);  // I-position ang cursor sa ikaduhang row
  lcd.print("Detector ^.^");

  delay(2000);  // Maghulat og 2 seconds para makita ang mensahe
  
  lcd.clear();  // I-clear ang screen pagkahuman sa animation
}

void loop() {
  // Basaha ang state sa vibration sensor
  int vibrationState = digitalRead(vibrationPin);
  
  // Kuhaa ang current time para sa debouncing
  unsigned long currentMillis = millis();
  
  // Mag-respond lang sa vibration kung nakaabot na sa delay time gikan sa last detection
  if (vibrationState == HIGH && (currentMillis - lastVibrationTime) > debounceDelay) {
    // I-update ang last vibration time
    lastVibrationTime = currentMillis;
    
    // I-print ang state sa serial monitor (optional)
    Serial.print("Vibration State: ");
    Serial.println(vibrationState);
    
    // I-clear ang LCD screen
    lcd.clear();
    
    // I-display ang sensor state sa LCD
    lcd.setCursor(0, 0);
    lcd.print("Vibration Detected:");
    lcd.setCursor(0, 1);
    lcd.print("Earthquake!^_^");
    
    // I-turn on ang buzzer kung makakita og vibration
    digitalWrite(buzzerPin, HIGH);  // I-turn on ang buzzer
    tone(buzzerPin, 1000);          // I-start ang buzzer with 1kHz tone
    delay(1000);                    // Mag-sound ang buzzer for 1 second
    noTone(buzzerPin);              // I-stop ang buzzer
    
    // I-turn on ang LED kung makakita og vibration
    digitalWrite(ledPin, HIGH);     // I-turn on ang LED
  } else {
    // Kung walay vibration nga nadetect, i-turn off ang LED ug i-display ang "Observing..." sa LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Vibration Detected:");
    lcd.setCursor(0, 1);
    lcd.print("Observing...^.^");
    
    // I-turn off ang LED kung walay vibration nga nadetect
    digitalWrite(ledPin, LOW);      // I-turn off ang LED
  }

  // Gamay nga delay para dili magdagan kaayo paspas ang loop
  delay(100);  // Pwede nimo i-adjust kini para sa mas maayo nga sensitivity
}
