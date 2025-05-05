#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <iostream>
#include <sstream>
#include <ESP32Servo.h>

const char* ssid = "ParagC";
const char* password = "12345678";

// Motor driver pins
const int enA = 5; // Speed control for motor A
const int enB = 23; // Speed control for motor B
const int in1 = 12;
const int in2 = 13;
const int in3 = 14;
const int in4 = 27;

// Servo pins
const int servoPin1 = 19;
const int servoPin2 = 18;

Servo servo1;
Servo servo2;

// Initialize AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print IP address to Serial Monitor
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Configure motor driver pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Attach servos to pins
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  // Print generated web page to Serial Monitor for debugging
  // Serial.println(generateWebPage());

  // Start the web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", generateWebPage());
  });
  server.on("/motorAForward", HTTP_GET, [](AsyncWebServerRequest *request){
    motorAForward();
    request->send(200);
  });
  server.on("/motorBBackward", HTTP_GET, [](AsyncWebServerRequest *request){
    motorABackward();
    request->send(200);
  });
  server.on("/motorBForward", HTTP_GET, [](AsyncWebServerRequest *request){
    motorBForward();
    request->send(200);
  });
  server.on("/motorABackward", HTTP_GET, [](AsyncWebServerRequest *request){
    motorBBackward();
    request->send(200);
  });
  server.on("/stopMotors", HTTP_GET, [](AsyncWebServerRequest *request){
    stopMotors();
    request->send(200);
  });
  server.on("/turnServo1Positive10Degrees", HTTP_GET, [](AsyncWebServerRequest *request){
    turnServo1Positive10Degrees();
    request->send(200);
  });
  server.on("/turnServo1Negative10Degrees", HTTP_GET, [](AsyncWebServerRequest *request){
    turnServo1Negative10Degrees();
    request->send(200);
  });
  server.on("/turnServo2Positive10Degrees", HTTP_GET, [](AsyncWebServerRequest *request){
    turnServo2Positive10Degrees();
    request->send(200);
  });
  server.on("/turnServo2Negative10Degrees", HTTP_GET, [](AsyncWebServerRequest *request){
    turnServo2Negative10Degrees();
    request->send(200);
  });
  server.begin();
}

void loop() {

}

String generateWebPage() {
  String page = "<!DOCTYPE html><html><head><title>ESP32 Motor Control</title></head><body>";
  page += "<h1>ESP32 Motor Control</h1>";
  page += "<h2>Motors:</h2>";
  page += "<button ontouchstart='sendCommand(\"/motorAForward\")'>Forward</button><br>";
  page += "<button ontouchstart='sendCommand(\"/motorABackward\")'>Right</button><br>";
  page += "<button ontouchstart='sendCommand(\"/motorBForward\")'>Left</button><br>";
  page += "<button ontouchstart='sendCommand(\"/motorBBackward\")'>Backward</button><br>";
  page += "<button ontouchstart='sendCommand(\"/stopMotors\")'>Stop Motors</button><br>";

  page += "<h2>Servo 1:</h2>";
  page += "<button ontouchstart='sendCommand(\"/turnServo1Positive10Degrees\")'>Turn +20°</button><br>";
  page += "<button ontouchstart='sendCommand(\"/turnServo1Negative10Degrees\")'>Turn -20°</button><br>";
  page += "<h2>Servo 2:</h2>";
  
  page += "<button ontouchstart='sendCommand(\"/turnServo2Positive10Degrees\")'>Turn +20°</button><br>";
  page += "<button ontouchstart='sendCommand(\"/turnServo2Negative10Degrees\")'>Turn -20°</button><br>";
  page += "<script>";
  page += "function sendCommand(command) {";
  page += "  fetch(command);"; // Send a GET request to the specified command
  page += "}";
  page += "</script>";
  page += "</body></html>";
  return page;
}

void motorAForward() {
  // A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 255);
  // B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 255);
}

void motorABackward() {
  // B Backward
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 255);
  // A Backward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 255);
  
}

void motorBForward() {
  // making left B Forward and A BACK
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}

void motorBBackward() {
  // making right A Forward and B BACK
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void turnServo1Positive10Degrees() {
  int currentPosition = servo1.read(); // Get the current position of servo 1
  int targetPosition = currentPosition + 20; // Calculate the target position by adding 20 degrees

  // Make sure the target position stays within the valid range of 0 to 180 degrees
  if (targetPosition > 180) {
    targetPosition = 180;
  }

  // Set servo 1 to the target position
  servo1.write(targetPosition);
}

void turnServo1Negative10Degrees() {
  int currentPosition = servo1.read(); // Get the current position of servo 1
  int targetPosition = currentPosition - 20; // Calculate the target position by subtracting 20 degrees

  // Make sure the target position stays within the valid range of 0 to 180 degrees
  if (targetPosition < 0) {
    targetPosition = 0;
  }

  // Set servo 1 to the target position
  servo1.write(targetPosition);
}

void turnServo2Positive10Degrees() {
  int currentPosition = servo2.read(); // Get the current position of servo 2
  int targetPosition = currentPosition + 20; // Calculate the target position by adding 20 degrees

  // Make sure the target position stays within the valid range of 0 to 180 degrees
  if (targetPosition > 180) {
    targetPosition = 180;
  }

  // Set servo 2 to the target position
  Serial.print("Turning servo 2 to position: ");
 // Debugging statement
  servo2.write(targetPosition);
}

void turnServo2Negative10Degrees() {
  int currentPosition = servo2.read(); // Get the current position of servo 2
  int targetPosition = currentPosition - 20; // Calculate the target position by subtracting 20 degrees

  // Make sure the target position stays within the valid range of 0 to 180 degrees
  if (targetPosition < 0) {
    targetPosition = 0;
  }

  // Set servo 2 to the target position
  Serial.print("Turning servo 2 to position: ");
// Debugging statement
  servo2.write(targetPosition);
}