This repository is forked from kakopappa's original "esp8266-alexa-multiple-wemo-switch" repository. 
I've customized this code for increased functionality in the following ways:

  1: Web interface to control each relay through the going to the local IP address, which returns a control center web page.
  2: Support for an external physical on/off switch that can be toggled to manually control each relay. This uses debouncing to allow for state-less control of each relay.
  3: SPI LCD library integration, which shows two different menus, one with the current local IP address obtained from the DHCP server and connection status, and the other with Alexa callwords associated with each relay.
  4: Polling of a DHT11/DHT22 temperature sensor to control relay(s) automatically based on ambient temperature 
  

-------------------------------------------------------------
#Arduino Esp8266 Alexa Multiple Belkin wemo switch emulator

This project supports emulating upto 14 belkin wemo switches using 1 ESP 8266  chip.

Parts you need:

WeMos D1-mini ($4.00)  http://www.aliexpress.com/store/product/D1-mini-Mini-NodeMcu-4M-bytes-Lua-WIFI-Internet-of-Things-development-board-based-ESP8266/1331105_32529101036.html

How to use:

1. Download the code
2. Open wemos.ino in the Arduino editor.
2. Change the WI-FI settings. 
3. Define switches and callbacks
3. Flash 
4. Enjoy

## Do you want to turn on / off your TV, AirCon (AC) using Amazon Alexa? Checkout my latest project https://irdevkit.com/

Previous post on single wemo switch
https://github.com/kakopappa/arduino-esp8266-alexa-wemo-switch/edit/master/README.md


### Articles about the Wemos switch
* [monkeytypewritr](https://medium.com/@monkeytypewritr/amazon-echo-esp8266-iot-a42076daafa5#.oc4od1xa0)


### Credits

- makermusings - [GitHub](https://github.com/makermusings/fauxmo)

### Developed By

* Aruna Tennakoon
 * [paypal.me/arunat](http://paypal.me/arunat)
