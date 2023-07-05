#include <IRremote.h>

//MOTOR PINS
int in1 = 6;
int in2 = 7;
int in3 = 11;
int in4 = 12;

// IR receiver
const int RECV_PIN = 8;
IRrecv irrecv(RECV_PIN);
decode_results results;

// Ultrasonic sensor
const int trigPin = 5;
const int echoPin = 4;

// Left ultrasonic sensor
const int trigPinLeft = 2;
const int echoPinLeft = 3;

long duration;
int FirstDistance=0;
int SecondDistance=0;
double speed=0;
double kph = 0;
int distance=1;
float Time = 0.5;
float delayedtime = 1000*Time;


bool stopped = false;
bool on = false;
int leftCount = 0;
int rightCount = 0;

void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 

  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);

  irrecv.enableIRIn();
  irrecv.blink13(true);
}

void loop() {

  ReadRemote();

  if(on == false) {
    return;
  }

  Forward();
  CheckSensors();
  delay(300);
}

void CheckSensors() {
  int distFront = GetDistance(trigPin, echoPin);
  int distLeft = GetDistance(trigPinLeft, echoPinLeft);
  if(distFront < 40) {
    if(stopped == false)
    { 
      if(leftCount < 2)
      {
        Left();
        leftCount++;
        Serial.println("Left"); 
      } else {
        Right();
        rightCount++;
        Serial.println("Right"); 
        if(rightCount > 1) {
          leftCount = 0;
          rightCount = 0;
        }
      }
      delay(400);
    }
    Stop();
    //stopped = true;
  } 
  else {
    stopped = false;
  }

  if(distLeft < 15) {
    Left();
    delay(200);
    Stop();
  } 
}

void ReadRemote() {
    if (irrecv.decode(&results)){
      Serial.println(results.value, HEX);
 switch(results.value)
 {
   case 0x8AB3679B: 
    Serial.println("GO"); 
    on = true;
    break;
   case 0xFA3F159F: 
    Serial.println("STOP"); 
    Stop();
    on = false;
    break;
   case 0x9716BE3F:
    Left();
    delay(300);
    Stop();
    break;
   case 0xE5CFBD7F:
    Right();
    delay(300);
    Stop();
    break;
   case 0x2A89195F:
    Backwards();
    delay(400);
    Stop();
    break;
   case 0xA3C8EDDB:
    Forward();
    delay(400);
    Stop();
    break;
 }//End Case
      irrecv.resume();
      delay(200);
  }
}

void Forward() {
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void Backwards() {
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void Right() {
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void Left() {
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void Stop() {
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);
}

float GetDistance(int trig, int echo)
{
  // Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echo, HIGH);
  
  //calculating distance
  distance= duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance in cm : ");
  Serial.println(distance);
  
  return distance;
}
