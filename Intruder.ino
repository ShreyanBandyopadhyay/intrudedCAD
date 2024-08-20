
#include <Servo.h>      // Loading Servo Motor Header Files

int inputPin = 3;       // choose the input pin (for PIR sensor)
int pirState = LOW;     // we start, assuming no motion detected
int val = 0;            // variable for reading the pin status
int buzzer = 8;         // choose pin for the buzzer
int servoPin = 5;       // Servo Pin
int ledPin = 2;         // LED Pin
int calibrationTime = 30; // calibration time in seconds
const int stableThreshold = 1000; // Time in milliseconds to check if sensor value is stable

Servo Servo1;           // creating servo object

void setup() {
  pinMode(inputPin, INPUT);  // declare sensor as input
  pinMode(buzzer, OUTPUT);   // declare buzzer as output
  pinMode(ledPin, OUTPUT);   // declare LED as output
  Servo1.attach(servoPin);   // attaching servo pin

  Serial.begin(9600);
  Serial.println("Waiting for the sensor to warm up.");
  
  for(int i = 0; i < calibrationTime; i++){
    delay(1000); // 1 second delay
    Serial.print(".");
  }
  
  Serial.println("SENSOR ACTIVE");
  Servo1.write(90);  // Initialize the servo motor to a neutral position to stop it
}

void loop() {
  val = digitalRead(inputPin);  // read input value

  Serial.print("Sensor Value: ");
  Serial.println(val); // Debugging statement to monitor sensor value

  if (val == HIGH) {            // check if the input is HIGH (motion detected)
    digitalWrite(ledPin, HIGH); // turn LED ON
    tone(buzzer, 1000, 100);    // turn buzzer on for 100 ms
    
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;

      // Make servo go to 0 degrees 
      Servo1.write(0); 
      delay(500); 

      Serial.println("Arm Moved");
      delay(500);
    }
  } else {  // No motion detected
    digitalWrite(ledPin, LOW); // turn LED OFF

    if (pirState == HIGH){
      // we have just turned off
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
      noTone(buzzer);
      Servo1.write(90);  // Move servo back to neutral position to stop it
      Serial.println("Arm Moved Back To Original Stage");
      Serial.println("Sensor ready to detect again"); // Notify that sensor is ready for new motion
    }
  }
}

// Function to check if the sensor is connected and stable
bool isSensorConnected() {
  unsigned long startMillis = millis();
  int stableCount = 0;
  int lastValue = digitalRead(inputPin);

  while (millis() - startMillis < stableThreshold) {
    int sensorValue = digitalRead(inputPin);
    delay(100);  // Delay to allow the sensor value to stabilize

    if (sensorValue == lastValue) {
      stableCount++;
    } else {
      stableCount = 0;
    }
    lastValue = sensorValue;
  }

  // Check if the sensor value was stable enough
  return stableCount > (stableThreshold / 100);  // Adjust this ratio as needed
}
