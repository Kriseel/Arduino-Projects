#include <Servo.h>

Servo myServo;

int sensorPin = A0;
int sensorVal = 0;

float voltage = 0;
int oldTemp = 0;
int newTemp = 0;

int angle;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(9);

  Serial.begin(9600);
  
  sensorVal = analogRead(sensorPin);
  voltage = (sensorVal / 1024.0) * 5.0;
  oldTemp = (voltage - 0.5) * 100;
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorVal = analogRead(sensorPin);
  voltage = (sensorVal / 1024.0) * 5.0;
  newTemp = (voltage - 0.5) * 100;

  if(newTemp != oldTemp) {
    Serial.print("\nTemp: ");
    Serial.print(newTemp);

    angle = constrain(map(newTemp, 10, 40, 0, 179), 0, 179);

    Serial.print(", angle: ");
    Serial.print(angle);

    myServo.write(angle);
  }
  
  delay(15);
}
