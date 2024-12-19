#include <Servo.h>
#include <EEPROM.h>//used to store recorded values
Servo myServo;
float resolution = 1000;//MUST be less than EEPROM.length()
float recordTime = 5; //delay time
bool recording = false;
void setup() {
 pinMode(13, OUTPUT); //status led
 pinMode(2, OUTPUT);
 myServo.attach(8);
 myServo.attach(9);
 myServo.attach(11);
 myServo.attach(12);
 Serial.begin(9600);
 digitalWrite(2, HIGH);
 //Serial.println(EEPROM.length());
}
void loop() {
 if (recording == true) {//record
   for (int i = 1; i <= resolution; i++) {
     digitalWrite(13, HIGH); //light status led
     int val = map(analogRead(A0), 0, 1023, 0, 180);
     int val1 = map(analogRead(A1), 0, 1023, 0, 180);
     int val2 = map(analogRead(A2), 0, 1023, 0, 180);
     int val3 = map(analogRead(A3), 0, 1023, 0, 180);

     EEPROM.write(i, val);
     //Serial.println(EEPROM.read(i));
     myServo.write(val);
     myServo.write(val1);
     myServo.write(val2);
     myServo.write(val3);
     delay(recordTime);
   }
   digitalWrite(13, LOW); //turn off status led
   delay(1000);//give time for person
   recording = false;
 }
 else {
   for (int i = 1; i <= resolution; i++) {//playback
     if (digitalRead(2) == 0) {//stop playback and record new values
       recording = true;
       break;
     }
     int readval = EEPROM.read(i);
     myServo.write(readval);
     //Serial.println(readval);
     delay(recordTime);
   }
   digitalWrite(13, HIGH); //show a new repeat
   delay(100);
   digitalWrite(13, LOW);
 }
}