#include "Arduino.h"

//초음파센서 trig, echo핀 설정 trig는 trigger이고 echo는 말그대로 되돌아옴
int distTrigPin = 5; //왜 앞에 const를 다는가 변수 이름은 distTrigpin이라고 설정한거고 그 핀 번호를 4번으로 하겠다라는 의미인가?
int distEchoPin = 3;
 
//3색 LED 핀 설정
int ledredpin = 9;
int ledgreenpin = 10;
int ledbluepin = 8;
int ledRedPin = 12;
int ledGreenPin = 11;
int ledBluePin = 13;
 
void setup() {
  pinMode(distTrigPin, OUTPUT);
  pinMode(distEchoPin, INPUT);
  Serial.begin(115200);
}
 
void loop() {
  analogWrite(ledredpin, 0);
  analogWrite(ledgreenpin, 0);
  analogWrite(ledbluepin, 0);
  
  analogWrite(ledRedPin, 0);
  analogWrite(ledGreenPin, 0);
  analogWrite(ledBluePin, 0);
 
  //거리 측정
  digitalWrite(distTrigPin, HIGH); //초음파 발사 
  // 1sec = 1000 ms, 1ms = 1000 microSec
  delayMicroseconds(10);//10ms = 0.00001sec 초음파 센서는 미세하게 다뤄야 하기때문에 충분히 발사되는데 0.0001초면 충분하다는 의미?
  digitalWrite(distTrigPin, LOW); //잠깐 쉬고 바로 꺼줌
 
  //pulseIn: pin이 HIGH가 될 때까지의 시간을 구함
  //최대 1초까지 대기, 대기 중 HIGH가 안된다면 0으로 반환
  long duration = pulseIn(distEchoPin, HIGH);//long이라고 하는 형태로 duration이라는 이름으로 변수 선언, 그 변수 값은 echo핀으로 초음파가 돌아온 시간을 넣는다.
  if(duration == 0) { //loop문 종료조건
    return;
  }
 
  // 29.1를 나누면 cm단위, 왕복이기 때문에 58.2
  int distance = duration / 58.2;
  Serial.println("duration: ");
  Serial.println(duration);
  Serial.println("distance: ");
  Serial.println(distance);
 
  if(distance < 30) {
    analogWrite(ledRedPin, 255);
    analogWrite(ledredpin, 255);
  }
  else if(distance < 50) {
    analogWrite(ledGreenPin, 255);
    analogWrite(ledgreenpin, 255);
  }
  else {
    analogWrite(ledBluePin, 255);
    analogWrite(ledbluepin, 255);
  }
 
  delay(100);
}
