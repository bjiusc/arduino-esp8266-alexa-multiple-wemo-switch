#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <Wire.h>

//for lcd
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//****************CHANGE THESE*******************
// Enter your Wi-Fi settings
const char* ssid = "DPS surveillance-5G";
const char* password = "bombsquad";
//declare alexa callwords here
String switchName = "lights";
String switchName2 = "fan";
//define pins used for relay(s), and override physical switch pin
#define RELAY_PIN_1 0
#define RELAY_PIN_2 0
#define SWITCH 16
// pin declarations for SPI LCD
#define OLED_MOSI  13 
#define OLED_CLK   15 
#define OLED_DC    14
#define OLED_CS    2
#define OLED_RESET 12
//***********************************************


//initialize LCD pins
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);  

bool switchOverrideState;
bool switchOneState;
bool switchTwoState;

MDNSResponder mdns;
String webPage = "";

// prototypes
boolean connectWifi();

//on/off callbacks 
void switchOneOn();
void switchOneOff();
void switchTwoOn();
void switchTwoOff();

//used for time tracking for menu switching
unsigned long previousMillis = 0;  
const long interval = 7000; //wait 3 seconds before next update
bool menuChange = 0;

boolean wifiConnected = false;
UpnpBroadcastResponder upnpBroadcastResponder;

//initialize switch pointers
Switch *switchOne = NULL; 
Switch *switchTwo = NULL; 

void setup()
{
  delay(100);
  Serial.begin(9600);
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT); 
  pinMode(SWITCH, INPUT_PULLUP);
   
  // Initialize wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here with constructors. Max 14 (stable ~10)
    // Format: Alexa invocation name, local port no, on callback, off callback
    switchOne = new Switch(switchName, 80, switchOneOn, switchOneOff);
    switchTwo = new Switch(switchName2, 81, switchTwoOn, switchTwoOff);

    Serial.println("Adding switches to the UPnP broadcast responder");
    upnpBroadcastResponder.addDevice(*switchOne);
    upnpBroadcastResponder.addDevice(*switchTwo);
  }

  //initialize display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //initialize web server
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  Serial.println("All Services Started Successfully");
}
 
void loop()
{
  //code to toggle between menus
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
      
	 if(wifiConnected){
      if (menuChange) {
      printAltStatus();
      }
      else {
      printStatus(wifiConnected);
      }
      upnpBroadcastResponder.serverLoop();
      switchOne->serverLoop();
      switchTwo->serverLoop();
	 }

   //get relay status and button press status
      switchOverrideState = digitalRead(SWITCH);
      switchOneState = digitalRead(RELAY_PIN_1);
      switchTwoState = digitalRead(RELAY_PIN_2);

    //using button press status, toggle relay for switch 1
   if (switchOverrideState == 0) {
    delay(20); //debouncing
    if (switchOneState) {
      digitalWrite(RELAY_PIN_1, LOW); //turn relay off
    }
    else {
      digitalWrite(RELAY_PIN_1, HIGH); //turn relay on
    }
    delay(2000); //prevent strobing, will cause on-off-on switching if the button is pressed for more than two seconds
   }
}

void switchOneOn() {
    Serial.print("Lights turn on ...");
    digitalWrite(RELAY_PIN_1, HIGH);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
  display.println("Lights On");
  display.display();
  delay(1000);

}

void switchOneOff() {
    Serial.print("Lights turn off ...");
    digitalWrite(RELAY_PIN_2, LOW);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
  display.println("Lights Off");
  display.display();
  delay(1000);
}

void switchTwoOn() {
  Serial.print("Fan turn on ...");
  digitalWrite(RELAY_PIN_2, HIGH);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,25);
  display.println("Fan On");
  display.display();
  delay(1000);
}

void switchTwoOff() {
  Serial.print("Fan turn off ...");
  digitalWrite(RELAY_PIN_2, LOW);
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

//if connected to wifi, show READY
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

//prints at the top of the LCD the ESP8266's local IP address, can be used for webcontrol
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

//Print current temperature
void printTemp(float value)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,40);
  display.println("Temperature: ");
  display.setCursor(60,40);
  display.println(value);
}

//uses printVal to print status of wifi connection (may be redundant)
void printStatus(boolean value)
{
      printTitle();
      printVal(value);
      //printTemp(value);
      display.display();
}

//alt menu showing configuration settings
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
  display.println(switchOne->getAlexaInvokeName());
  display.setCursor(0,20);
  display.println("2:");
  display.setCursor(20,20);
  display.println(switchTwo->getAlexaInvokeName());
//display
  display.display();
}
