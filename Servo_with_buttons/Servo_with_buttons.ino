#include <Servo.h>

Servo myServo;

int lessButtonPin = 12;
int moreButtonPin = 2;

int lessButtonState;
int moreButtonState;

int value = 179;

int step = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  myServo.attach(6);

  pinMode(lessButtonPin, INPUT);
  pinMode(moreButtonPin, INPUT);

  lessButtonState = LOW;
  moreButtonState = LOW;

  myServo.write(value);
}

void loop() {
  // put your main code here, to run repeatedly:
  moreButtonState = digitalRead(moreButtonPin);
  lessButtonState = digitalRead(lessButtonPin);

  if (lessButtonState == HIGH) {

    value -= step;
    value = constrain(value, 0, 179);
    Serial.println(value);
    myServo.write(constrain(value, 0, 179));

  }
    
  delay(15);

  if (moreButtonState == HIGH) {

    value += step;
    value = constrain(value, 0, 179);
    Serial.println(value);
    myServo.write(constrain(value, 0, 179));

  }

  Serial.print("lessButtonState: ");
  Serial.println(lessButtonState);
  Serial.print("moreButtonState: ");
  Serial.println(moreButtonState);
  delay(30);
}
