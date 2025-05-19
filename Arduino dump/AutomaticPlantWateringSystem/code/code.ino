#include <Wire.h>

// Pag-define sa mga pins
int moisturePin = A0;     // Moisture sensor nakasaksak sa A0
int relayPin = 7;         // Relay signal nakasaksak sa digital pin 7
int moistureValue = 0;    // Variable para itipig ang moisture sensor value
int threshold = 500;      // Moisture level threshold (i-adjust kung kinahanglan)

void setup() {
  // I-setup ang relay pin isip output
  pinMode(relayPin, OUTPUT);

  // Siguraduhon nga ang relay naka-off sa pagsugod
  digitalWrite(relayPin, HIGH); // Relay kay LOW-active, so HIGH means OFF

  // Sugdi ang serial communication para sa debugging (optional)
  Serial.begin(9600);

  // Initial message sa Serial Monitor
  Serial.println("Plant Monitor Initialized");
}

void loop() {
  // Basaha ang moisture sensor value (analog)
  moistureValue = analogRead(moisturePin);

  // I-check kung uga o basa ang yuta
  if (moistureValue > threshold) {
    // Uga ang yuta, paandara ang water pump
    digitalWrite(relayPin, LOW);  // Paandara ang pump (relay kay LOW-active)
    Serial.println("Watering Plant");
  } else {
    // Basa ang yuta, patya ang pump
    digitalWrite(relayPin, HIGH); // Patya ang pump
    Serial.println("Soil is Moist");
  }

  // Optional: i-print ang sensor values sa Serial Monitor para sa debugging
  Serial.print("Moisture: ");
  Serial.println(moistureValue);

  // Gamay nga delay before magkuha balik ug reading
  delay(1000);
}
