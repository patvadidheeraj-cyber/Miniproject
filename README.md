AgroSense - IoT Environmental Monitoring System

## Overview

AgroSense is an IoT-based environmental monitoring system designed to collect, store, and visualize real-time temperature, humidity, and heat index data. The system uses a DHT22 sensor connected to a microcontroller and sends sensor readings to Firebase Realtime Database. A web dashboard provides live monitoring, historical trends, alerts, and weekly analytics.

## Features

* Real-time Temperature Monitoring
* Real-time Humidity Monitoring
* Heat Index Calculation
* Firebase Realtime Database Integration
* User Authentication (Login/Register/Password Reset)
* Live Dashboard Visualization
* Weekly Analysis and Statistics
* Alert System for Threshold Violations
* Device Status Monitoring
* Responsive Web Interface

---

## Hardware Requirements

* ESP32 Development Board
* DHT22 Temperature and Humidity Sensor
* USB Cable
* Wi-Fi Connection
* Power Supply

---

## Software Requirements

### Arduino IDE

Install the following libraries:

* WiFi
* Firebase ESP Client
* DHT Sensor Library
* ArduinoJson

### Web Dashboard

* Modern Web Browser
* Firebase Project
* Internet Connection

---

## Project Structure

```text
AgroSense/
│
├── mvsr_iot_monitor.ino      # ESP32 firmware
├── AgroSense.html            # Web dashboard
├── README.md
└── assets/
```

---

## Installation

### Step 1: Clone Repository

```bash
git clone https://github.com/yourusername/agrosense.git
cd agrosense
```

### Step 2: Install Arduino Libraries

Open Arduino IDE and install:

```text
DHT sensor library
Firebase ESP Client
ArduinoJson
```

from:

```text
Tools → Manage Libraries
```

### Step 3: Configure Wi-Fi

Open the Arduino sketch and update:

```cpp
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

---

### Step 4: Configure Firebase

Replace the Firebase credentials:

```cpp
#define API_KEY "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL "YOUR_DATABASE_URL"
```

Configure Firebase Authentication and Realtime Database.

---

## Hardware Connections

| DHT22 Pin | ESP32 Pin                  |
| --------- | -------------------------- |
| VCC       | 3.3V                       |
| GND       | GND                        |
| DATA      | GPIO 4 (or configured pin) |

> Update the GPIO pin in the source code if required.

---

## Code Upload

### Select Board

```text
Tools → Board → ESP32 Dev Module
```

### Select Port

```text
Tools → Port → COMx
```

### Upload Firmware

Click:

```text
Upload (→)
```

Wait for:

```text
Hard resetting via RTS pin...
```

---

## Running the Dashboard

### Method 1: Open Directly

Open:

```text
AgroSense.html
```

in your browser.

### Method 2: Local Server (Recommended)

Python:

```bash
python -m http.server 8000
```

Open:

```text
http://localhost:8000/AgroSense.html
```

---

## Firebase Database Structure

```json
{
  "iot_readings": {
    "reading_1": {
      "temp": 29.5,
      "hum": 68,
      "heatIndex": 31.2,
      "timestamp": 1718000000
    }
  }
}
```

---

## Dashboard Features

### Live Metrics

* Temperature
* Humidity
* Heat Index
* Data Rate

### Statistics

* Average Temperature
* Maximum Temperature
* Minimum Temperature
* Average Humidity
* Heat Index Analytics

### Alerts

Automatic alerts are generated when:

```text
Temperature > 35°C
Temperature > 40°C
Humidity > 85%
Humidity < 20%
Heat Index > 35°C
Heat Index > 45°C
```

---

## System Workflow

```text
DHT22 Sensor
      ↓
ESP32
      ↓
Wi-Fi Network
      ↓
Firebase Realtime Database
      ↓
AgroSense Dashboard
      ↓
Analytics & Alerts
```

---

## Future Enhancements

* Soil Moisture Monitoring
* MQTT Integration
* SMS Alerts
* Email Notifications
* Mobile Application
* AI-Based Crop Recommendations

---

## License

This project is developed for academic and educational purposes.

---

## Authors

Dheeraj and Team

MVSR Engineering College

IoT Environmental Monitoring System Project
