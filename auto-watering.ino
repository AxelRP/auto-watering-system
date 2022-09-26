// Ds1302 - Version: Latest. Docs: https://github.com/Treboada/Ds1302
#include <Ds1302.h>

/* Settings */
int pumpingTime = 15000;      // How much time the pump will be activated each time. In miliseconds
int moistureThreshold = 540;  // values from 250 to 700, where 250 is under water and 700 is super dry. When the value is higher than this threshold, the pump will start
int pumpingHours[] = {9, 12, 15, 18, 21}; // Hours to pump if moisture sensor allows it (24h format)

/* Pins */
int arduinoWorkingLedPin = 12;  // Yellow LED
int pumpOnLedPin = 9;           // Blue LED
int relayPin = 11;              // Signal to relay
int forcePumpPin = 7;           // Button
int moistSensorPin = 0;         // Soil Sensor input at Analog PIN A0
int CLKPin = 4;
int DATPin = 3;
int RSTPin = 2;
Ds1302 rtc(RSTPin, CLKPin, DATPin);

/* Local vars */
int moistureLevel = 0;
int timerCount = 0;
int statusLed = true;
int pumping = false;

void setup() {
  Serial.begin(9600);
  rtc.init();
  // Uncomment the following lines to set the date. Run only once and then comment it again, otherwise you will override the clock on every setup
  // Ds1302::DateTime dt = {
  //     .year = 22,
  //     .month = 9,
  //     .day = 26,
  //     .hour = 17,
  //     .minute = 37,
  //     .second = 00,
  //     .dow = Ds1302::DOW_TUE
  // };
  //rtc.setDateTime(&dt);

  pinMode(arduinoWorkingLedPin, OUTPUT);
  pinMode(pumpOnLedPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(forcePumpPin, INPUT);

  digitalWrite(arduinoWorkingLedPin, HIGH);
  digitalWrite(pumpOnLedPin, LOW);
  digitalWrite(relayPin, HIGH);
}

void loop() {
  moistureLevel = analogRead(moistSensorPin);

  if ((isTimeToPump() && moistureLevel > moistureThreshold) || digitalRead(forcePumpPin) == HIGH) {
    if (!pumping) {
      Serial.print("Pumping... (moisture: ");
      Serial.print(moistureLevel);
      Serial.println(")");
    }
    pumping = true;
    digitalWrite(arduinoWorkingLedPin, LOW);
    digitalWrite(pumpOnLedPin, HIGH);
    digitalWrite(relayPin, HIGH);
    delay(pumpingTime);  // Keep pump until pumpingTime miliseconds

  } else {
    if (pumping) {
      Serial.println("Stop pumping");
    }
    pumping = false;
    digitalWrite(pumpOnLedPin, LOW);
    digitalWrite(relayPin, LOW);
  }

  delay(1000);

  if (statusLed == true || pumping) {
    digitalWrite(arduinoWorkingLedPin, LOW);
    statusLed = false;
  } else {
    digitalWrite(arduinoWorkingLedPin, HIGH);
    statusLed = true;
  }
}

bool isTimeToPump() {
  Ds1302::DateTime now;
  rtc.getDateTime(&now);

  // Serial.print("Current Date / Time: ");
  // Serial.print(now.day);
  // Serial.print("/");
  // Serial.print(now.month);
  // Serial.print("/");
  // Serial.print("20");
  // Serial.print(now.year);
  // Serial.print(" ");
  // Serial.print(now.hour);
  // Serial.print(":");
  // Serial.println(now.minute);

  if (now.minute != 0) { // Always pump at o'clock
    return false;
  }
  if (now.second > 5) { // Only return true during the first 5 seconds of the pumping hour. With this we prevent sending a true signal only once (and not during the whole minute)
    return false;
  }
  // Loop over the pumping hours
  for (byte i = 0; i < (sizeof(pumpingHours) / sizeof(pumpingHours[0])); i++) {
    if (pumpingHours[0] == now.hour) {
      return true;
    }
  }
  return false;
}