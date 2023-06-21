// The University of Arizona
// College of Engineering * 2021 Summer Engineering Academy *
// Biosystems Engineering Department
// Vegetation Index and Phenology Lab (vip.arizona.edu)
// Dr. Kamel Didan
// Dr. Armando Barreto Munoz
// Mr. Truman P. Combs
// Ms. Madeline Melichar
// Learn about our lab at this site vip.arizona.edu
/* 
  We work in the following fields 
* We work with NASA on their Earth Observing Satellites
* We develop Image processing tools and science algorithms 
* We teach Engineers about UAS &  Drones and Proximal sensing to monitor and observe the Environment
* We work with Big Geospatial data, the science required to work with it, and implement Machine Learning tools for smart solution to environemental monitoring 
* And More.....
*/ 


/* About this Project
========================
  This Arduino program will communicate with a temperature and humidity sensor (DHT22 or AHT10) 
  It wil then collect data and displays it to the serial monitor or serial plotter 
  If you wish you can capture this data & analyze and/or plot it
*/

// Libraries to work with the Temp and Humidity sensors 
#include <Adafruit_AHTX0.h>   // Library for AHT10

// Define the pins to where the LEDS will be attached 

#define LED_Humidity_Pin  3 // D3 PIN -  This LED will glow if Humidity exceeds a certain number
#define LED_Temp_Pin      4 // D3 PIN - This LED will glow if Temperature exceeds a certain number
#define DELAY_FAST  10
#define DELAY_SLOW  250

Adafruit_AHTX0 AHT10;       // Create an AHT object to work with the sensor and library

// Declare Global variables for Temp and Humidity to hold the data read by the sensor
float AHT10_Humidity,AHT10_Temp;

void setup() {
// Initialize the communication protocol 
  Serial.begin(9600);  // Use a 9600 Baud Rate - basically a speed of communication

// Set the LED output for humidity and Temp
  pinMode(LED_Humidity_Pin, OUTPUT); // Set the LED as an output mode so we can send a pulse to it when needed
  pinMode(LED_Temp_Pin, OUTPUT);    // Set the LED as an output mode so we can send a pulse to it when needed

// Initialize the AHT10 I2C sensor communication
  if (! AHT10.begin()) {
    //Removed this info print statement so as not to interfere with the plotting and labels
    //You can uncomment it
    //Serial.println("Could not find the AHT10 sensor. Please check wiring");
    flashError();
    while (1) delay(10);
  } else {
    //Removed this info print statement so as not to interfere with the plotting and labels
    //You can uncomment it
    //Serial.println("The AHT10 sensor was found & is ready");
  }

// Create label for the plot if needed
  Serial.flush();
  Serial.println("AHT10_Temp , AHT10_Humidity");
}
 
void loop() {

// Call a function that reads the data form the AHT10 sensor
  Get_AHT10_Data();

  // Print the data in order so it could be labelled & plotted 
  Serial.print(AHT10_Temp);Serial.print(",");Serial.print(AHT10_Humidity);
  Serial.println();

// Test the Status of Humidity and Temperature and turn ON/OFF the corresponding LED
  if (AHT10_Humidity > 30.0){
    digitalWrite(LED_Humidity_Pin, HIGH);    // Turn the LED on (HIGH is the voltage level)
    delay(DELAY_SLOW);                             // Wait for a quarter of a second
    digitalWrite(LED_Humidity_Pin, LOW);     // Turn the LED off by making the voltage LOW
    delay(DELAY_SLOW);                             // Wait for a quarter of a second
  }
  if (AHT10_Temp > 30.0){
    digitalWrite(LED_Temp_Pin, HIGH);
    delay(DELAY_SLOW);                      
    digitalWrite(LED_Temp_Pin, LOW); 
    delay(DELAY_SLOW);    
  }
}

void Get_AHT10_Data(){
 // Communicate with the humidity and temp sensor and get data
  sensors_event_t Humidity, Temp;

  // Now get the data
  AHT10.getEvent(&Humidity, &Temp); // Get Temp and humidity data
  AHT10_Humidity =Humidity.relative_humidity;
  AHT10_Temp=Temp.temperature;
}

// Indicate that there is a problem by flashing the LEDs rapidly
void flashError(){
    for(int i = 1; i < 200; i++){
      digitalWrite(LED_Humidity_Pin, HIGH);    // Turn the LED on (HIGH is the voltage level)
      delay(DELAY_FAST);                             // Wait for a quarter of a second
      digitalWrite(LED_Humidity_Pin, LOW);     // Turn the LED off by making the voltage LOW
      delay(DELAY_FAST);    
      digitalWrite(LED_Temp_Pin, HIGH);    // Turn the LED on (HIGH is the voltage level)
      delay(DELAY_FAST);                             // Wait for a quarter of a second
      digitalWrite(LED_Temp_Pin, LOW);     // Turn the LED off by making the voltage LOW
      delay(DELAY_FAST);                // Wait for a quarter of a second
    }
}