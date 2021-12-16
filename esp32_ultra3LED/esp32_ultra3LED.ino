#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

// 초음파 센서 핀 설정, VCC (+) GND (-)
const int distTrigPin = 35; // tirg: 초음파 쏨
const int distEchoPin = 34; // echo: 초음파 받음
// float duration=0;

// LED 핀 설정
#define PIN 14
#define NUM_LEDS 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

unsigned int colorCode=0; // R=1 G=2 B=3
unsigned int preColor=3; // 초기값 Blue
String led_state = "";
String textColor = "";

// 와이파이 정보 설정
const char* ssid = "Smart Trashcan";
const char* password = "12183887";
String header; // HTTP request 저장하기 위한 변수
WiFiServer server(80); // 웹 서버 포트 설정
            
String htmlHeader="<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"icon\" href=\"data:,\"></head>";

unsigned long currentTime = millis(); // Current time
unsigned long previousTime = 0; // Previous time
const long delayTime = 2000; // Define timeout time in ms (example: 2000ms = 2s)

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
    led_state="많음";
    textColor="red";
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
    led_state="보통";
    textColor="green";
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
    led_state="적음";
    textColor="blue";
    Serial.println("LED color : Blue");
  }
}

void setup() {
  pinMode(distTrigPin, OUTPUT);
  pinMode(distEchoPin, INPUT);
  
  strip.begin();
  strip.show();
  
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Wi-fi NOT CONNECTED\n");
  }
  
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  // Part A. 초음파 센서를 이용한 거리 측정
  digitalWrite(distTrigPin, HIGH); //초음파 발사 
  delayMicroseconds(10); //10ms = 0.00001sec / 1sec = 1000 ms, 1ms = 1000 microSec
  digitalWrite(distTrigPin, LOW); //잠깐 쉬고 바로 꺼줌
  long duration = pulseIn(distEchoPin, HIGH);
  if(duration == 0) { //loop문 종료조건
    return;
  }
  // pulseIn: Pulse의 HIGH 구간 시간 측정 -> echo 핀으로 초음파가 돌아온 시간
  // 최대 1초까지 대기, 대기 중 HIGH가 안된다면 0으로 반환
//  if(digitalRead(distEchoPin)==LOW){
//    duration = pulseIn(distEchoPin, HIGH);
//  } else{
//    pinMode(distEchoPin, OUTPUT);
//    digitalWrite(distEchoPin, LOW);
//    pinMode(distEchoPin, INPUT);
//  }
  //if(duration == 0) { return; } // 초음파 센서 동작 안 하면 loop 종료

  int distance = duration / 58.2;
  // float distance = duration/58.2;
  Serial.println(distance);
  
  int trash_percent = (80-distance)/60*100;
  if(trash_percent<0) trash_percent=0;


  // Part B. 거리 측정값에 따른 LED 색 설정
  if(distance>5){ // validity check
    if(distance<30) { // Red
      setColor(1);
      preColor=1;
    }
    else if(distance<60) { // Green
      setColor(2);
      preColor=2;
    }
    else{ // Blue
      setColor(3);
      preColor=3;
    }
  } else { setColor(preColor);}
  strip.show();
/*

  // Part C. 웹 서버 구동
  WiFiClient client = server.available();   // 클라이언트 수신 대기
  Serial.println(WiFi.localIP()); 
  if (client) {                             // 클라이언트 연결 시
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";  // make a String to hold incoming data from the client
    
    while (client.connected() && currentTime - previousTime <= delayTime) {
      currentTime = millis();
      if (client.available()) {   
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // HTML 웹 페이지 Display
            client.println(htmlHeader);
            client.println("<body><h2 style=\"background-color:#009C7B; height: 50px;color: white; text-align: center;\">실시간 쓰레기 양 확인</h1>");
            client.println("<h1 style=\"text-align:center;font-size:72pt;color:");
            
            client.println(textColor);
            client.println(";\">");
            client.print(trash_percent);
            client.println("%</p>");
            
            client.println("</h1><div style=\"padding: 5%;\"><div style=\"display:flex;\"><h2 style=\"width: 80%;\">현재 쓰레기 양</h2><h2>");
            client.print(trash_percent);
            client.println("%</p>");
            client.println("</div><div style=\"display:flex;\"><h2 style=\"width:80%;\">현재 쓰레기통 상태</h2><h2>");
            client.print(led_state);
            client.println("</p><h2></div>");
    
            client.println();  // The HTTP response ends with another blank line
            break;            // Break out of the while loop
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } 
      }
    header = ""; // Clear the header variable
    client.stop();    // Close the connection
    Serial.println("Client disconnected.");
    Serial.println("");*/
    
  delay(100);
}
