// Pin assignments
const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 2;
const int buzzerPin = 8;
const int vibratorPin = 7;  // I-define ang pin para sa vibrator motor

// Constants para sa ultrasonic sensor
long duration;
int distance;

// Threshold distance (sa cm) aron mag-trigger ang buzzer ug LED feedback
const int thresholdDistance = 30; // 30 cm (pwede nimo i-adjust kung kinahanglan)

// Variables para sa buzzer ug vibrator motor state management
bool buzzerState = false;  // Nagbantay kung naka-on ang buzzer
bool vibratorState = false;  // Nagbantay kung naka-on ang vibrator motor

// Variables para sa debouncing
int lastDistance = -1;  // Last valid nga distansya
unsigned long lastTime = 0;  // Oras sa last valid nga pagbasa
const unsigned long debounceDelay = 100;  // Delay sa ms taliwala sa mga pagbasa para sa debouncing

void setup() {
  // Pag-initialize sa serial communication
  Serial.begin(9600);
  
  // Pag-setup sa mga pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(vibratorPin, OUTPUT);  // I-set ang vibrator motor pin as output
}

void loop() {
  // I-check kung igo na ba ang oras para magkuha og bag-ong reading (debouncing)
  if (millis() - lastTime >= debounceDelay) {
    // I-send ang pulse aron mag-trigger sa ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Sukda ang duration sa pulse
    duration = pulseIn(echoPin, HIGH);

    // I-calculate ang distansya sa cm
    distance = duration * 0.0344 / 2;

    // I-print ang distansya para sa debugging
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // I-update ang distansya kung valid ang bag-ong reading
    if (distance > 0 && distance < 400) {  // Valid nga range sa distansya (kasagaran 2cm to 400cm para sa HC-SR04)
      lastDistance = distance;
      lastTime = millis();  // I-update ang last valid nga oras
    }
  }

  // I-check kung ang object kay naa sulod sa threshold distance
  if (lastDistance != -1 && lastDistance < thresholdDistance) {
    // Object detected: i-activate ang LED, buzzer, ug vibrator motor
    digitalWrite(ledPin, HIGH);  // I-on ang LED

    // I-on ang buzzer kung dili pa siya naka-on
    if (!buzzerState) {
      int buzzerTone = map(lastDistance, 0, thresholdDistance, 1000, 2000); // I-map ang distansya ngadto sa tone range
      tone(buzzerPin, buzzerTone);  // I-play ang tone base sa distansya
      buzzerState = true;           // I-set ang buzzerState ngadto sa true, nagpasabot nga naka-on ang buzzer
    }

    // I-on ang vibrator motor kung dili pa siya naka-on
    if (!vibratorState) {
      digitalWrite(vibratorPin, HIGH);  // I-on ang vibrator motor
      vibratorState = true;             // I-set ang vibratorState ngadto sa true, nagpasabot nga naka-on ang motor
    }
  } else {
    // Wala'y object nga nadetect: i-off ang LED, buzzer, ug vibrator motor
    digitalWrite(ledPin, LOW);   // I-off ang LED

    // I-off ang buzzer kung naka-on pa siya
    if (buzzerState) {
      noTone(buzzerPin);          // I-stop ang buzzer
      buzzerState = false;        // I-set ang buzzerState ngadto sa false, nagpasabot nga naka-off ang buzzer
    }

    // I-off ang vibrator motor kung naka-on pa siya
    if (vibratorState) {
      digitalWrite(vibratorPin, LOW);  // I-off ang vibrator motor
      vibratorState = false;           // I-set ang vibratorState ngadto sa false, nagpasabot nga naka-off ang motor
    }
  }

  // Gamay nga delay para sa stability sa mga readings
  delay(50);  // Gamay nga delay
}
