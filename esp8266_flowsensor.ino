#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>

#define WIFI_SSID "project"
#define WIFI_PASSWORD "123456789"
#define API_KEY "AIzaSyAL-eubS-eXCq0nBYkBVEo0PrCFc7AKG_g"
#define USER_EMAIL "project@gmail.com"
#define USER_PASSWORD "123456789"
#define DATABASE_URL "https://espmonitorfinalshoe-default-rtdb.asia-southeast1.firebasedatabase.app/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define RELAY1 D5
#define RELAY2 D6
#define FLOW_SENSOR A0  // Flow sensor connected to A0

SoftwareSerial arduinoSerial(D3, D4);  // RX, TX (ESP8266 to Arduino)

float totalLiters = 0.0;
const float MAX_FLOW_RATE = 30.0;  // Adjust based on your flow sensor specs
unsigned long lastMillis = 0;

void initWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected! IP address: " + WiFi.localIP().toString());
}

void initFirebase() {
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.database_url = DATABASE_URL;
    Firebase.begin(&config, &auth);
    while (auth.token.uid == "") {
        delay(1000);
    }
    Serial.println("Firebase connected!");
}

void setup() {
    Serial.begin(115200);
    arduinoSerial.begin(9600);
    
    pinMode(RELAY1, OUTPUT);
    pinMode(RELAY2, OUTPUT);
    pinMode(FLOW_SENSOR, INPUT);

    initWiFi();
    initFirebase();
    sensors.begin();
}

void loop() {
    static unsigned long lastMillis = 0;
    
    if (millis() - lastMillis >= 1000) {
        lastMillis = millis();
        
        // Read water flow sensor value
        int sensorValue = analogRead(FLOW_SENSOR);
        
        // Convert to L/min
        float flowRate = (sensorValue / 1023.0) * MAX_FLOW_RATE;

        // Calculate total liters used
        totalLiters += (flowRate / 60.0);

        Serial.print("Flow: ");
        Serial.print(flowRate);
        Serial.print(" L/min | Total: ");
        Serial.print(totalLiters);
        Serial.println(" L");

        if (Firebase.ready()) {
            Firebase.setFloat(fbdo, "/readings/flow_rate", flowRate);
            Firebase.setFloat(fbdo, "/readings/total_liters", totalLiters);
            Serial.println("✅ Flow data sent to Firebase successfully!");
        }
    }

    if (arduinoSerial.available()) {
        String receivedData = arduinoSerial.readStringUntil('\n');
        receivedData.trim();
        
        Serial.println("Received: " + receivedData);
        
        int heartRate, SpO2, CO_level;
        float waterFlow;
        
        int valuesParsed = sscanf(receivedData.c_str(), "%d,%d,%d,%f", &heartRate, &SpO2, &CO_level, &waterFlow);
        
        if (valuesParsed == 4) {  
            Serial.print("Parsed - BPM: ");
            Serial.print(heartRate);
            Serial.print(", SpO2: ");
            Serial.print(SpO2);
            Serial.print(", CO: ");
            Serial.print(CO_level);
            Serial.print(", Flow: ");
            Serial.println(waterFlow);
            
            if (Firebase.ready()) {
                Firebase.setInt(fbdo, "/readings/bpm", heartRate);
                Firebase.setInt(fbdo, "/readings/spo2", SpO2);
                Firebase.setInt(fbdo, "/readings/co_level", CO_level);
                Firebase.setFloat(fbdo, "/readings/water_flow", waterFlow);
                Serial.println("✅ Health data sent to Firebase successfully!");

                // Handle emergency alerts
                if (heartRate > 150 || SpO2 < 90) {
                    digitalWrite(RELAY1, HIGH);
                    Firebase.pushString(fbdo, "/alerts", "Emergency: High BPM/Low SpO2");
                } else {
                    digitalWrite(RELAY1, LOW);
                }

                if (CO_level > 600) {
                    digitalWrite(RELAY2, HIGH);
                    Firebase.pushString(fbdo, "/alerts", "CO Alert: Fan ON");
                } else {
                    digitalWrite(RELAY2, LOW);
                }
            }
        } else {
            Serial.println("⚠️ Data parsing error! Check input format.");
        }
    }
}
