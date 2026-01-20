#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(10, 11); // Create SoftwareSerial object //Rx Tx    
DFRobotDFPlayerMini myDFPlayer; // Create DFPlayer Mini object

int volume = 0;
int startPin = 7;

void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);


  if (myDFPlayer.begin(mySerial, true, false)) {
    Serial.println("DF player connected");
  }
  else {
    Serial.println("DF player not connected");
  }

  pinMode(7, INPUT);
}

void loop() {

  volume = 30;
  Serial.println(volume);

  if (digitalRead(startPin) == HIGH) {
    Serial.println("Motion detected");

    myDFPlayer.volume(volume);
    delay(40);
    
    myDFPlayer.playMp3Folder(1);
    
    unsigned long time = millis();
    int counter = 1;
    while (millis() < time + 13000) {
      Serial.println(counter);
      delay(1000);
      counter++;
    }
  }
  
}

