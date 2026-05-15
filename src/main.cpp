#include <Arduino.h>
#include "DHT.h"
#include <ESP8266WiFi.h>

extern "C" {
  #include "user_interface.h" // Diperlukan untuk wifi_set_sleep_type
}

//#define BLYNK_TEMPLATE_ID "TMPL5nxAG3220"
//#define BLYNK_TEMPLATE_NAME "HeatGuard"

// ============ KONFIGURASI BLYNK ============
//#define BLYNK_TEMPLATE_ID "TMPL5nxAG3220"      // Ganti dengan Template ID dari Blynk
//#define BLYNK_TEMPLATE_NAME "HeatGuard"  // Ganti dengan Template Name dari Blynk
//#define BLYNK_AUTH_TOKEN "hPaw1_wnn_tz2ZEbGxeJLYk_hBGrxJ3h"  // Ganti dengan Auth Token dari Blynk App

#define BLYNK_TEMPLATE_ID "TMPL5eYfW3X-J"
#define BLYNK_TEMPLATE_NAME "HeatGuard 1"
#define BLYNK_AUTH_TOKEN "K3PW5mX6enaSUkwgJxoMFDEJntUz8KC_"

#include <BlynkSimpleEsp8266.h>

char ssid[] = "IDMAKER";         // Ganti dengan nama WiFi Anda
char pass[] = "idmaker2026";     // Ganti dengan password WiFi Anda

// ============ KONFIGURASI DHT22 ============
#define DHTPIN D1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ============ VARIABEL GLOBAL ============
float temperature = 0.0;
float humidity = 0.0;
float heatIndex = 0.0;

// ============ FUNCTION DECLARATIONS ============
void readSensorData();
void sendDataToBlynk();

// ============ SETUP ============
void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n=====================================");
  Serial.println("  DHT22 Blynk IoT - WEMOS D1");
  Serial.println("=====================================");
  
  // Inisialisasi DHT22
  dht.begin();
  Serial.println("[INFO] DHT22 sensor initialized");
  
  // Set WiFi ke Station Mode
  WiFi.mode(WIFI_STA);
  
  // Hubungkan ke WiFi dan Blynk
  Serial.println("[INFO] Connecting to WiFi...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  if (Blynk.connected()) {
    Serial.println("[SUCCESS] Connected to Blynk!");
  } else {
    Serial.println("[ERROR] Failed to connect to Blynk");
  }
  
  // Aktifkan Light Sleep
  wifi_set_sleep_type(LIGHT_SLEEP_T);
  Serial.println("[INFO] Light Sleep enabled");
}

// ============ LOOP ============
void loop() {
  Serial.println("\n[INFO] Publishing cycle started");
  
  // Baca sensor
  Serial.println("[INFO] Reading sensor data...");
  readSensorData();
  
  // Pastikan koneksi Blynk aktif
  Blynk.run(); // Jalankan Blynk untuk maintain koneksi
  delay(100);
  
  if (!Blynk.connected()) {
    Serial.println("[INFO] Reconnecting to Blynk...");
    Blynk.connect();
    delay(1000); // Tunggu koneksi
  }
  
  if (Blynk.connected()) {
    sendDataToBlynk();
    Serial.println("[INFO] Data sent to Blynk successfully");
  } else {
    Serial.println("[ERROR] Failed to connect to Blynk, will retry in 5 minutes");
  }
  
  // Langsung tidur selama 5 menit tanpa delay tambahan di setup
  Serial.println("[INFO] Entering Light Sleep for 5 minutes...");
  Serial.flush(); // Pastikan semua serial data terkirim sebelum sleep
  delay(300000); // Masuk light sleep 5 menit (300 detik)
  
  Serial.println("\n[INFO] Woke up from Light Sleep");
}

// ============ FUNCTION: Read Sensor Data ============
void readSensorData() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("[ERROR] Failed to read from DHT sensor!");
    Serial.println("[INFO] Check sensor connection and wiring");
    temperature = 0.0;
    humidity = 0.0;
  } else {
    heatIndex = dht.computeHeatIndex(temperature, humidity, false);
    
    Serial.print("[DATA] Temp: ");
    Serial.print(temperature, 2);
    Serial.print("°C | Humidity: ");
    Serial.print(humidity, 2);
    Serial.print("% | Heat Index: ");
    Serial.print(heatIndex, 2);
    Serial.println("°C");
  }
}

// ============ FUNCTION: Send Data to Blynk ============
void sendDataToBlynk() {
  // Kirim data ke virtual pins di Blynk
  Blynk.virtualWrite(V0, temperature);  // Suhu ke V0
  Blynk.virtualWrite(V1, humidity);     // Kelembaban ke V1
  Blynk.virtualWrite(V2, heatIndex);    // Heat Index ke V2
  
  Serial.println("[INFO] Data sent to Blynk");
}
