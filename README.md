IoT Sensor Monitoring System

This is a simple IoT project that monitors sensor data in real time using an ESP8266 and Firebase.
The data is displayed on a web dashboard so it can be viewed live from anywhere.

---

About the Project

The ESP8266 collects data from connected sensors and sends it to Firebase.
The dashboard fetches this data and updates continuously, showing the latest readings.

---

Features

* Real-time sensor data updates
* Web-based dashboard
* Firebase integration
* Easy to understand and modify

---

Technologies Used

* ESP8266 (NodeMCU)
* Arduino (C/C++)
* Firebase Realtime Database
* HTML, CSS, JavaScript

---

Folder Structure

```
Miniproject/
│
├── DASHBOARD/              # Dashboard files  
├── mvsr_iot_monitor_2_/    # Arduino code  
```

---

How to Run

1. Open the Arduino code (`.ino` file)
2. Update WiFi name and password
3. Add your Firebase details
4. Upload the code to ESP8266
5. Open the dashboard (`index.html`) in a browser

---

Notes

* Make sure WiFi credentials are correct
* Do not upload sensitive data (API keys, passwords) publicly

---

Author

Dheeraj
