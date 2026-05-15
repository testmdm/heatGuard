// DataStorage.h - DHT22 Data Logger Storage Library
// 
// Library untuk manajemen penyimpanan data sensor DHT22 dengan circular buffer
// di LittleFS. Mendukung 5 hari data dengan interval 5 detik.
//
// Usage:
//   DataStorage storage;
//   storage.begin();
//   storage.addRecord(temperature, humidity, heatIndex);
//   String json = storage.getRecordsAsJSON();

#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <Arduino.h>
#include <LittleFS.h>
#include <time.h>

// ============ STRUKTUR DATA ============
struct SensorData {
  uint32_t timestamp;  // Unix timestamp (4 bytes)
  float temperature;   // Suhu dalam °C (4 bytes)
  float humidity;      // Kelembaban dalam % (4 bytes)
  float heatIndex;     // Heat index dalam °C (4 bytes)
  
  // Total: 16 bytes per data point
  // 5 hari * 86400 detik = 432000 data points
  // 432000 * 16 bytes = 6.912 MB (fits dalam LittleFS)
};

class DataStorage {
private:
  const char* dataFile = "/data/sensor_data.bin";
  const char* dataDir = "/data";
  
  // Konfigurasi penyimpanan
  static const uint32_t MAX_DAYS = 5;
  static const uint32_t RECORD_INTERVAL = 5;  // Interval dalam detik
  static const uint32_t RECORDS_PER_DAY = 86400 / RECORD_INTERVAL;  // 17280 records per hari
  static const uint32_t MAX_RECORDS = RECORDS_PER_DAY * MAX_DAYS;    // 86400 records untuk 5 hari
  static const uint32_t MAX_FILE_SIZE = MAX_RECORDS * sizeof(SensorData);
  
  uint32_t currentIndex = 0;  // Index penulisan data berikutnya
  uint32_t totalRecords = 0;  // Total records yang tersimpan
  
public:
  // Constructor
  DataStorage() {}
  
  // Inisialisasi filesystem dan buat direktori jika belum ada
  bool begin() {
    if (!LittleFS.begin()) {
      Serial.println("[ERROR] LittleFS initialization failed!");
      return false;
    }
    
    // Buat direktori data jika belum ada
    if (!LittleFS.exists(dataDir)) {
      if (!LittleFS.mkdir(dataDir)) {
        Serial.println("[ERROR] Failed to create data directory!");
        return false;
      }
    }
    
    // Baca index dari file metadata
    loadMetadata();
    Serial.println("[INFO] DataStorage initialized");
    Serial.print("[INFO] Current records: ");
    Serial.println(totalRecords);
    
    return true;
  }
  
  // Simpan data sensor ke file
  bool addRecord(float temperature, float humidity, float heatIndex) {
    SensorData data;
    data.timestamp = time(nullptr);
    data.temperature = temperature;
    data.humidity = humidity;
    data.heatIndex = heatIndex;
    
    return writeRecord(data);
  }
  
  // Tulis record ke file (dengan circular buffer logic)
  bool writeRecord(const SensorData& data) {
    File file = LittleFS.open(dataFile, "r+b");
    
    // Jika file belum ada, buat file baru
    if (!file) {
      file = LittleFS.open(dataFile, "wb");
      if (!file) {
        Serial.println("[ERROR] Failed to create data file!");
        return false;
      }
    }
    
    // Cari posisi penulisan (circular buffer)
    if (totalRecords < MAX_RECORDS) {
      // File masih belum penuh, tulis di akhir
      file.seek(totalRecords * sizeof(SensorData));
      totalRecords++;
    } else {
      // File sudah penuh, gunakan circular buffer
      file.seek(currentIndex * sizeof(SensorData));
    }
    
    // Tulis data
    size_t written = file.write((uint8_t*)&data, sizeof(SensorData));
    file.close();
    
    if (written != sizeof(SensorData)) {
      Serial.println("[ERROR] Failed to write record!");
      return false;
    }
    
    // Update index untuk record berikutnya
    currentIndex = (currentIndex + 1) % MAX_RECORDS;
    saveMetadata();
    
    return true;
  }
  
  // Baca semua records (untuk dikirim ke web client)
  String getRecordsAsJSON(uint32_t limit = 0) {
    File file = LittleFS.open(dataFile, "rb");
    if (!file || file.size() == 0) {
      return "[]";
    }
    
    // Tentukan berapa records yang akan dibaca
    uint32_t recordsToRead = (limit > 0 && limit < totalRecords) ? limit : totalRecords;
    if (recordsToRead == 0) recordsToRead = totalRecords;
    
    String json = "[";
    SensorData data;
    uint32_t recordsRead = 0;
    
    // Tentukan starting position untuk circular buffer
    uint32_t startIdx = 0;
    if (totalRecords >= MAX_RECORDS) {
      // Jika sudah penuh, mulai dari currentIndex (record tertua)
      startIdx = currentIndex;
    }
    
    // Baca records
    for (uint32_t i = 0; i < totalRecords && recordsRead < recordsToRead; i++) {
      uint32_t idx = (startIdx + i) % MAX_RECORDS;
      file.seek(idx * sizeof(SensorData));
      
      if (file.read((uint8_t*)&data, sizeof(SensorData)) == sizeof(SensorData)) {
        if (recordsRead > 0) json += ",";
        
        json += "{";
        json += "\"timestamp\":" + String(data.timestamp) + ",";
        json += "\"time\":\"" + formatTime(data.timestamp) + "\",";
        json += "\"temperature\":" + String(data.temperature, 2) + ",";
        json += "\"humidity\":" + String(data.humidity, 2) + ",";
        json += "\"heatIndex\":" + String(data.heatIndex, 2);
        json += "}";
        
        recordsRead++;
      }
    }
    
    json += "]";
    file.close();
    
    return json;
  }
  
  // Get records dalam format CSV (untuk export)
  String getRecordsAsCSV(uint32_t limit = 0) {
    File file = LittleFS.open(dataFile, "rb");
    if (!file || file.size() == 0) {
      return "";
    }
    
    uint32_t recordsToRead = (limit > 0 && limit < totalRecords) ? limit : totalRecords;
    if (recordsToRead == 0) recordsToRead = totalRecords;
    
    String csv = "Timestamp,Time,Temperature(C),Humidity(%),HeatIndex(C)\n";
    SensorData data;
    uint32_t recordsRead = 0;
    
    uint32_t startIdx = 0;
    if (totalRecords >= MAX_RECORDS) {
      startIdx = currentIndex;
    }
    
    for (uint32_t i = 0; i < totalRecords && recordsRead < recordsToRead; i++) {
      uint32_t idx = (startIdx + i) % MAX_RECORDS;
      file.seek(idx * sizeof(SensorData));
      
      if (file.read((uint8_t*)&data, sizeof(SensorData)) == sizeof(SensorData)) {
        csv += String(data.timestamp) + ",";
        csv += formatTime(data.timestamp) + ",";
        csv += String(data.temperature, 2) + ",";
        csv += String(data.humidity, 2) + ",";
        csv += String(data.heatIndex, 2) + "\n";
        
        recordsRead++;
      }
    }
    
    file.close();
    return csv;
  }
  
  // Dapatkan info penyimpanan
  void getStorageInfo(uint32_t& total, uint32_t& used, uint32_t& free) {
    FSInfo fsInfo;
    LittleFS.info(fsInfo);
    
    total = fsInfo.totalBytes;
    used = fsInfo.usedBytes;
    free = total - used;
  }
  
  // Dapatkan jumlah records yang tersimpan
  uint32_t getRecordCount() {
    return totalRecords;
  }
  
  // Dapatkan persentase penggunaan
  float getUsagePercent() {
    if (totalRecords == 0) return 0.0;
    return (totalRecords * 100.0) / MAX_RECORDS;
  }
  
  // Clear semua data
  bool clearAllData() {
    if (LittleFS.exists(dataFile)) {
      if (LittleFS.remove(dataFile)) {
        totalRecords = 0;
        currentIndex = 0;
        saveMetadata();
        Serial.println("[INFO] All data cleared!");
        return true;
      }
    }
    return false;
  }
  
private:
  // Format timestamp ke string (YYYY-MM-DD HH:MM:SS)
  String formatTime(uint32_t timestamp) {
    time_t t = timestamp;
    struct tm* timeinfo = localtime(&t);
    
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return String(buffer);
  }
  
  // Simpan metadata (index dan total records)
  void saveMetadata() {
    File file = LittleFS.open("/data/metadata.txt", "w");
    if (file) {
      file.print(currentIndex);
      file.print(",");
      file.print(totalRecords);
      file.close();
    }
  }
  
  // Baca metadata
  void loadMetadata() {
    File file = LittleFS.open("/data/metadata.txt", "r");
    if (file) {
      String line = file.readStringUntil('\n');
      int commaIdx = line.indexOf(',');
      if (commaIdx > 0) {
        currentIndex = line.substring(0, commaIdx).toInt();
        totalRecords = line.substring(commaIdx + 1).toInt();
      }
      file.close();
    }
  }
};

#endif
