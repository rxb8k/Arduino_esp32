#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// 초음파 센서 핀 설정, vcc (+) gnd (-)
const int distTrigPin = 35; // tirg: 초음파 쏨
const int distEchoPin = 34; // echo: 초음파 받음
 
// 3색 LED 핀 설정
const int red_1 = 16; // 주황색
const int green_1 = 14; // 노란색
const int blue_1 =15; // 

const int red_2 = 23;
const int green_2 = 21;
const int blue_2 = 22;

String led_state = "";

//sql 연결 설정 
IPAddress server_addr(192,168,140,1);
char user[] = "root";
char password_[] = "asdf0792";

// 와이파이 정보 설정
const char* ssid = "Smart Trashcan";
const char* password = "12183887";
String header; // HTTP request 저장하기 위한 변수
WiFiServer server(80); // 웹 서버 포트 설정

WiFiClient client; 
MySQL_Connection conn(&client); 
MySQL_Cursor* cursor; 
char INSERT_SQL[] = "INSERT INTO test_arduino.hello_arduino (message) VALUES ('Hello, Arduino!')";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in ms (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  // LED 채널 설정
  ledcAttachPin(red_1, 0);
  ledcAttachPin(green_1, 1);
  ledcAttachPin(blue_1, 2);
  ledcAttachPin(red_2, 3);
  ledcAttachPin(green_2, 4);
  ledcAttachPin(blue_2, 5);

  ledcSetup(0, 5000, 8);
  ledcSetup(1, 5000, 8);
  ledcSetup(2, 5000, 8);
  ledcSetup(3, 5000, 8);
  ledcSetup(4, 5000, 8);
  ledcSetup(5, 5000, 8);
  
  pinMode(distTrigPin, OUTPUT);
  pinMode(distEchoPin, INPUT);
  
  Serial.begin(115200);

  // Wi-fi 연결
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Wi-fi NOT CONNECTED\n");
  }
  
  // 웹 서버 구동
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();  
  
  Serial.print("Connecting to SQL... "); 
  if (conn.connect(server_addr, 3306, user, password_)) { Serial.println("OK."); } 
  else { Serial.println("FAILED."); } 
  // create MySQL cursor object 
  cursor = new MySQL_Cursor(&conn);

}

void loop(){
  // Part A. 초음파 센서를 이용한 거리 측정
  digitalWrite(distTrigPin, HIGH); //초음파 발사 
  delayMicroseconds(10); //10ms = 0.00001sec / 1sec = 1000 ms, 1ms = 1000 microSec
  digitalWrite(distTrigPin, LOW); //잠깐 쉬고 바로 꺼줌
 
  // pulseIn: Pulse의 HIGH 구간 시간 측정 -> echo 핀으로 초음파가 돌아온 시간
  // 최대 1초까지 대기, 대기 중 HIGH가 안된다면 0으로 반환
  long duration = pulseIn(distEchoPin, HIGH); 
  
  if(duration == 0) { // 초음파 센서 동작 안 하면 loop 종료
      return;
  }

  int distance = duration / 58.2;  // 29.1를 나누면 cm단위, 왕복이기 때문에 58.2
  int trash_percent = (80-distance)/60*100;
  // Serial.println(distance);
  Serial.println(trash_percent);

  // Part B. 거리 측정값에 따른 LED 색 설정
  if(distance < 30) { // Red
    ledcWrite(0, 255);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 255);
    ledcWrite(4, 0);
    ledcWrite(5, 0);
    led_state="많음";
  }
  else if(distance < 60) { // Green
    ledcWrite(0, 0);
    ledcWrite(1, 255);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
    ledcWrite(4, 255);
    ledcWrite(5, 0);
    led_state="보통";
  }
  else{ // Blue
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 255);
    ledcWrite(3, 0);
    ledcWrite(4, 0);
    ledcWrite(5, 255);
    led_state="적음";
  }
  
  // Part C. 웹 서버 구동
  WiFiClient client = server.available();   // 클라이언트 수신 대기

  Serial.println(WiFi.localIP()); 
  
  if (client) {                             // 클라이언트 연결 시
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";                // m+ake a String to hold incoming data from the client
    
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {   
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // HTML 웹 페이지 Display
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            client.println("<h2>현재 쓰레기 양</h2>");
            client.print(trash_percent);
            client.println("%</p>");
            client.println("<h2>현재 쓰레기통 상태</h2>");
            client.print(led_state);
            client.println("</p>");

            sprintf(INSERT_SQL, "INSERT INTO test.test VALUES (NOW(),%d,%d)", trash_percent, led_state); 
            
            //쿼리문 온습도
            if (conn.connected()) { cursor->execute(INSERT_SQL); //실제 excute되는 시점 }
    
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } 
      }
    header = ""; // Clear the header variable
    client.stop();    // Close the connection
    Serial.println("Client disconnected.");
    Serial.println("");
    
  delay(500);
  }
}
