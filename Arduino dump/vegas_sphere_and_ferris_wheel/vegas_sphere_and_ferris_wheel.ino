/*
   -- Ferris Wheel Control with ESP32 & RemoteXY (BLE) --
   
   This code allows you to control a stepper motor using an ESP32 via Bluetooth (BLE) 
   using the RemoteXY app. The speed of the stepper motor is controlled with a slider.  

   Libraries required:
   - RemoteXY (https://remotexy.com/en/library/)
   - AccelStepper (Install via Arduino Library Manager)
*/

//////////////////////////////////////////////
//        Include Required Libraries        //
//////////////////////////////////////////////
#define REMOTEXY_MODE__ESP32CORE_BLE
#include <BLEDevice.h>
#include <RemoteXY.h>
#include <AccelStepper.h>

//////////////////////////////////////////////
//         RemoteXY BLE Configuration       //
//////////////////////////////////////////////

#define REMOTEXY_BLUETOOTH_NAME "Vegas"
#define REMOTEXY_ACCESS_PASSWORD "1234"

#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = { 
  255,2,0,0,0,87,0,19,0,0,0,67,100,32,71,119,97,112,111,0,
  32,1,106,200,1,1,5,0,130,10,9,88,175,12,45,2,28,41,49,20,
  1,32,43,16,16,79,78,0,79,70,70,0,4,46,102,17,69,0,32,26,
  129,30,90,50,9,64,25,70,101,114,114,105,115,32,87,104,101,101,108,0,
  129,37,21,33,12,64,25,76,105,103,104,115,0 
};
#pragma pack(pop)

// Define RemoteXY Variables
struct {
  uint8_t switch_01; // ON/OFF Switch
  int8_t slider_01;  // Speed Control (0-100)
  uint8_t connect_flag; // BLE Connection Status
} RemoteXY;

//////////////////////////////////////////////
//           Stepper Motor Setup            //
//////////////////////////////////////////////

#define LED_PIN 2 // Define LED pin (change according to your ESP32 board)


// Define stepper motor pins (ULN2003)
#define IN1 5
#define IN2 18
#define IN3 19
#define IN4 21

// Create stepper motor object (Half-step mode for smoother movement)
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

//////////////////////////////////////////////
//                Setup                     //
//////////////////////////////////////////////

void setup() {
  RemoteXY_Init();  // Initialize RemoteXY
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  stepper.setMaxSpeed(1200);  // Max speed (adjustable)
  stepper.setAcceleration(500); // Smooth acceleration
}

//////////////////////////////////////////////
//                 Loop                     //
//////////////////////////////////////////////


void loop() { 
  RemoteXY_Handler();  // Handle RemoteXY data

   if (RemoteXY.switch_01) {
    digitalWrite(LED_PIN, LOW); // Turn LED OFF
  } else {
    digitalWrite(LED_PIN, HIGH); // Turn LED ON
  }

  // Map slider (0-100) to motor speed ( 0 to 1200)
  int motorSpeed = map(RemoteXY.slider_01, 0, 100, 0, 1200);
  
  // Apply speed to stepper motor
  stepper.setSpeed(motorSpeed);
  stepper.runSpeed();
}
