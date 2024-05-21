#define TestWaterPumpPin 3  //Pin connected 5V water pump which is used for Testing Tank
#define DistilledWaterPumpPin 7  // Pin connected to 5V water connected which is used with distilled water to clean the sensors.
#define SolenoidValvePin 8    // Pin connected to 5V Solenoid (which is actually a water pump which is used as a virtual Solenoid)
#define biofilter 9  //Pin connected to 3V aerator which is connected to BioSponge(acts as aerator+biofilter)
#define aerator 10  //Pin connected to 9V aerator
#define led 11  //Pin connected to 9V Led bulbs
#define WaterPumpPin 12       // Pin connected to 12V water pump for Recirculation form Fish Tank to Biological Filter and Mechanical Filter(We made a single Filter which filters both Mechanical and Biological waste due to budget constrain) 

void setup() {  
  pinMode(TestWaterPumpPin, OUTPUT);
  pinMode(DistilledWaterPumpPin, OUTPUT);
  pinMode(SolenoidValvePin, OUTPUT);
  pinMode(biofilter, OUTPUT);
  pinMode(aerator, OUTPUT);
  //digitalWrite(aerator, LOW);  //We use LOW transmitter relay,so it sends electricity when digitalWrite is LOW
  pinMode(led,OUTPUT);
  pinMode(WaterPumpPin, OUTPUT);
  
  Serial.begin(9600);       //Initialize the serial monitor
  Serial.println("Ready");  //Test the serial monitor
}


void loop() {   
// Code of Testing each actuator by ON and OFF
    Serial.println("Checking ON.....");
    digitalWrite(DistilledWaterPumpPin, HIGH);  // Turn on the water pump
    delay(10000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    Serial.println("ON Working");
    Serial.println("Checking OFF.....");
    digitalWrite(DistilledWaterPumpPin, LOW);   // Turn off the water pump
    delay(5000);
    Serial.println("OFF Working");
// Replace  with each actuator pin to test for each actuator to check whether each actuator is working or not.
}



//IDEA:-
// We will run Actuators in our RAS system in 2 modes i.e Passive Mode and Active Mode.
// In Passive mode we will run actutors in our Recirculation system continuously after a fixed time interval.
// In Active mode we will run actutors in our Recirculation system only if the values of ph,tds,turbidity are abnormal when compare with a threshold values(these threshold values depends on the type of fish beed we cultivate in our RAS aquaculture).
// There will be no change in the functionality of Sensors,Wifimodule and ML module irrespective of whether we use Active mode or Passive mode.The Active mode or Passive mode should only affect the Actutators.
// The Passive mode is very useful when the farmer want to run the RAS system at a particular time everyday,suppose at 6:00am morning and at 5:00pm evening everyday.Note that running RAS system in Passive mode required only actuators,there is no need of any sensors.So passive mode is like a dirt cheap automation in RAS system.
//The Active mode is useful when the farmer wants to run the RAS system only when abnormality in the ph,tds,turbidity parameters is found.So to run our RAS system we need both sensors(which are present in Testing kit) and actuators.

