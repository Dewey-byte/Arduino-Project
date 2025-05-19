#include <Servo.h>

const int trigPin = 11;
const int echoPin = 12;
const int servoPin = 6;

// Define short-range detection threshold
const int distanceThreshold = 2;  // Detect objects within 5 cm

Servo waterServo;
const int closedAngle = 0;   
const int openAngle = 180;  

unsigned long lastTriggerTime = 0; // To track when the servo last moved
const int servoHoldTime = 1000;    // Servo stays open for 2 seconds

void setup() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    waterServo.attach(servoPin);
    waterServo.write(closedAngle); // Start in closed position
    Serial.begin(9600);
}

void loop() {
    int distance = getDistance(); // Get ultrasonic sensor reading

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 0 && distance <= distanceThreshold) {
        if (waterServo.read() != openAngle) {  // Open only if it's closed
            waterServo.write(openAngle);
            Serial.println("Dispensing water...");
            lastTriggerTime = millis();  // Record when it was triggered
        }
    } else {
        if (millis() - lastTriggerTime > servoHoldTime) {
            if (waterServo.read() != closedAngle) {  // Close only if it's open
                waterServo.write(closedAngle);
                Serial.println("Waiting for object...");
            }
        }
    }
}

// Function to measure distance (non-blocking)
int getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2; // Convert to cm
    return distance;
}
