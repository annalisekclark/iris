#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>


/*************************************************** 
  Speed of sound Year 7 IRIS Project
  Author: Annalise and Stephen Kennedy-Clark
  Attributions:
  Code written (pasted from several sites on the net)
  
  websites used for code:
  For the LCD
  https://www.dfrobot.com/wiki/index.php/LCD_KeyPad_Shield_For_Arduino_SKU:_DFR0009
    
  For the Pressure sensor
  http://blog.circuits4you.com/2016/04/arduino-temperature-and-bmp085-pressure.html
  and Adafruit Industries Library by Limor Fried/Ladyada  
  https://www.adafruit.com/products/391

  For the Sonar sensor
  http://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/

***************************************************
BMP085 pressure and temperature sensor
  Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!) - Orange Jumper
  Connect GND to Ground                                - Blue Jumper
  Connect SCL to i2c clock - Analog 5                  - Green Jumper
  Connect SDA to i2c data -  Analog 4                  - Yellow Jumper
  // EOC is not used, it signifies an end of conversion
  // XCLR is a reset pin, also not used here
***************************************************
HC-SR04 Ultrasonic sensor
  VCC: +5VDC                                           - Red Jumper
  Trig : Trigger (INPUT) - Pin 13                      - White Jumper 
  Echo: Echo (OUTPUT)  - Pin 12                        - Brown Jumper
  GND: GND                                             - Black Jumper
***************************************************
csv serial output format
TEMP_C, PRESSURE_PA, PING_TIME_US


****************************************************/


// Init
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Adafruit_BMP085 bmp;

float temp_c;
long pressure_pa, ping_time_us;
static char outstr[5];
int trigPin = 13;    
int echoPin = 12;    
long duration;
int interval_between_pings = 1000; // 1000 milliseconds = 1 second

///////////////////////////////////////////////////////////////////////
// the setup routine runs once when you press reset:
void setup() {
  // start serial conection to USB so we can collect data from the comouter
  Serial.begin(9600);
  bmp.begin();
  //Define inputs and outputs for sonar sensor 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  // initialize the LCD so we can display data to the little screen 
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  // Print csv header to the serial port
  Serial.println("TEMP_C, PRESSURE_PA, PING_TIME_US"); 
}

///////////////////////////////////////////////////////////////////////
// the loop routine runs over and over again forever:
void loop() {
  ////////////////////////////////
  // SONAR SENSOR
  // The Sonar sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  ////////////////////////////////
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);  
  
  ////////////////////////////////
  // TEMP & PRESSURE
  // read data from pressure and temp sensor
  temp_c       = bmp.readTemperature() -1.7;
  pressure_pa  = bmp.readPressure();  
  
  ////////////////////////////////
  // write to the serial port
  Serial.print(temp_c);
  Serial.print(", ");
  Serial.print(pressure_pa);
  Serial.print(", ");
  Serial.println(duration);  
  
  ////////////////////////////////
  // write to the LCD  
  dtostrf(temp_c, 5, 1, outstr ) ;
  lcd.clear();
  // temp
  lcd.setCursor(0,0);
  lcd.print(outstr);
  lcd.print("C");
  // pressure
  lcd.setCursor(9,1);  
  lcd.print(String(pressure_pa) + "Pa" );
  // time
  lcd.setCursor(0,1);  
  lcd.print(duration);
  lcd.print(char(228));
  lcd.print("s");
  
  ////////////////////////////////
  // delay the next loop 
  delay(interval_between_pings);
}
