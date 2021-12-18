#include <Servo.h>
#include <SoftwareSerial.h>

#define TRIG	2
#define ECHO	3
#define FLAME	4


#define RED		9
#define	GRN		10
#define	BLUE	11


Servo myservo;
int pos = 0, sweepAmount = 1;


unsigned long flameMillis = 0;
unsigned long ultrasonicMillis = 0; 
unsigned long trashMillis = 0;

int state = 0;


void setup() {
  
  pinMode(FLAME, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  myservo.attach(5);

  Serial.begin(9600);
}

void loop() {
  analogWrite(RED, 0);
  analogWrite(GRN, 0);
  analogWrite(BLUE,0);

  state = digitalRead(FLAME);
  
  unsigned long curMillis = millis();
  
  if (curMillis - flameMillis > 3000) {
    
  if (state == 1 ){  
    myservo.write(pos);
    pos += sweepAmount;
   
    if(pos == 0 || pos == 180){
    sweepAmount = -sweepAmount; // servo motor turns 0 to 180, 180 to 0
    
    for(flameMillis=curMillis; flameMillis>=curMillis+3000; flameMillis++);{
      digitalRead(FLAME);}
    }
    }
    delay(5);}  // if flame sensor doesnt detect flame, servo motor works
   else if (state == 0){ // if flame sensor detects flame, servo motor stops
      Serial.println("flame"); // flame 
      Serial.println();
    }

   
    if(curMillis - ultrasonicMillis > 100){ // ultrasonic and 3led start
      curMillis = ultrasonicMillis;
      
      digitalWrite(TRIG, HIGH);
      delayMicroseconds(10);
      digitalWrite(TRIG, LOW);
      
      long duration = pulseIn(ECHO, HIGH);
      
      if (duration == 0) {
        return;
      }
      
      long distance = duration * 0.017;
      
        if (distance < 15) {
        analogWrite(RED, 255); //
        }
        
       if ((distance < 15) && (curMillis - trashMillis > 1000)){
       trashMillis = curMillis;
      Serial.print("trashtrash"); // print 'trash' in 1 second interval
      Serial.println();
       }
      else if (distance < 40) {
        analogWrite(BLUE, 255);
      }
      
      else if (distance < 500) {
        analogWrite(GRN, 255);
      }
    }
 }





    
