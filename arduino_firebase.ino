#include <SoftwareSerial.h>

#define HEART_SENSOR A1  // Pulse sensor for heart rate
#define MQ7_SENSOR A2    // MQ7 CO sensor
#define FLOW_SENSOR A0   // Water flow sensor

SoftwareSerial espSerial(3, 2);  // TX, RX (Connect to ESP8266 RX, TX)

volatile int pulseCount = 0;
float flowRate = 0.0, totalLiters = 0.0;
const float calibrationFactor = 7.5;

int bpm = 0;  // Store heart rate
unsigned long lastPulseTime = 0;
int pulseThreshold = 550;  // Adjust based on your sensor

void pulseCounter() {
    pulseCount++;
}

// Function to measure heart rate
int measureHeartRate() {
    int pulseVal = analogRead(HEART_SENSOR);
    static int lastPulseVal = 0;
    static int beats = 0;
    static unsigned long startTime = millis();

    if (pulseVal > pulseThreshold && lastPulseVal < pulseThreshold) {  // Detect rising edge
        beats++;
    }
    lastPulseVal = pulseVal;

    if (millis() - startTime >= 10000) {  // Measure for 10 seconds
        bpm = beats * 6;  // Convert to BPM
        beats = 0;
        startTime = millis();
    }

    return bpm;
}

void setup() {
    Serial.begin(115200);
    espSerial.begin(9600);

    pinMode(FLOW_SENSOR, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR), pulseCounter, FALLING);

    Serial.println("Arduino Ready");
}

void loop() {
    int heartRate = measureHeartRate();
    int CO_level = analogRead(MQ7_SENSOR);

    // Calculate flow rate (L/min)
    flowRate = (pulseCount / calibrationFactor);
    totalLiters += (flowRate / 60.0);
    pulseCount = 0;

    int fakeSpO2 = random(90, 99);  // Simulated SpO2

    // Send data to ESP8266
    String data = String(heartRate) + "," + String(fakeSpO2) + "," + String(CO_level) + "," + String(flowRate);
    espSerial.println(data);

    Serial.println("Sent: " + data);
    delay(2000);
}
