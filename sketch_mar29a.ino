#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <Servo.h>

#define WIFI_SSID "Pixel_8171"
#define WIFI_PASSWORD "88c2ca9d056a"

#define FIREBASE_HOST "conwings-eb540.firebaseio.com"
#define FIREBASE_AUTH "RfgeplEch5BUbXMkQOIbZZJpTcAN6kIQ1wXaQzL9"

FirebaseData firebaseData;
Servo leftWing;
Servo rightWing;

String path = "/Data";
String currentState = "closed";
unsigned long previousMillis = 0;
const long interval = 3000;

void openWings() {
  if (currentState != "opened") {
    Serial.println("Opening wings");
    leftWing.write(60);
    rightWing.write(0);
    currentState = "opened";
  } else {
    Serial.println("Wings are already opened");
    Serial.println("No action taken");
  }
}

void closeWings() {
  if (currentState != "closed") {
    Serial.println("Closing wings");
    leftWing.write(0);
    rightWing.write(60);
    currentState = "closed";
  } else {
    Serial.println("Wings are already closed");
    Serial.println("No action taken");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  leftWing.attach(2);
  rightWing.attach(16);

  closeWings();
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.println("*********************************");
    Serial.println("Three seconds have passed");
    Serial.println("Retrieving data from firebase");
    Serial.println("*********************************");

    if (Firebase.getString(firebaseData, "/Data/wingStatus")) {
      if (firebaseData.dataType() == "string") {
        Serial.println(firebaseData.stringData());
        firebaseData.stringData() == "open" ? openWings() : closeWings();
      } else {
        Serial.println("Wrong datatype returned");
      }
    } else {
      Serial.println(firebaseData.errorReason());
    }
  }
}
