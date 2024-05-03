#define PhSensorPin A0          // the pH meter Analog output is connected with the Arduino’s Analog A0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
 
void setup()
{
  pinMode(13,OUTPUT);  
  Serial.begin(9600);  
  Serial.println("Ready");    //Test the serial monitor
}
void loop()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(PhSensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  //Below 2 Lines added for our faulty chinese ph sensor for calibiration via software
  phValue=phValue-1.7;  //Our ph sensor is always giving 1.7 units extra when put in fresh water which should ideally yield 7,so subtracted this value
  phValue=14-phValue;   //Our ph sensor is showing acicic values as greater than 7 and basic values as less than 7,so to reverse it we did 14-phValue operation
  Serial.print("pH:");  
  Serial.print(phValue,2);  //We are printing the phValue with 2 decimal places
  Serial.println(" ");
  digitalWrite(13, HIGH);       
  delay(1000);
  digitalWrite(13, LOW); 
 
}
