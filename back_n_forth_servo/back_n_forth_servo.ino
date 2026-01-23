// LYS AV OG PÅ KODE
// monter servo til høyre for lysbryter, ledning ned (emilie rom)


#include <Servo.h>
Servo myServo;

// >90 = Light off
// <90 = Light on

int off = 115;
int on = 80;
int rNumber;
int tNumber;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(9);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  rNumber = int(random(1, 15));
  Serial.println("\nFirst loop");
  for (int i = 0; i < rNumber; i++) {
    Serial.print("Round\t");
    Serial.print(i + 1);
    Serial.print("\tof\t");
    Serial.println(rNumber);
    
    myServo.write(off);
    delay(1000);
    myServo.write(on);
    delay(1000);
  }

  myServo.write(off);
  delay(1000);

  myServo.write(on);
  delay(2000);

  rNumber = int(random(1, 5));
  Serial.println("\nSecond loop");
  for (int i = 0; i < rNumber; i++) {
    Serial.print("Round\t");
    Serial.print(i + 1);
    Serial.print("\tof\t");
    Serial.println(rNumber);

    tNumber = int(random(1000, 6000));
    myServo.write(off);
    Serial.print("Loop delay off: \t");
    Serial.println(tNumber);
    delay(tNumber);

    tNumber = int(random(1000, 6000));
    myServo.write(on);
    Serial.print("Loop delay on: \t");
    Serial.println(tNumber);
    delay(tNumber);
  }

  myServo.write(off);
  delay(3000);

  myServo.write(on);
  delay(5000);

  myServo.write(off);
  delay(2000);

  tNumber = int(random(1500, 3000));
  myServo.write(on);
  Serial.print("End of program delay: \t");
  Serial.println(tNumber);
  delay(tNumber);
}
