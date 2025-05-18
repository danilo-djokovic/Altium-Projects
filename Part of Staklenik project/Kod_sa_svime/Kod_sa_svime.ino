//=========================DEFINE=======================================================================
//Senzor temp. i pritiska
#include <Wire.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;
//Servo
#include <Servo.h>
Servo Servo_mg995;
//Potenciometar
#define CLK 10 //ENC_A
#define DT 11 //ENC_B
#define SW 12
//Bazer
#define Bazer 3

//=========================PROMENLJIVE==================================================================
float temperatura;
float pritisak;
int analogni_kvalitet_vazduha;
int pomocna = 0;
//Servo promenljive
int counter = 0;

//=========================FUNKCIJE==========================================================================
void Korak5_napred(){
  counter = counter +5;

  Servo_mg995.write(135);
      delay(400);
      Servo_mg995.write(90);

      Serial.print("Broj okreta: ");
      Serial.println(counter);
}

void Korak5_nazad(){
  counter = counter -5;

  Servo_mg995.write(45);
      delay(400);
      Servo_mg995.write(90);

      Serial.print("Broj okreta: ");
      Serial.println(counter);
}

void Korak10_napred(){
  counter = counter +10;

  Servo_mg995.write(135);
      delay(800);
      Servo_mg995.write(90);

      Serial.print("Broj okreta: ");
      Serial.println(counter);
}

void Korak10_nazad(){
  counter = counter -10;

  Servo_mg995.write(45);
      delay(800);
      Servo_mg995.write(90);

      Serial.print("Broj okreta: ");
      Serial.println(counter);
}
//=========================SETUP===============================================================================
void setup() {
Serial.begin(9600);
//Senzor temp. i pritiska
if (!bme.begin(0x76)) {
    Serial.println("Senzor nije pronadjen");
  }
//Senzor kvaliteta vazduha
  pinMode(8, INPUT_PULLUP); //digitalni
  pinMode(0, INPUT); //analogni
//Semafor
  pinMode(6, OUTPUT); //zelena
  pinMode(5, OUTPUT); //zuta
  pinMode(4, OUTPUT); //crvena
//Servo
  Servo_mg995.attach(9);
//Potenciometar
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
//Bazer
  pinMode(Bazer, OUTPUT);
  digitalWrite(Bazer, LOW);
}

//=========================KOD=================================================================================

void loop() {

  temperatura = bme.readTemperature();
  pritisak = bme.readPressure();
  analogni_kvalitet_vazduha = analogRead(0); 
  
  Serial.println("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  Serial.println("Pritisak: ");
  Serial.print(pritisak / 1000.0);
  Serial.println(" kPa");

  Serial.println("Kvalitet vazduha: ");
  Serial.println(analogni_kvalitet_vazduha, DEC); 

  Serial.println();

switch(pomocna){
  case 0:

      if(temperatura >= 30 || analogni_kvalitet_vazduha >= 50 ){
      pomocna = 3;
    }
      else if((temperatura >=27 && temperatura < 30) || (analogni_kvalitet_vazduha >=40 && analogni_kvalitet_vazduha < 50) ){
      pomocna = 2;
    } 
      else if((temperatura >0 && temperatura < 27) || (analogni_kvalitet_vazduha >0 && analogni_kvalitet_vazduha < 40) ){
      pomocna = 1;
    }
  break;

//zelena je case 1
  case 1:
    digitalWrite(6 , HIGH);
    digitalWrite(5 , LOW);
    digitalWrite(4 , LOW);
    delay(4000);

    if(counter == 5 || counter == 10){
      if(counter == 10){
        Korak10_nazad();
        counter =0;
      } 
      else if(counter == 5){
        Korak5_nazad();
        counter = 0;
    }
}
pomocna = 0;
  break;
  
//zuta je case 2
  case 2:
    digitalWrite(6, LOW);
    digitalWrite(5 , HIGH);
    digitalWrite(4 , LOW);
    delay(4000);

    tone(Bazer, 1700, 1000);

    if(counter == 0 || counter ==10){
      if(counter == 0){
        Korak5_napred();
        counter =5;
      }
      else if(counter == 10){
        Korak5_nazad();
        counter =5;
      }
}
pomocna = 0;
  break;

//crvena je case 3
  case 3:
    digitalWrite(6, LOW);
    digitalWrite(5 , LOW);
    digitalWrite(4 , HIGH);
    delay(4000);
    
    tone(Bazer, 1500, 1000);
    delay(1500);
    tone(Bazer, 1500, 1000);
  
    if(counter == 0 || counter == 5){
      if(counter == 5){
        Korak5_napred();
        counter = 10;
      }
      else if(counter == 0){
        Korak10_napred();
        counter = 10;
      }
    }
pomocna = 0;
  break;
}
}