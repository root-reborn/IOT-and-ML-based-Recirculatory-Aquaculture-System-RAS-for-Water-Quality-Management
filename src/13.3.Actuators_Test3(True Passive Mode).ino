//This code is useful when we want to run our entire RAS system in complete Passive Mode without any Sensors,ML model for DO prediction and IOT module.

#define TestWaterPumpPin 10  //Pin connected 5V water pump which is used for Testing Tank
#define DistilledWaterPumpPin 9  // Pin connected to 5V water connected which is used with distilled water to clean the sensors.
#define SolenoidValvePin 8    // Pin connected to 5V Solenoid (which is actually a water pump which is used as a virtual Solenoid)
//#define biofilter 9  //Pin connected to 3V aerator which is connected to BioSponge(acts as aerator+biofilter)
//#define aerator 10  //Pin connected to 9V aerator
#define led 11  //Pin connected to 9V Led bulbs
#define WaterPumpPin 12       // Pin connected to 12V water pump for Recirculation form Fish Tank to Biological Filter and Mechanical Filter(We made a single Filter which filters both Mechanical and Biological waste due to budget constrain) 


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows


void setup() {  // put your setup code here, to run once:
  pinMode(TestWaterPumpPin, OUTPUT);
  pinMode(DistilledWaterPumpPin, OUTPUT);
  pinMode(SolenoidValvePin, OUTPUT);
  //pinMode(biofilter, OUTPUT);
  //pinMode(aerator, OUTPUT);
  //digitalWrite(aerator, LOW);  //We use LOW transmitter relay,so it sends electricity when digitalWrite is LOW
  pinMode(led,OUTPUT);
  pinMode(WaterPumpPin, OUTPUT);

  digitalWrite(SolenoidValvePin, HIGH);
  digitalWrite(DistilledWaterPumpPin, HIGH);
  digitalWrite(TestWaterPumpPin, HIGH);
  digitalWrite(led, HIGH);
  digitalWrite(WaterPumpPin, HIGH);
  


  Serial.begin(9600);       //Initialize the serial monitor
  Serial.println("Ready");  //Test the serial monitor

  lcd.init(); // initialize the lcd
  lcd.backlight();
  
}


void loop() {   // put your main code here, to run repeatedly:
  
// Code for running all Actuators of RAS system after a fixed time interval.

    lcd.print("TestWaterPumpPin ON ");
    digitalWrite(TestWaterPumpPin, LOW);  // Turn on the water pump
    delay(20000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("TestWaterPumpPin OFF");
    digitalWrite(TestWaterPumpPin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();

    lcd.print("SolenoidValve ON    ");
    digitalWrite(SolenoidValvePin, LOW);
    delay(15000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("SolenoidValve OFF   ");
    digitalWrite(SolenoidValvePin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();


    lcd.print("DistillWatPump ON   ");
    digitalWrite(DistilledWaterPumpPin, LOW);  // Turn on the water pump
    delay(20000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("DistillWatPump OFF  ");
    digitalWrite(DistilledWaterPumpPin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();


    lcd.print("SolenoidValve ON    ");
    digitalWrite(SolenoidValvePin, LOW);
    delay(15000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("SolenoidValve OFF   ");
    digitalWrite(SolenoidValvePin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();
     

    lcd.print("led ON              ");
    digitalWrite(led, LOW);
    delay(10000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("led OFF             ");
    digitalWrite(led, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();

    lcd.print("WaterPumpPin ON     ");
    digitalWrite(WaterPumpPin, LOW);
    delay(40000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("WaterPumpPin OFF    ");
    digitalWrite(WaterPumpPin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();
}


