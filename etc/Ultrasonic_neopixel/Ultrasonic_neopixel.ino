#include <Adafruit_NeoPixel.h>

int distTrigPin = 5;
int distEchoPin = 3;
#define PIN 13
#define NUM_LEDS 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

unsigned int colorCode=0; // R=1 G=2 B=3
unsigned int preColor=3; // 초기값 Blue

void setColor(unsigned int colorCode){
  if(colorCode==1){ // Red
    strip.setPixelColor(0, 255, 0, 0);
    strip.setPixelColor(1, 255, 0, 0);
    strip.setPixelColor(2, 255, 0, 0);
    strip.setPixelColor(3, 255, 0, 0);
    strip.setPixelColor(4, 255, 0, 0);
    strip.setPixelColor(5, 255, 0, 0); 
    strip.setPixelColor(6, 255, 0, 0);
    strip.setPixelColor(7, 255, 0, 0);
    Serial.println("LED color : Red");
  }
  else if(colorCode==2){ // Green
    strip.setPixelColor(0, 0, 255, 0);
    strip.setPixelColor(1, 0, 255, 0);
    strip.setPixelColor(2, 0, 255, 0);
    strip.setPixelColor(3, 0, 255, 0);
    strip.setPixelColor(4, 0, 255, 0);
    strip.setPixelColor(5, 0, 255, 0);
    strip.setPixelColor(6, 0, 255, 0);
    strip.setPixelColor(7, 0, 255, 0);
    Serial.println("LED color : Green");
  }
  else if(colorCode==3) { // Blue
    strip.setPixelColor(0, 0, 0, 255);
    strip.setPixelColor(1, 0, 0, 255);
    strip.setPixelColor(2, 0, 0, 255);
    strip.setPixelColor(3, 0, 0, 255);
    strip.setPixelColor(4, 0, 0, 255);
    strip.setPixelColor(5, 0, 0, 255);
    strip.setPixelColor(6, 0, 0, 255);
    strip.setPixelColor(7, 0, 0, 255);
    Serial.println("LED color : Blue");
  }
}
void setup() {
  pinMode(distTrigPin, OUTPUT);
  pinMode(distEchoPin, INPUT);
  Serial.begin(115200);
  strip.begin();
  strip.show();
}

void loop() {
  digitalWrite(distTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(distTrigPin, LOW);
  float duration = pulseIn(distEchoPin, HIGH);
  if(duration == 0) { return; }

  float distance = duration / 58.2;
  Serial.println(distance);

  if(distance>5){ // validity check
    if(distance<30) { // Red
      setColor(1); 
      preColor=1;
    }
    else if(distance<50) { // Green
      setColor(2); 
      preColor=2;
    }
    else{ // Blue
      setColor(3); 
      preColor=3;
    }
  } else { setColor(preColor);}
  strip.show();
  
}
