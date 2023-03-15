#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SimpleTimer.h>
#include <SPI.h>
#define BLYNK_PRINT Serial

//This is the project ID sent to you from blynk, after creating the project in the blynk app
char auth[] = "AUTH code";

//Ths part contains the details of the wifi network you are connecting to for this project
char ssid[] = "SSID";                   //WiFi SSID, vitalstatistix, maximus
char pass[] = "Password";                       //WiFi Password, vit2k4eva, okli7064

//definition of variables
const int sensorPin = 4; 
int sensorState = 0;
int lastState = 0;
#define DHTPIN 14    
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

/*Analog*/
#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750   // Define min value we consider soil 'dry'


void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(sensorPin, INPUT);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  Blynk.begin(auth, ssid, pass);
}


int sensor = 0;
int sensormap = 0;

void sendTemps()
{
sensor = analogRead(A0);
sensormap = map(sensor, 1024, 0, 0, 100);
Serial.println(sensor);
Serial.println(sensormap);
Blynk.virtualWrite(V2, sensormap);
delay(1000);
}


void loop()
{ 
  Blynk.run();
  timer.run();
  sendTemps();
  
/*DIGITAL
sensorState = digitalRead(sensorPin);
Serial.println(sensorState);

if (sensorState == 1 && lastState == 0) {
  Serial.println("needs water, send notification");
  Blynk.notify("Water your plants");
  lastState = 1;
  delay(1000);
    } 
  else if (sensorState == 1 && lastState == 1) {
    //do nothing, has not been watered yet
  Serial.println("has not been watered yet");
  delay(1000);
  }
  else {
    //st
    Serial.println("does not need water");
    lastState = 0;
    delay(1000);
  }
  */

 /* ANALOG-----------------------------------------------------*/
    if ((sensor > soilDry) && (lastState == 0)) {
    Serial.println("Status: Soil is too dry");
    Blynk.notify("Water your plants");
    lastState = 1;
    delay(1000);
  } else if ((sensor < soilWet) && (lastState == 1)) {
    Serial.println("Status: Soil is too wet");
    Blynk.notify("That's enough watering");
    lastState = 0;
    delay(1000);
  } else if((sensor < soilDry) && (sensor > soilWet)){
    Serial.println("Status: Soil moisture is perfect");
    delay(1000);
    } else {
   Serial.println("Water your Plants now");
   delay(1000);
  }

    
  delay(100);
}
