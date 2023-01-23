#include <rgb_lcd.h>

#include "DHT.h"
#include "Ticker.h"
#define DHTPIN 2
#define DHTTYPE DHT11
const int pinSound = A0;
const int pinLed   = 3;
const int pinButton = 8;
const int speakerPin = 6;
const int pinLight = A1;
const int potentiometer = A2;
DHT dht(DHTPIN, DHTTYPE);

class Config {
  public:
    int id;
    String name;
    int minTemp;
    int maxTemp;
    int minHumi;
    int maxHumi;
};


class Device {
  public:
    int id;
    Config config;
    float temp;
    float humi;
};

Device arduino;
Config configs[2];


int lightThresholdvalue = 400;
int soundThresholdvalue = 500;
int counter = 0;

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 255;
const int colorB = 255;

void printMessage() {
  Serial.println("Hey");
}

Ticker timer1(readTempAndHumidity, 0, 1); // once, immediately 
Ticker timer2(readTempAndHumidity, 900000); // once, immediately 

void setup() {
  // Configure LED's pin for output signals.
    pinMode(pinLed, OUTPUT);
    pinMode(potentiometer, INPUT);
    Serial.begin(9600);
    dht.begin();
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    lcd.print("Hello, world!");
    configs[0] = {id: 1, name: "Banana", minTemp: 15, maxTemp: 25, minHumi: 50, maxHumi: 80};
    timer1.start();
    timer2.start();
}

float *readTempAndHumidity(){
    // wait a few seconds between measurements.

  // read humidity
  float humi  = dht.readHumidity();
  // read temperature as Celsius
  float tempC = dht.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht.readTemperature(true);

  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT sensor!");
  }
  static float result[2];
  result[0] = humi;
  result[1] = tempC;

  Serial.print(result[0]);
  Serial.print(", ");
  Serial.print(result[1]);
  Serial.println();
  return result;
 }

 bool readLight(){
   // Read the value of the light sensor. The light sensor is an analog sensor.
    int sensorValue = analogRead(pinLight);

    // Turn the LED on if the sensor value is below the threshold.
    if(sensorValue < lightThresholdvalue)
    {
        digitalWrite(pinLed, HIGH);
        return true;
    }
    else
    {
        digitalWrite(pinLed, LOW);
        return false;
    }
 }

void loop() {
  if (!arduino.config.id) {
    lcd.clear();
    lcd.print("Select Config");
    delay(3000);
    while (!arduino.config.id) {
      int value = analogRead(potentiometer);
      Serial.println(sizeof(configs)/sizeof(Config) % (value/sizeof(configs)/sizeof(Config)));
    }
  }
  timer1.update();
  timer2.update();
//   // put your main code here, to run repeatedly:
//   float* tempandhum = readTempAndHumidity();
//   bool light = readLight();
//   Serial.println(light);
//   Serial.print(tempandhum[0]);
//   Serial.print(", ");
//   Serial.print(tempandhum[1]);
//   Serial.println();
 }
