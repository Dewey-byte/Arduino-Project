// Pin assignments
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 12;
const int buzzerPin = 8;

// Constants for ultrasonic sensor
long duration;
int distance;

// Threshold distance (in cm) to trigger buzzer and LED feedback
const int thresholdDistance = 30; // 30 cm (you can adjust this as needed)

// Variables for buzzer state management
bool buzzerState = false;  // Keeps track of the buzzer state

// Variables for debouncing
int lastDistance = -1;  // Last valid distance value
unsigned long lastTime = 0;  // Time of the last valid reading
const unsigned long debounceDelay = 100;  // Delay in ms between readings to debounce

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Check if enough time has passed to take another reading (debouncing)
  if (millis() - lastTime >= debounceDelay) {
    // Send a pulse to trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the duration of the pulse
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance in cm
    distance = duration * 0.0344 / 2;

    // Print distance for debugging
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Only update distance if the new reading is valid
    if (distance > 0 && distance < 400) {  // Valid distance range (usually 2cm to 400cm for HC-SR04)
      lastDistance = distance;
      lastTime = millis();  // Update last valid time
    }
  }

  // Check if an object is detected within the threshold distance
  if (lastDistance != -1 && lastDistance < thresholdDistance) {
    // Object detected: activate LED and buzzer
    digitalWrite(ledPin, HIGH);  // Turn on LED

    // Only turn on buzzer if it's not already on
    if (!buzzerState) {
      int buzzerTone = map(lastDistance, 0, thresholdDistance, 1000, 2000); // Map the distance to a tone range
      tone(buzzerPin, buzzerTone);  // Play a tone based on distance
      buzzerState = true;           // Set buzzerState to true, indicating the buzzer is on
    }
  } else {
    // No object detected: turn off LED and buzzer
    digitalWrite(ledPin, LOW);   // Turn off LED

    // Only turn off the buzzer if it's on
    if (buzzerState) {
      noTone(buzzerPin);          // Stop the buzzer
      buzzerState = false;        // Set buzzerState to false, indicating the buzzer is off
    }
  }

  // Delay before the next reading
  delay(50);  // Small delay for stability
}
