const int trigPin = 12;
const int echoPin = 13;

void setup(){
    Serial.begin(9600);
 
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}
 
void loop(){
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    float duration = pulseIn(echoPin, HIGH); 
    float distance = duration / 58;
    Serial.println(distance);
    delay(100);
}
