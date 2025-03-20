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
- **HBT v2 ** ) for BPM and for  SpO2 i make a random value to generate since my max30100 is not working 
- **Gas Sensor** (MQ-x) for CO level detection
- **Water Flow Sensor** for flow rate and total usage
- **Temperature Sensor** for temperature readings
- **Firebase Realtime Database** for data storage

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
For questions or contributions, reach out to `your-email@example.com` or open an issue in the repository.

---
🚀 Happy Coding! 🎯

