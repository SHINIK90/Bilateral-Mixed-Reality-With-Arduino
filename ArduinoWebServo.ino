#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(10, 11); // RX, TX
Servo myServo;
int servoPin = 9;

void setup() {
  // Start the hardware serial communication
  Serial.begin(9600);
  // Attach the servo to the specified pin
  myServo.attach(servoPin);
  // Start the software serial communication
  mySerial.begin(9600);
  Serial.println("Arduino Uno ready to communicate with ESP32");
}

void loop() {
  // Read data from the hardware serial (coming from Serial Monitor or another source)
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n'); // Read the incoming data as a string
    int angle = data.toInt(); // Convert the string to an integer
    mySerial.println(data); // Send the data to the ESP32
    Serial.println(angle); // Output the received angle for debugging
    myServo.write(angle); // Set the servo position
  }

  // Read data from the software serial (coming from ESP32)
  if (mySerial.available()) {
    String input = mySerial.readStringUntil('\n'); // Read the incoming data as a string
    int angle = input.toInt(); // Convert the string to an integer
    Serial.println(angle); // Output the received angle for debugging
    myServo.write(angle); // Set the servo position
  }
}
