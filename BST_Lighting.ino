/*  Bridger Solar Team at Montana State University
    Authors: Levi Allery and Tanner Whetzel
    System Integration Capstone: Lighting, Wiring, and Schematics
    Last Updated: April 3, 2091
                  April 1, 2019
                  March 31, 2019
                  March 29, 2019
                  March 27, 2019
                  February 20, 2019
                  February 19, 2019
                  February 18, 2019
                  February 17, 2019
                  February 15, 2019
*/

// NOTE: Running light functions have been commented out for the time being. 
// Bit-bang the non PWM pins (PWM are D3, D5, D9, D10, D11) for dimming the running light signal
/*
   do {
      digitalWrite(xxx, LOW);
      delayMicroseconds(100); // About 10% duty cycle at 1kHz
      digitalWrite(xxx, HIGH);
      delayMicroseconds(1000-100);
   } while (runlight == HIGH);
*/
// Let other signals override the running lights

//#include <Wire.h>
/*
// Communication with Pi
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;
const int i2cSDA = A4;
const int i2cSDL = A5;
*/
// Output to lights
const int runlightPinOUT = 7; // Left active to prevent Pin floating
const int bmsFaultPinOUT = 6;
const int brakePinOUT = 5; // PWM Pin
const int leftTurnPinOUT = 4;
const int rightTurnPinOUT =  3; // PWM Pin
const int headlightPinOUT = 2;

// Input from switches
const int bmsFaultPinIN = A2;
const int hazardPinIN = A1;
const int brakePinIN = 12;
//const int runlightPinIN = 10;
const int headlightPinIN = 11;
const int leftTurnPinIN = 9;
const int rightTurnPinIN =  8;

// Digital and Analog read state variables
int bms;
int hazard;
int left;
int right;
int headlight;
int brake;
//int runlight;

// Storing time for flashing lights
unsigned long oldBMSMillis = 0;
unsigned long oldHazardMillis = 0;
unsigned long oldLtMillis = 0;
unsigned long oldRtMillis = 0;

const long bmsFlash = 500; //Sets BMS fault flash interval
const long interval = 600; //Sets blinker and hazard interval

// Setup pinouts
void setup() {
  //pinMode(i2cSDA, OUTPUT);
  Serial.begin(9600); // Start Serial for output
  //Wire.begin(SLAVE_ADDRESS);
/*
  // Define callbacks for I2C communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready!");
*/
  // Initialize output pins
  pinMode(bmsFaultPinOUT, OUTPUT);
  pinMode(headlightPinOUT, OUTPUT);
  pinMode(brakePinOUT, OUTPUT);
  pinMode(leftTurnPinOUT, OUTPUT);
  pinMode(rightTurnPinOUT, OUTPUT);
  pinMode(runlightPinOUT, OUTPUT);

  // Initialize input pins
  pinMode(bmsFaultPinIN, INPUT);
  pinMode(hazardPinIN, INPUT);
  pinMode(headlightPinIN, INPUT);
  pinMode(brakePinIN, INPUT);
  pinMode(leftTurnPinIN, INPUT);
  pinMode(rightTurnPinIN, INPUT);
  //pinMode(runlightPinIN, INPUT);
}

// Master loop
void loop() {
  Serial.write("\nHello\n");
  
  bms = analogRead(bmsFaultPinIN);
  hazard = analogRead(hazardPinIN);
  left = digitalRead(leftTurnPinIN);
  right = digitalRead(rightTurnPinIN);
  brake = digitalRead(brakePinIN);
  headlight = digitalRead(headlightPinIN);
  //runlight = digitalRead(runlightPinIN);

  unsigned long newBMSMillis = millis();
  unsigned long newHazardMillis = millis();
  unsigned long newLtMillis = millis();
  unsigned long newRtMillis = millis();

  brakeLightFXN();
  headlightFXN();

  digitalWrite(runlightPinOUT, LOW);

  //Checking state of BMS Fault and time since state change against flashing interval
  if (bms <= 512) { //bms corresponds to a 10-bit analog read for the Nano, so if bms is greater than 50%...
    if (newBMSMillis - oldBMSMillis >= bmsFlash) {
      oldBMSMillis = newBMSMillis;

      bmsFaultFXN(); //Execute BMS Fault code
    }
  }

  //Checking state of hazards and time since state change against flashing interval
  if (hazard <= 512) { //hazard corresponds to a 10-bit analog read for the Nano, giving a resolution from 0-1023
    if (newHazardMillis - oldHazardMillis >= interval) {
      oldHazardMillis = newHazardMillis;

      hazardFXN(); //Execute Hazard code
    }
  }

  //Checking state of left turn signal and time since state change against flashing interval
  if (left == HIGH) {
    if (newLtMillis - oldLtMillis >= interval) {
      oldLtMillis = newLtMillis;

      leftTurnFXN(); //Execute Left Turn Signal code
    }
  }

  //Checking state of right turn signal and time since state change against flashing interval
  if (right == HIGH) {
    if (newRtMillis - oldRtMillis >= interval) {
      oldRtMillis = newRtMillis;

      rightTurnFXN(); //Execute Right Turn Signal code
    }
  } 
}
/*
// Callback for received data
void receiveData(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.print("Data received.");
    Serial.println(number);

    if (number == 1) {
      if (state == 0) {
        digitalWrite(i2cSDA, HIGH); // Set 
      }
    }
  }
}
*/
// Left Turn Signal function
void leftTurnFXN() {
  //Changing state of left turn signal
  if (leftTurnPinOUT == HIGH) {
    digitalWrite(leftTurnPinOUT, LOW);
  } else {
    digitalWrite(leftTurnPinOUT, HIGH);
    delay(300);
  }
  digitalWrite(leftTurnPinOUT, LOW);
  Serial.write("Bananas. \n");
}

// Right Turn Signal function
void rightTurnFXN() {
  //Changing state of right turn signal
  if (rightTurnPinOUT == HIGH) {
    digitalWrite(rightTurnPinOUT, LOW);
  } else {
    digitalWrite(rightTurnPinOUT, HIGH);
    delay(300);
  }
  digitalWrite(rightTurnPinOUT, LOW);
  Serial.write("Oranges. \n");
}

// Headlight function
void headlightFXN() {
  if (headlight == LOW) {
    // turn lights on
    digitalWrite(headlightPinOUT, LOW);
    Serial.write("Pineapples! \n");
  }
  else {
    digitalWrite(headlightPinOUT, HIGH);
  }
}

// Brakelight function
void brakeLightFXN() {
  if (brake == HIGH) {
    // turn lights on
    digitalWrite(brakePinOUT, LOW);
    Serial.write("TOMATOES! \n");
  } else {
    digitalWrite(brakePinOUT, HIGH);
    Serial.write("Apples. \n");
  }
}
/*
// Running Lights function
void runLightFXN() {
  if (runlight == HIGH) {
    // turn lights on
    digitalWrite(runlightPinOUT, LOW);
    Serial.write("Peaches. \n");
  } else {
    digitalWrite(runlightPinOUT, HIGH);
  }
}
*/
// BMS Fault function
void bmsFaultFXN() {
  //Changing state of BMS Fault--> also flashes hazards
  if (bmsFaultPinOUT == HIGH) {
    digitalWrite(bmsFaultPinOUT, LOW);
    hazard = 1023;
  } else {
    digitalWrite(bmsFaultPinOUT, HIGH);
    delay(200);
    hazard = 0;
  }
  digitalWrite(bmsFaultPinOUT, LOW);
  Serial.write("Pomegranates! Sh*t, Pomegranates! \n");
}

// Hazards function
void hazardFXN() {
  //Changing state of hazards--> right turn and left turn flashing
  if (rightTurnPinOUT == HIGH | leftTurnPinOUT == HIGH) {
    digitalWrite(rightTurnPinOUT, LOW);
    digitalWrite(leftTurnPinOUT, LOW);
  } else {
    digitalWrite(rightTurnPinOUT, HIGH);
    digitalWrite(leftTurnPinOUT, HIGH);
    delay(200);
  }
  digitalWrite(rightTurnPinOUT, LOW);
  digitalWrite(leftTurnPinOUT, LOW);
  Serial.write("Aw, Fig! \n");
}
