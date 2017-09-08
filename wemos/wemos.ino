#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

//for web service
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;
String webPage = "";

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// pin declarations for SPI LCD
#define OLED_MOSI  13   //D1
#define OLED_CLK   15   //D0
#define OLED_DC    14
#define OLED_CS    2
#define OLED_RESET 12
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#define SWITCH 16
int switchState;

// prototypes
boolean connectWifi();

//on/off callbacks 
void lightsOn();
void lightsOff();
void fanOn();
void fanOff();

// Change this before you flash
const char* ssid = "Lorenzo_2.4";
const char* password = "";

//used for real time
String timeDate = "";

//used for time tracking
unsigned long previousMillis = 0;  
const long interval = 7000; //wait 3 seconds before next update
bool menuChange = 0;

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

//NOTICE: declare alexa callwords here
String switchName = "lights";
String switchName2 = "fan";
Switch *lights = NULL;  //pin D3, GPIO 
Switch *fan = NULL;   //pin D2, GPIO 

void setup()
{
  delay(150);
  Serial.begin(9600);
  pinMode(0, OUTPUT); //relay 
pinMode(4, OUTPUT); //relay pin
  //pinMode(14, OUTPUT); //relay pin
  pinMode(SWITCH, INPUT_PULLUP); //switch pin
   
  // Initialize wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    lights = new Switch(switchName, 80, lightsOn, lightsOff);
    fan = new Switch(switchName2, 81, fanOn, fanOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lights);
    upnpBroadcastResponder.addDevice(*fan);
  }

  //initialize display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //init web server

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  //server.begin();
  Serial.println("All Services Started");
}
 
void loop()
{
  unsigned long currentMillis = millis();
  unsigned long delta = currentMillis - previousMillis;
  if (delta >= interval) {
    previousMillis = currentMillis - 1000;
    if (menuChange == LOW) {
      menuChange = HIGH;
    } else {
      menuChange = LOW;
    }

  }
      //printStatus(wifiConnected);
      switchState = digitalRead(SWITCH);
      bool lightState = digitalRead(15);
      
	 if(wifiConnected){
      if (menuChange) {
      printAltStatus();
      }
      else {
      printStatus(wifiConnected);
      }
      upnpBroadcastResponder.serverLoop();
      fan->serverLoop();
      lights->serverLoop();

	 }

    //ext toggle switch
   if (switchState == 0) {
    delay(20); //debouncing
    if (lightState) {
      digitalWrite(0, LOW);
    }
    else {
      digitalWrite(0, HIGH);
    }
    delay(2000); //prevent strobe
   }
}

void lightsOn() {
    Serial.print("Lights turn on ...");
    digitalWrite(0, HIGH);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
  display.println("Lights On");
  display.display();
  delay(1000);

}

void lightsOff() {
    Serial.print("Lights turn off ...");
    digitalWrite(0, LOW);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
  display.println("Lights Off");
  display.display();
  delay(1000);
}

void fanOn() {
  Serial.print("Fan turn on ...");
  digitalWrite(4, HIGH);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
  display.println("Fan On");
  display.display();
  delay(1000);
}

void fanOff() {
  Serial.print("Fan turn off ...");
  digitalWrite(4, LOW);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
  display.println("Fan Off");
  display.display();
  delay(1000);
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}

void printVal(boolean value)
{
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(25,10);
  if (value == 1) {
  display.println("READY");
  }
  else {
    display.println("ERROR");
  }
}

void printTitle()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.println("IP: ");
  display.setCursor(30,0);
  display.println(WiFi.localIP());
}
void printTemp(float value)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,40);
  display.println("Temperature: ");
  display.setCursor(60,40);
  display.println(value);
}

void printStatus(boolean value)
{
      printTitle();
      printVal(value);
      //printTemp(value);
      display.display();
}

void printAltStatus()
{
//title
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("SSID: ");
  display.setCursor(35,0);
  display.println(ssid);

//body
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("1:");
  display.setCursor(20,10);
  display.println(lights->getAlexaInvokeName());
  display.setCursor(0,20);
  display.println("2:");
  display.setCursor(20,20);
  display.println(fan->getAlexaInvokeName());
//display
  display.display();
}
