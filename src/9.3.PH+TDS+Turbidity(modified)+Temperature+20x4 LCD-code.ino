#define PhSensorPin A0       // the pH meter Analog output is connected with the Arduino’s Analog A0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10], temp;


// #include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A1
GravityTDS gravityTds;
float temperature = 25, tdsValue = 0;


#define TurbiditySensorPin A2
float ntu;


#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is conntec to the Arduino digital pin 3
#define ONE_WIRE_BUS 3  //Connect temperature sensor output i.e yellow wire to Digital Pin 3 (Because pin 0 is reserved for Rx and pin 1 is reserved for Tx)
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows


void setup() {   // Put setup code here, to run once

  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ready");  //Test the serial monitor


  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);       //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();            //initialization


  sensors.begin();


  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 1);               // move cursor to the second row
  lcd.print("  EDP Project sem-6");  // print message at the second row
  lcd.setCursor(0, 2);               // move cursor to the third row
  lcd.print("  Dr.Munesh Singh");    // print message at the third row
  delay(5000);
  lcd.clear();

  lcd.setCursor(0, 0);            // move cursor the first row
  lcd.print("pH         :");      // print message at the first row
  lcd.setCursor(0, 1);            // move cursor to the second row
  lcd.print("TDS        :");      // print message at the second row
  lcd.setCursor(0, 2);            // move cursor to the third row
  lcd.print("Turbidity  :");      // print message at the third row
  lcd.setCursor(0, 3);            // move cursor to the fourth row
  lcd.print("Temperature:");      // print message the fourth row
}


void loop() {   // Put main code here, to run repeatedly
  
  //PH Code
  for (int i = 0; i < 10; i++)  //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = analogRead(PhSensorPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++)  //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++)  //take the average value of 6 center sample
    avgValue += buf[i];
  float phValue = (float)avgValue * 5.0 / 1024 / 6;  //convert the analog into millivolt
  phValue = 3.5 * phValue;                           //convert the millivolt into pH value
  //Below 2 Lines added for our faulty chinese ph sensor for calibiration via software
  phValue = phValue - 1.7;  //Our ph sensor is always giving 1.7 units extra when put in fresh water which should ideally yield 7,so we subtracted this value
  phValue = 14 - phValue;   //Our ph sensor is showing acidic values as greater than 7 and basic values as less than 7,so to reverse it we did 14-phValue operation
  Serial.print("pH:");
  Serial.print(phValue, 2);  //We are printing the phValue with 2 decimal places
  Serial.println(" ");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);

  
  //TDS Code
  //temperature = readTemperature();  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();                     //sample and calculate
  tdsValue = gravityTds.getTdsValue();     // then get the value
  Serial.print("TDS:");
  Serial.print(tdsValue, 0);  //We are printing the tdsValue with 0 decimal places on serial monitor
  Serial.println("ppm");
  delay(1000);


  //Turbidity code
    for (int i = 0; i < 10; i++)  //Get 10 samples of analog voltage values from the turbidity sensor 
  {
    buf[i] = analogRead(TurbiditySensorPin);    // reads the input on analog pin A2
    delay(10);                                                     //10 milliseconds between each readings helps stabilize the readings and reduce noise.
  }
  for (int i = 0; i < 9; i++)  //sort the analog voltage values from small to large
  {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++)  //Take average of the middle six values (index 2 to 7).Ignoring the first and last values help mitigate the effects of outliers on the average.
   avgValue += buf[i];
  avgValue=(float)avgValue/6;  //The (float) part casts the variable avgValue to a floating-point number.This is done to ensure that the division operation that follows produces a floating-point result rather than integer.
  float voltage = avgValue * (5.0 / 1024.0);      // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  if(voltage < 2.5){       //Converting analog voltage from turbidity sensor to Nephelometric Turbidity Units(NTU)
      ntu = 3000;
    }else{
      ntu = -1120.4*square(voltage)+5742.3*(voltage)-4353.8;   //This relationship is only valid for voltages between 2.5 and 4.2V.(3,000 to 0 turbidity).The sensor gives out 4.2V roughly at zero turbidity (clear water).                                                                                       
    }
  Serial.print("Turbidity:");
  Serial.print(ntu);  // prints the value of turbidity in ntu units on the serial monitor
  Serial.println("ntu");
  delay(1000);


  //Temperature  code
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures();
  Serial.print("Temperature:");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println("C");
  delay(1000);


  //20x4 LCD Display code
  lcd.setCursor(0, 0);                  // move cursor the first row
  lcd.print("pH         :");            // print message at the first row
  lcd.print(phValue);
  lcd.setCursor(0, 1);                  // move cursor to the second row
  lcd.print("TDS        :");            // print message at the second row
  lcd.print(tdsValue, 0);               //We are printing the tdsValue with 0 decimal places on LCD Display also. 
  lcd.print("ppm");      
  lcd.setCursor(0, 2);                  // move cursor to the third row
  lcd.print("Turbidity  :");            // print message at the third row
  lcd.print(ntu, 0);                    //prints the Turbidity value in ntu units with 0 decimal places on LCD screen.This makes sure enough space is there for diplaying the units "ntu" on the LCD screen.
  lcd.print("ntu");
  lcd.setCursor(0, 3);                  // move cursor to the fourth row
  lcd.print("Temperature:");            // print message the fourth row
  lcd.print(sensors.getTempCByIndex(0));
  lcd.print("C");
  delay(1000);
}

