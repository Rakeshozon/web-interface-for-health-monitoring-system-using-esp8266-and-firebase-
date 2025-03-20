# ESP8266 Health Monitoring Dashboard

## Overview
This project is a real-time health monitoring system using the ESP8266 microcontroller. It reads and displays vital health metrics, including heart rate (BPM), SpO2 level, CO level, water flow rate, and total water usage. The data is stored in Firebase and visualized in a web-based dashboard.

## Features
- **Real-time Data Monitoring**: Displays live sensor data.
- **Firebase Integration**: Data is stored and retrieved from Firebase.
- **Responsive Dashboard**: Optimized for mobile and desktop views.
- **Alerts System**: Displays warnings when abnormal health conditions are detected.
- **Water Flow Monitoring**: Tracks real-time water usage.

## Components Used
- **ESP8266** (NodeMCU) for data transmission
- **hbt v2 ** for BPM and i priteda random variable for SpO2 since my max30100 is not working
- **Gas Sensor** (MQ-x) for CO level detection
- **Water Flow Sensor** for flow rate and total usage
- **Temperature Sensor** for temperature readings
- **Firebase Realtime Database** for data storage

## Arduino Code Overview
### Libraries and Sensor Pins
```cpp
#include <SoftwareSerial.h>

#define HEART_SENSOR A1  // Pulse sensor for heart rate
#define MQ7_SENSOR A2    // MQ7 CO sensor
#define FLOW_SENSOR A0   // Water flow sensor

SoftwareSerial espSerial(3, 2);  // TX, RX (Connect to ESP8266 RX, TX)
```

### Pulse Counting and Heart Rate Calculation
```cpp
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
```

### Setup and Loop
```cpp
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
```

## Installation
1. **Clone the Repository**
   ```sh
   git clone https://github.com/your-repo/esp8266-health-monitor.git
   cd esp8266-health-monitor
   ```

2. **Setup Firebase**
   - Create a Firebase Realtime Database.
   - Copy the Firebase API key and database URL.
   - Update the credentials in the ESP8266 firmware.

3. **Upload the Code to ESP8266**
   - Install the required libraries: `ESP8266WiFi`, `FirebaseESP8266`, `Adafruit_Sensor`
   - Compile and upload using Arduino IDE.

4. **Run the Web Dashboard**
   - Open `index.html` in a browser.
   - Ensure Firebase SDK is correctly configured.
   - Verify data updates in real time.

## Usage
- Power on the ESP8266.
- Open the web dashboard.
- Monitor real-time health and water flow data.
- Receive alerts if values exceed safe limits.

## Optimization for Mobile View
- Used **CSS Flexbox and Grid** for responsive design.
- Adjusted text sizes and spacing for better readability.
- Optimized UI elements for touch interaction.

## Future Improvements
- Add user authentication.
- Implement historical data visualization.
- Integrate additional sensors for more parameters.

## License
This project is open-source under the MIT License.

## Contact
For questions or contributions, reach out to `rakeshozon46@gmail.com` or open an issue in the repository.

---
🚀 Happy Coding! 🎯

