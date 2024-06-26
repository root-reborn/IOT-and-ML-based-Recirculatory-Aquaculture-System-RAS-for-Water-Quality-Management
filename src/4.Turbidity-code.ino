#define TurbiditySensorPin A2

void setup() {
  Serial.begin(9600); 
}
void loop() {
  int sensorValue = analogRead(TurbiditySensorPin);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.print("Turbidity:");
  Serial.print(voltage); // print out the value you read:
  Serial.println("V");
  delay(1000);
}
