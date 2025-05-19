#include <LiquidCrystal.h>

#define FEET_PER_MILE 5280
#define TRACK_LEN 1320
#define RESET_BTN 47
const byte RS = 52;
const byte EN = 49;
const byte D4 = 53;
const byte D5 = 50;
const byte D6 = 51;
const byte D7 = 48;
const byte BAUD_RATE = 9600;
const byte LED_Prestage = 8;
const byte LED_Stage = 7;
const byte LED_Y1 = 6;
const byte LED_Y2 = 5;
const byte LED_Y3 = 4;
const byte LED_Start = 3;
const byte LED_RED_Light = 2;
const byte Pre_Stage_Sensor = A0;
const byte Stage_Sensor = A1;
const byte Finish_Sensor = A2;
const byte Start_Button = 46;

//define missing slash char for Flip ani..
uint8_t slash[8] = {
  0b10000,
  0b10000,
  0b01000,
  0b00100,
  0b00100,
  0b00010,
  0b00001,
  0b00001,
};

byte flip = 0;
unsigned long AniMilli = 0;
int IntervalAni = 50;

int state = 0;
bool StartFlag = false;
unsigned long countdownStart;
unsigned long raceStart;
unsigned long reactionTime;
bool FinishFlag;
unsigned long FinishET;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  
  pinMode(LED_Prestage, OUTPUT);
  pinMode(LED_Stage, OUTPUT);
  pinMode(LED_Y1, OUTPUT);
  pinMode(LED_Y2, OUTPUT);
  pinMode(LED_Y3, OUTPUT);
  pinMode(LED_Start, OUTPUT);
  pinMode(LED_RED_Light, OUTPUT);
  
  pinMode(Pre_Stage_Sensor, INPUT);
  pinMode(Stage_Sensor, INPUT);
  pinMode(Finish_Sensor, INPUT);
  pinMode(Start_Button, INPUT_PULLUP);
  pinMode(RESET_BTN, INPUT_PULLUP);
  //Serial.begin(BAUD_RATE);
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.createChar(1, slash);
  lcd.setCursor(0, 1);
  lcd.print(F("System Ready"));
  delay(2000);
  lcd.clear();
}

void loop()
{
  int Pre_Stage_Sensor_Value = analogRead(Pre_Stage_Sensor);
  int Stage_Sensor_Value = analogRead(Stage_Sensor);
  int Finish_Sensor_Value = analogRead(Finish_Sensor);
  /*
    Serial.print("PreStage: ");
    Serial.println(Pre_Stage_Sensor_Value);
    Serial.print("Stage: ");
    Serial.println(Stage_Sensor_Value);
    Serial.print("Finish: ");
    Serial.println(Finish_Sensor_Value);
    Serial.print("State: ");
    Serial.println(state);
    Serial.println();
    delay(2000);
  */
  switch (state) {
    case 0:
      if (Pre_Stage_Sensor_Value > 500) {
        digitalWrite(LED_Prestage, LOW);
      }
      else {
        digitalWrite(LED_Prestage, HIGH);
        state++;
      }
      break;

    case 1:// Vehicle Staging State
      if (Stage_Sensor_Value > 500) {
        digitalWrite(LED_Stage, LOW);
      }
      else {
        digitalWrite(LED_Stage, HIGH);

      }

      if (Stage_Sensor_Value < 500) {
        lcd.clear();
        lcd.print("Vehicle Ready");
        delay(500);
        state++;
      }
      else {
        lcd.clear();
        lcd.print("Please Stage");
        delay(500);
        state--;
      }
      
      break;

    case 2:
      if (Stage_Sensor_Value > 501) {
        state--;
      }
      else
      {

        if (digitalRead(Start_Button) == LOW)
        {
          countdownStart = millis();
          state++;
        }
      }
      break;

    case 3:

      if (millis() - countdownStart > 930)
      {  //check sensor just before dropping the flag
        if (Stage_Sensor_Value < 500){        
        digitalWrite(LED_Y3, LOW);
        digitalWrite(LED_Start, HIGH);
        StartFlag = true;
        state++;} else{
             digitalWrite(LED_Y3, LOW);
             digitalWrite(LED_Prestage, LOW);
             digitalWrite(LED_Stage, LOW);
             digitalWrite(LED_RED_Light, HIGH);
             delay(100);
             lcd.clear();
             lcd.print("!!Bad Start!!");
             state= 7;
        }


      }
      else if (millis() - countdownStart > 850)
      {
        digitalWrite(LED_Y2, LOW);
        digitalWrite(LED_Y3, HIGH);
      }
      else if (millis() - countdownStart > 650)
      {
        digitalWrite(LED_Y1, LOW);
        digitalWrite(LED_Y2, HIGH);
      }
      else if (millis() - countdownStart > 550)
      {
        digitalWrite(LED_Y1, HIGH);
      }
      break;

    case 4: //check for false start??
      if (analogRead(Stage_Sensor) > 500 && !StartFlag)
      {
        digitalWrite(LED_RED_Light, HIGH);
        lcd.clear();
        lcd.print("!!Bad Start!!");
        state = 7;
      } else state++;

      break;
   case 5:
    if ((millis() - countdownStart > 2000) && analogRead(Stage_Sensor_Value > 500)) {
        lcd.clear();
        lcd.print("       GO        ");
        raceStart = millis(); // Record the race start time
        state++;
    }
    break;

case 6:
    if (analogRead(Finish_Sensor) < 500) {
        if (!FinishFlag) {
            FinishFlag = true;
            FinishET = millis() - raceStart;
            lcd.clear();
            lcd.print("ET:");
            float secs = float(FinishET) / 1000;
            lcd.print(secs, 2); // Display elapsed time to 2 decimal places
            lcd.setCursor(0, 1);
            lcd.print("KPH:");

            int t = 80;
            digitalWrite(LED_Y1, HIGH);
            delay(t);
            digitalWrite(LED_Y1, LOW);
            digitalWrite(LED_Y2, HIGH);
            delay(t);
            digitalWrite(LED_Y2, LOW);
            digitalWrite(LED_Y3, HIGH);

            float fps = (TRACK_LEN / secs);
            Serial.print("fps:"); Serial.println(fps, 4);
            float mph = (fps / FEET_PER_MILE) / 0.00028;
            float kph = mph * 1.60934; // Convert MPH to KPH
            Serial.print("KPH:"); Serial.println(kph, 4);
            lcd.print(kph, 2); // Display KPH to 2 decimal places
            state = 7;
        }
    } else {
        // Update the running timer during the race
        unsigned long currentMillis = millis();
        float runningTime = float(currentMillis - raceStart) / 1000; // Calculate elapsed time
        lcd.setCursor(0, 0);
        lcd.print("Time:");
        lcd.print(runningTime, 2); // Display elapsed time with 2 decimal places
        lcd.setCursor(0, 1);
        lcd.print("Racing...");
    }
    break;

  case 7:
      if (digitalRead(RESET_BTN) == LOW)
      { 
        digitalWrite(LED_Start, LOW);   
        digitalWrite(LED_Stage, LOW); 
        digitalWrite(LED_RED_Light, LOW); 
        digitalWrite(LED_Y1, LOW);
        digitalWrite(LED_Y2, LOW);
        digitalWrite(LED_Y3, LOW);      
        StartFlag = false;
        FinishFlag = false;
        state = 0;
      }
      break;

  }
}// END LOOP BRACKETS

void FlipAni() {
  if (millis() - AniMilli >= IntervalAni)
  {
    AniMilli = millis();
    switch (flip) {
      case 0: flip = 1; lcd.setCursor(0, 0); lcd.print("|"); lcd.setCursor(15, 0); lcd.print("|"); break;
      case 1: flip = 2; lcd.setCursor(0, 0); lcd.print("/"); lcd.setCursor(15, 0); lcd.print("/"); break;
      case 2: flip = 3; lcd.setCursor(0, 0); lcd.print("-"); lcd.setCursor(15, 0); lcd.print("-"); break;
      case 3: flip = 4; lcd.setCursor(0, 0); lcd.print("\x01"); lcd.setCursor(15, 0); lcd.print("\x01"); break;
      case 4: flip = 0; lcd.setCursor(0, 0); lcd.print("|"); lcd.setCursor(15, 0); lcd.print("|"); break;
    }
  }
}
