#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <ArduinoJson.h>

// ── CHANGE ONLY THESE 2 LINES ─────────────────────────────────
#define WIFI_SSID     "Dotspot"
#define WIFI_PASSWORD "abcd1234"
// ──────────────────────────────────────────────────────────────

#define FIREBASE_HOST "iot-sensor-monitor-327e9-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyD19iohWC2iLpcrjPsQEnCElIyRSRPHeYY"

#define DHTPIN  D4
#define DHTTYPE DHT22
#define READ_INTERVAL_MS 10000

DHT dht(DHTPIN, DHTTYPE);

float calcHeatIndex(float tempC, float humidity) {
  float tempF = tempC * 9.0 / 5.0 + 32.0;
  float hi = 0.5 * (tempF + 61.0 + ((tempF - 68.0) * 1.2) + (humidity * 0.094));
  if (hi > 79) {
    hi = -42.379
       + 2.04901523  * tempF
       + 10.14333127 * humidity
       - 0.22475541  * tempF * humidity
       - 0.00683783  * tempF * tempF
       - 0.05481717  * humidity * humidity
       + 0.00122874  * tempF * tempF * humidity
       + 0.00085282  * tempF * humidity * humidity
       - 0.00000199  * tempF * tempF * humidity * humidity;
  }
  return (hi - 32.0) * 5.0 / 9.0;
}

void connectWiFi() {
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(1000);

  // Let DHCP assign IP automatically (static IP removed)
  WiFi.config(0U, 0U, 0U);

  Serial.print("\n[WiFi] MAC Address : ");
  Serial.println(WiFi.macAddress());
  Serial.print("[WiFi] Connecting to: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;

    if (attempts % 10 == 0) {
      int s = WiFi.status();
      Serial.print(" [status:");
      Serial.print(s);
      Serial.print("] ");

      // Print human-readable status
      if (s == WL_CONNECT_FAILED)   Serial.print("(CONNECT_FAILED - wrong password?) ");
      if (s == WL_NO_SSID_AVAIL)    Serial.print("(SSID not found) ");
      if (s == WL_CONNECTION_LOST)  Serial.print("(CONNECTION_LOST) ");
      if (s == WL_DISCONNECTED)     Serial.print("(DISCONNECTED) ");

      // Every 20 attempts, retry with a fresh begin
      if (attempts % 20 == 0) {
        Serial.println("\n[WiFi] Retrying connection...");
        WiFi.disconnect(true);
        delay(1000);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      }
    }

    if (attempts > 60) {
      Serial.println("\n[WiFi] Could not connect after 60 attempts. Restarting ESP...");
      delay(1000);
      ESP.restart();
    }
  }

  Serial.println("\n[WiFi] Connected!");
  Serial.print("[WiFi] IP Address : "); Serial.println(WiFi.localIP());
  Serial.print("[WiFi] Gateway    : "); Serial.println(WiFi.gatewayIP());
  Serial.print("[WiFi] Subnet     : "); Serial.println(WiFi.subnetMask());
  Serial.print("[WiFi] Signal     : "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
}

void pushToFirebase(float temp, float hum, float heatIdx) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[Firebase] No Wi-Fi, skipping push.");
    return;
  }

  StaticJsonDocument<256> doc;
  doc["temp"]      = String(temp, 1);
  doc["hum"]       = String(hum, 0);
  doc["heatIndex"] = String(heatIdx, 1);
  doc["timestamp"] = millis();
  doc["sensor"]    = "DHT22";
  doc["location"]  = "MVSR-AgriLab";

  String payload;
  serializeJson(doc, payload);

  String url = "https://" + String(FIREBASE_HOST)
             + "/iot_readings.json?auth=" + String(FIREBASE_AUTH);

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  if (!http.begin(client, url)) {
    Serial.println("[Firebase] http.begin() failed!");
    return;
  }

  http.addHeader("Content-Type", "application/json");
  http.setTimeout(10000);

  int code = http.POST(payload);

  if (code == 200 || code == 201) {
    Serial.println("[Firebase] Data pushed successfully!");
    Serial.print("[Firebase] Response: ");
    Serial.println(http.getString());
  } else if (code < 0) {
    Serial.print("[Firebase] Connection error: ");
    Serial.println(http.errorToString(code));
  } else {
    Serial.print("[Firebase] Server error. HTTP code: ");
    Serial.println(code);
  }
  http.end();
}

void setup() {
  Serial.begin(9600);   // Changed from 9600 to 115200 for faster/cleaner output
  delay(500);
  Serial.println("\n========================================");
  Serial.println("  MVSR IoT Monitor - DHT22 + Firebase  ");
  Serial.println("========================================");

  dht.begin();
  delay(2000);

  connectWiFi();
  Serial.println("[System] Ready! Reading every 10 seconds.\n");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Connection lost. Reconnecting...");
    connectWiFi();
  }

  float hum  = dht.readHumidity();
  float temp = dht.readTemperature();

  if (isnan(hum) || isnan(temp)) {
    Serial.println("[DHT22] Read failed! Check wiring on pin D4.");
    delay(2000);
    return;
  }

  float heat = calcHeatIndex(temp, hum);
  int score  = constrain((int)(100 - abs(temp - 25) * 2.5 - abs(hum - 65) * 0.8), 0, 100);

  Serial.println("------------------------------");
  Serial.print("[DHT22] Temp      : "); Serial.print(temp, 1); Serial.println(" C");
  Serial.print("[DHT22] Humidity  : "); Serial.print(hum,  0); Serial.println(" %");
  Serial.print("[DHT22] Heat Index: "); Serial.print(heat, 1); Serial.println(" C");
  Serial.print("[Score] Crop Score: "); Serial.print(score);   Serial.println(" / 100");

  if (temp > 35) Serial.println("[ALERT] High temperature! Crops at risk.");
  if (hum  > 85) Serial.println("[ALERT] High humidity! Disease risk.");
  if (hum  < 30) Serial.println("[ALERT] Low humidity! Drought stress.");
  Serial.println("------------------------------");

  pushToFirebase(temp, hum, heat);

  delay(READ_INTERVAL_MS);
}
