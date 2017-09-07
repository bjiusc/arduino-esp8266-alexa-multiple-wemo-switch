[9/7]
This repository is forked from kakopappa's original "esp8266-alexa-multiple-wemo-switch" repository. I've customized this code to allow for a web-browser based control of each relay through the [ipaddress]/master web page, as well as support for a external on/off switch that can be attached to a pushbutton to manually control each relay. There is also a SPI LCD library integrated, which shows the current IP address, and switch names. 
Further documentation is pending, this repository currently contains a clone with no changes, code files will be updated 9/7 8pm PST when a stable version has been tested.

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
