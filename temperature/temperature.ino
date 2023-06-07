/*
example1-basic.ino

This example shows basic data retrieval from the SparkFun Temperature Sensor - STTS22H.

Output Data Rates: 

STTS22H_POWER_DOWN
STTS22H_ONE_SHOT  
STTS22H_1Hz       
STTS22H_25Hz      
STTS22H_50Hz      
STTS22H_100Hz     
STTS22H_200Hz     

Written by: 
Elias Santistevan @ SparkFun Electronics December, 2022

Products: 
   SparkFun Temperature Sensor - STTS2H              https://www.sparkfun.com/products/21262
   SparkFun Micro Temperature Sensor - STTS2H        https://www.sparkfun.com/products/21051
 
Repository:
     https://github.com/sparkfun/SparkFun_STTS22H_Arduino_Library

SparkFun code, firmware, and software is released under the MIT
License(http://opensource.org/licenses/MIT).

*/

#include <Wire.h>
#include "SparkFun_STTS22H.h"
#include <SFE_MicroOLED.h> 
#include "SparkFun_Ublox_Arduino_Library.h"
#include "SerialTransfer.h"

SerialTransfer myTransfer;

bool mustanswer = false;

SparkFun_STTS22H mySTTS; 

#define PIN_RESET 9
#define DC_JUMPER 1
MicroOLED oled(PIN_RESET, DC_JUMPER);

float temp; 

struct __attribute__((packed)) STRUCT {
  float time;
  float temp;
} temperatureReading;



void setup()
{

	Wire.begin();

	Serial.begin(115200);

	if( !mySTTS.begin() )
	{
		Serial.println("Did not begin.");
		while(1);
	}

  // Setup the serial transfer
  myTransfer.begin(Serial);

	Serial.println("Ready");

	// Other output data rates can be found in the description
	// above. To change the ODR or mode, the device must first be
	// powered down.
	mySTTS.setDataRate(STTS22H_POWER_DOWN);
	delay(10);
	mySTTS.setDataRate(STTS22H_1Hz);

	// Enables incrementing register behavior for the IC.
	// It is not enabled by default as the datsheet states and
	// is vital for reading the two temperature registers.
	mySTTS.enableAutoIncrement();
  
  // Set up the OLED
  delay(100);
  oled.begin();
  oled.clear(ALL);
  oled.display();
  delay(1000);
  oled.clear(PAGE);
  oled.display();

  oled.setFontType(0);
  oled.setCursor(0,0);

  temperatureReading.temp = 0;
  temperatureReading.time = 0;

	delay(100);
}

void loop()
{
  uint16_t sendSize = 0;
	// Only use data ready for one-shot mode or 1Hz output. 
	if( mySTTS.dataReady() ) 
	{

		mySTTS.getTemperatureF(&temp);

		// Temperature in different units can be retrieved
		// using the following functions.

		//mySTTS.getTemperatureC(&temp);
		//mySTTS.getTemperatureK(&temp);

		Serial.print("Temp: "); 
		Serial.print(temp);
		Serial.println("F"); 

    oled.clear(PAGE);
    oled.setCursor(0, 0);
    oled.print(temp);
    oled.display();
    delay(2000);

    temperatureReading.temp = temp;

   ///////////////////////////////////////// Stuff buffer with struct
  sendSize = myTransfer.txObj(temperatureReading, sendSize);


  ///////////////////////////////////////// Send buffer
  myTransfer.sendData(sendSize);
	} 

	// delay = 1/ODR 
	delay(1000);

}
