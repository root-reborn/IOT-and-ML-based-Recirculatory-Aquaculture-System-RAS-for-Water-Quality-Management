//This code is for running our RAS system in Passive Mode.We will take readings from Sensors and Predict Dissolved Oxygen from ML model and Send this data to cloud via IOT module and Run Actuators after fixed time interval(Passive Mode).
//Components like aerator will run indefinitely.
#define PhSensorPin A0       // the pH meter Analog output is connected with the Arduino’s Analog A0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10], temp;


#include "GravityTDS.h"
#define TdsSensorPin A1
GravityTDS gravityTds;
float temperature = 25;
float tds = 0;


#define TurbiditySensorPin A2
float ntu;


#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 3                                 //Connect temperature sensor Data wire i.e yellow wire to Arduino Digital Pin 3 (Because pin 0 is reserved for Rx and pin 1 is reserved for Tx)
OneWire oneWire(ONE_WIRE_BUS);                 // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);                 // Pass our oneWire reference to Dallas Temperature sensor


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows


float dox;


//We will use Serial communication to transfer sensor data  from Arduino Mega to Nodemcu
// setting communication with nodemcu
#include <SoftwareSerial.h>       // We will use this "Software Serial Library" to create software-based serial ports on any digital pins of the Arduino board.These software-based serial ports function similarly to the hardware UART (Serial) and can be used for serial communication with peripherals like with the Nodemcu.
SoftwareSerial espSerial(5, 6);   // Creates a software serial port on pins 5 (RX) and 6 (TX) of our Arduino Mega
String str;


//Actuator Pins Define Here
#define TestWaterPumpPin 10  //Pin connected 5V water pump which is used for Testing Tank
#define DistilledWaterPumpPin 9  // Pin connected to 5V water connected which is used with distilled water to clean the sensors.
#define SolenoidValvePin 8    // Pin connected to 5V Solenoid (which is actually a water pump which is used as a virtual Solenoid)
//#define biofilter 9  //Pin connected to 3V aerator which is connected to BioSponge(acts as aerator+biofilter)
//#define aerator 10  //Pin connected to 9V aerator
#define led 11  //Pin connected to 9V Led bulbs
#define WaterPumpPin 12       // Pin connected to 12V water pump for Recirculation form Fish Tank to Biological Filter and Mechanical Filter(We made a single Filter which filters both Mechanical and Biological waste due to time and budget constrain)



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


  espSerial.begin(9600);         //initializing espSerial


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
}


void loop() {   // Put main code here, to run repeatedly
  
  //Initialize LCD Display with all our required fields
  lcd.setCursor(0, 0);            // move cursor the first row
  lcd.print("pH         :");      // print message at the first row
  lcd.setCursor(0, 1);            // move cursor to the second row
  lcd.print("TDS        :");      // print message at the second row
  lcd.setCursor(0, 2);            // move cursor to the third row
  lcd.print("Turbidity  :");      // print message at the third row
  lcd.setCursor(0, 3);            // move cursor to the fourth row
  lcd.print("Temperature:");      // print message the fourth row
  delay(1000);
  lcd.clear();

//Put water into the Testing Kit
    lcd.print("TestWaterPumpPin ON ");
    digitalWrite(TestWaterPumpPin, LOW);  // Turn on the water pump
    delay(20000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("TestWaterPumpPin OFF");
    digitalWrite(TestWaterPumpPin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();

//Now we are ready to read the values of ph,tds,turbidity and temperature paramenters once some water reached from Fish Tank to the Testing Kit.

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
  //Below 2 Lines added for our faulty ph sensor to calibirate it via software
  phValue = phValue - 1.7;  //Our ph sensor is always giving 1.7 units extra when put in fresh water which should ideally yield 7,so we subtracted this value.When we tested with lemon water(acidic) and soap water(basic) then also there is this extra 1.7 units even after calibirating the potentiometer our pH sensor.
  phValue = 14 - phValue;   //Our ph sensor is showing acidic values as greater than 7 and basic values as less than 7,so to reverse it we did 14-phValue operation
  float ph = phValue;
  Serial.print("pH:");
  Serial.print(ph, 2);  //We are printing the phValue with 2 decimal places
  Serial.println(" ");
  delay(500);

  
  //TDS Code
  //temperature = readTemperature();  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();                     //sample and calculate
  tds = gravityTds.getTdsValue();     // then get the value
  Serial.print("TDS:");
  Serial.print(tds, 0);  //We are printing the tdsValue with 0 decimal places on serial monitor
  Serial.println("ppm");
  delay(500);


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
    }else if (voltage > 4.2) {
    ntu = -1;                   //-1 is a status code indicating that sensor is faulty.
    }else {
      ntu = -1120.4*square(voltage)+5742.3*(voltage)-4353.8;   //This relationship is only valid for voltages between 2.5 and 4.2V.(3,000 to 0 turbidity).The sensor gives out 4.2V roughly at zero turbidity (clear water).                                                                                       
    }
  //If the turbidity sensor is giving analog voltage value greater than 4.2V even after caribirating its potentiometer,then it indicates that the sensor is faulty.
  //This is the case with our faulty turbidity sensor,even after calibirating its potentiometer,it is still giving analog voltage values greater than 4.2V many times(Ex:-4.74V).Consequently,the ntu value is turning out to be negative when substituting V>4.2 values in the above equation.
  //So to address this issue,let us print "-1" as the Turbidity value if we get analog voltage V>4.2 for the turbidity sensor.This "-1" status code indicates that the sensor is faulty.Let us also modify the LCD display code accordingly to reflect these changes in a new screen.
  float tur = ntu;
  Serial.print("Turbidity:");
  Serial.print(tur);  // prints the value of turbidity in ntu units on the serial monitor
  Serial.println("ntu");
  delay(500);


  //Temperature  code
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures();
  Serial.print("Temperature:");
  Serial.print(sensors.getTempCByIndex(0));
  float tem = sensors.getTempCByIndex(0);
  Serial.println("C");
  delay(500);


  // Dissolved Oxygen Prediction code
  // Dissolved Oxygen Prediction is done using Quadratic Regression Equation that has been obtained by training a Quadratic Regression Machine Learning model using the dataset "Water Quality Testing.csv" which is taken from "https://www.kaggle.com/datasets/shreyanshverma27/water-quality-testing/data"
  float con = 0.7*tds;    //Conductivity (µS/cm) = TDS (ppm)×K where K is a conversion factor that depends on the water composition and temperature. A typical value for K is around 0.5 to 0.8.We take K=0.7.We evluate conductivity value from tds value so that we can directly use it in our quadratic regression equatiuon to predict the dissolved oxygen value.
  int exit = 10;
  dox = -727.354168+0.000000*(1)+240.932430*(ph)+3.369445*(tem)-0.967652*(tur)-1.019194*(con)-17.181410*square(ph)-0.416744*(ph)*(tem)-0.332142*(ph)*(tur)+0.052905*(ph)*(con)-0.060093*square(tem)-0.100080*(tem)*(tur)+0.007691*(tem)*(con)+0.150668*square(tur)+0.011365*(tur)*(con)+0.000663*square(con);   // Since Arduino Mega Supports 32-bit floating values which means we can have 7 decimal degits of precision.So we use upto 6 decimal digit values of Bias and Weights(also called Intercept and Coefficient respectively) in our Quadratic Regression Equation
  Serial.print("Dissolved Oxygen:");
  Serial.print(dox, 1);          // prints the predicted value of dissolved oxygen with 1 decimal place on the serial monitor
  Serial.println("ppm");         // 1mg/L=1ppm of dissolved oxygen in dilute aqueous solutions.So the dissolved oxygen value obtained using the quadratic regression is in mg/L is same as in ppm units.
    if (ph >= 6.83 && ph <= 7.48 && tem >= 20.3 && tem <= 23.6 && tur >= 3.1 && tur <= 5.1 && con >= 316 && con <= 370)  //We are specifying the min and max values of each of the 4 parameters i.e ph,tem,tur,con  in the dataset on which our ML model is trained
    {     
    exit = 1;                    //Exit status 1 indicates that the value of dissolved oxygen predicted by our Quadratic Regression ML model is more likely a Accurate Prediction because all the values of ph,tem,tur,con obtained by our sensors are within the range of max and min values on which the Quadratic Regression ML model is trained.
    Serial.print("Exit Status:"); 
    Serial.println(exit);
    } else {
    exit = 0;                    //Exit status 0 indicates that the value of dissolved oxygen predicted by our Quadratic Regression ML model is more likely a Inaccurate Prediction because any of the values of ph,tem,tur,con obtained by our sensors are not within the range of max and min values on which the Quadratic Regression ML model is trained.This may sometimes exclusively occur because of a faulty sensor/not properly calibirated sensor or just naturally because of using a different water sample.
    Serial.print("Exit Status:"); 
    Serial.println(exit);
    }

  
  // Nodemcu code
  // Sending sensors values and predicted dissolved oxygen values to nodemcu in a string and the values are separated by comma.These values are then sent to the ThingsSpeak cloud platform.(IOT)
  str = String(ph)+String(",")+String(tds)+String(",")+String(tur)+String(",")+String(tem)+String(",")+String(dox);
  espSerial.println(str);   //Prints the above values on the Serial Monitor of the ESP module/Nodemcu module using commiunication b/w Arduino Mega and Nodemcu


  //20x4 LCD Display code
  lcd.setCursor(0, 0);                  // move cursor the first row
  lcd.print("pH         :");            // print message at the first row
  lcd.print(ph);
  lcd.setCursor(0, 1);                  // move cursor to the second row
  lcd.print("TDS        :");            // print message at the second row
  lcd.print(tds, 0);               //We are printing the tdsValue with 0 decimal places on LCD Display also. 
  lcd.print("ppm");      
  lcd.setCursor(0, 2);                  // move cursor to the third row
  lcd.print("Turbidity  :");            // print message at the third row
  lcd.print(tur, 0);                    //prints the Turbidity value in ntu units with 0 decimal places on LCD screen.This makes sure enough space is there for diplaying the units "ntu" on the LCD screen.
  lcd.print("ntu");
  lcd.setCursor(0, 3);                  // move cursor to the fourth row
  lcd.print("Temperature:");            // print message the fourth row
  lcd.print(tem);
  lcd.print("C");                       //Displays the values of all parameters for 10sec on the LCD screen
  delay(10000);                       

  lcd.clear();                         //For displaying Predicted Dissolved oxygen values.
  lcd.setCursor(0, 0);                 // move cursor the first row
  lcd.print("Diss Oxy:");           
  lcd.print(dox, 1);                   //We are printing the dissolved oxygen value with 1 decimal place with ppm units on LCD Display.
  lcd.print("ppm");       
  lcd.setCursor(0, 1);                 // move cursor the second row
  lcd.print("Ext Stat:");           
  lcd.print(exit);                   //We are printing the Exit Status Code of our dissolved oxygen prediction ML model.
  delay(5000);



  lcd.clear();                      //For Displaying Debugging Information
  lcd.setCursor(0, 0);            
  lcd.print("-1=>Faulty Sensor");      
  lcd.setCursor(0, 1);            
  lcd.print("Clean/Calibirate/");      
  lcd.setCursor(0, 2);            
  lcd.print("Replace the Sensor");      //Instructs to Clean/Calibirate/Replace the respective faulty sensor.
  lcd.setCursor(0, 3);            
  lcd.print("Turbidity Volt:");              //Prints the analog voltage output of the Turbidity sensor for debugging purposes.Fox example using this value we can calbirate the potentiometer of turbidity sensor accordingly.
  lcd.print(voltage);
  lcd.print("V");
  delay(3000);                          //Displays the Debugging Information for 3sec
  lcd.clear();    //Since a character written to the LCD display stays there until it is overwritten,so using lcd.clear() makes sure that all characters on LCD screen are completely erased by overwriting all positions with space character.This ensures that no character from previous display is carried on to the next display. 


//Remove water from Testing Kit
    lcd.print("SolenoidValve ON    ");
    digitalWrite(SolenoidValvePin, LOW);      //Since our solenoid value is faulty and not working,so we used another water pump to pump back the water into the Fish Tank.
    delay(15000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("SolenoidValve OFF   ");
    digitalWrite(SolenoidValvePin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();



// Code of Running Other Actuators


//Now we will put Distilled Water into Testing Kit from distilled water tank to clean the sensors.
    lcd.print("DistillWatPump ON   ");
    digitalWrite(DistilledWaterPumpPin, LOW);  // Turn on the water pump
    delay(20000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("DistillWatPump OFF  ");
    digitalWrite(DistilledWaterPumpPin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();


//Now we will remove the distilled water from Testing Kit once sensors are cleaned and leave this water into the Fish tank using a solenoid valve.
    lcd.print("SolenoidValve ON    ");
    digitalWrite(SolenoidValvePin, LOW);      //Since our solenoid value is faulty and not working,so we used another water pump to pump back the water into the Fish Tank.
    delay(15000);  // Wait for 15 seconds  as calculated from the reading taken at the time of testing
    lcd.print("SolenoidValve OFF   ");
    digitalWrite(SolenoidValvePin, HIGH);     // Turn off the water pump
    delay(10000);
    lcd.clear();
     

//Blinking of LED is analogous to "ON the water pump to pump water from Fish Tank to the Mechanical Filter".Since we didn't make a seperate biological and mechanical filter,we assume this LED as a powerful water pump which pumps the water into the mechanical filter with great force for purification.
//This Mechanical filter reduces the Turbidity(cloudiness) using different filtration layers.Mechanical Filter also reduces Total Dissolved Solvents(TDS) of the water using zeolite present in it.
    lcd.print("led ON              ");
    digitalWrite(led, LOW);
    delay(10000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("led OFF             ");
    digitalWrite(led, HIGH);   
    delay(10000);
    lcd.clear();


//This is a 12V water pump which pumps the water from Fish Tank to Biological Filter with force.The filtered water will automatically fall into the Fish Tank from Biological Filter through a hole at bottom of filter with help of gravity.
//This Biological filter contains bacteria which grows on bioballs,ceramic rings,bio sponge which breaks down the ammonia left out by the fish.Biofilter is used to decrease the alkaline nature of the water i.e to decrease the PH of the water.
    lcd.print("WaterPumpPin ON     ");
    digitalWrite(WaterPumpPin, LOW);   //Turn on the water pump
    delay(40000);  // Wait for 20 seconds  as calculated from the reading taken at the time of testing
    lcd.print("WaterPumpPin OFF    ");
    digitalWrite(WaterPumpPin, HIGH);   // Turn off the water pump
    delay(10000);
    lcd.clear();
}
