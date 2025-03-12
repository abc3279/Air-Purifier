#include <SoftwareSerial.h>

SoftwareSerial bt(2, 3);

//analog
int measurePin = 0; //A0

//digital
int ledPower = 7; //D2
int red = 9;
int btn = 4;
int yellow = 5;
int fan = 6;

//cal
int samplingTime = 200;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

//bool
bool onoff = 0; //0 off, 1 on

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(ledPower, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  pinMode(fan, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(132);
  if(onoff == 1)
  {
    if(dustDensity>=0 && dustDensity<=30) //(에어코리아 기준) 좋음
      analogWrite(fan, 100);
    else if(dustDensity>30 && dustDensity<=80) // 보통
      analogWrite(fan, 255);
    else if(dustDensity>80 && dustDensity<=150) // 나쁨
      analogWrite(fan, 255);
    else if(dustDensity>150) // 매우나쁨
      analogWrite(fan, 255);
  }
  else
    analogWrite(fan, 0);
  
  
  if(digitalRead(btn) == LOW)
  {
    Serial.println("btn");
    if(onoff == 0)
      onoff = 1;
    else
      onoff = 0;
  } 
  if(onoff == 1)
  {
    analogWrite(red, 0);
    analogWrite(yellow, 255);
  }
  else
  {
    analogWrite(yellow, 0);
    analogWrite(red, 30);
  }

  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  
  voMeasured = analogRead(measurePin);
  
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured * 5.0 / 1023.0;
  dustDensity = (calcVoltage - 0.3)/0.005;

  bt.print("Dust Density : ");
  bt.print(dustDensity);
  bt.println(" ug/m3"); //unit : ug/m3
  
}
