## 🚀 QUICK START GUIDE

### ⚙️ Prerequisites
- [ ] PlatformIO installed in VS Code
- [ ] WEMOS D1 Mini Lite board
- [ ] DHT22 sensor
- [ ] USB cable for upload

---

### 🔧 Step 1: Hardware Setup

**Wiring DHT22 to WEMOS D1 Mini Lite:**

```
DHT22 Pin 1 (VCC)   → 3.3V
DHT22 Pin 2 (DATA)  → D1 (GPIO5)
DHT22 Pin 3 (N/C)   → Not connected
DHT22 Pin 4 (GND)   → GND
```

**Recommended:**
- Add 10kΩ pull-up resistor between Pin 2 and VCC

**Wiring Diagram:**
```
DHT22
 │├─ 1 (VCC) ──→ 3.3V
 ├─ 2 (DATA) ──→ D1
 ├─ 3 (N/C)
 └─ 4 (GND) ──→ GND
```

---

### 📥 Step 2: Upload Code

**In VS Code Terminal:**

```bash
# Navigate to project
cd /home/nizar/Documents/PlatformIO/Projects/WEMOS-DHT22

# Build & Upload
pio run -t upload -e d1_mini_lite

# Alternative: Click "Upload" button in PlatformIO sidebar
```

**Expected Output:**
```
Compiling .../src/main.cpp
Linking output/firmware.elf
Building .../firmware.bin
Uploading .../firmware.bin
✓ Upload successful
```

---

### 📊 Step 3: Verify Upload

**Monitor Serial Output:**

```bash
pio device monitor -b 115200

# Press Ctrl+C to exit
```

**Expected Output:**
```
=====================================
  DHT22 Data Logger - WEMOS D1
  dengan 5 Hari History Storage
=====================================
[INFO] DHT22 sensor initialized
[INFO] Setting up WiFi Access Point...
[SUCCESS] Access Point created!
[INFO] SSID: DHT22_Sensor
[INFO] Password: 12345678
[INFO] IP Address: 192.168.4.1
[INFO] DNS Server started
[INFO] Setting up NTP time...
[INFO] Waiting for NTP time sync: ............
[INFO] Current time: Mon Jan 01 12:00:00 2024
[INFO] DataStorage initialized
[INFO] Web server started on port 80
[INFO] Setup complete!
=====================================

[DATA] Temp: 25.50°C | Humidity: 60.30% | Heat Index: 24.85°C
[DATA] Temp: 25.52°C | Humidity: 60.25% | Heat Index: 24.87°C
[STORAGE] Records saved: 288
[STORAGE] Records saved: 576
```

---

### 📱 Step 4: Access Dashboard

**From Smartphone/Laptop:**

1. **Open WiFi Settings**
   - Look for network: `DHT22_Sensor`
   - Password: `12345678`

2. **Connect to Network**
   - Wait for "Connected" status

3. **Open Browser**
   - Any URL will work (auto-redirect via Captive Portal)
   - Example: `http://192.168.4.1` or use Search (Siri/Google)

4. **Dashboard Appears**
   - Real-time sensor readings
   - 3 tabs: Charts, Table, Statistics

---

### 🎯 Step 5: First Data Collection

**Let it Run:**
- First hour: Fills basic data
- First day: Complete 24-hour cycle
- First 5 days: Full storage capacity

**Monitor Progress:**
- Go to Statistics tab
- Watch "Total Records" increase
- Every 24 min = 288 new records

---

### 📈 Step 6: View History

**Charts Tab:**
- Shows last 24 hours trend
- Temperature & Humidity graph
- Auto-refresh every 30 seconds

**Table Tab:**
- Shows recent 50 records
- Time, Temperature, Humidity, Heat Index
- Sortable columns

**Statistics Tab:**
- Storage used/available
- Total records count
- Average temperature & humidity

---

### ⬇️ Step 7: Export Data

**Download CSV File:**

1. Click "⬇️ Export" button
2. File downloaded: `dht22_YYYY-MM-DD.csv`
3. Open in Excel, Sheets, or text editor

**CSV Format:**
```csv
Timestamp,Time,Temperature(C),Humidity(%),HeatIndex(C)
1704067200,2024-01-01 12:00:00,25.50,60.30,24.85
1704067205,2024-01-01 12:00:05,25.45,60.25,24.80
1704067210,2024-01-01 12:00:10,25.48,60.28,24.83
```

---

### 🔧 Configuration (Optional)

**Change WiFi Credentials:**

```cpp
// File: src/main.cpp (Line 9-10)
const char* apSSID = "MY_SENSOR";      // Change this
const char* apPassword = "MyPassword";  // Must be 8+ chars
```

**Change Time Zone:**

```cpp
// File: src/main.cpp (Line 20-21)
// For UTC+7 (Asia/Jakarta):
const long gmtOffset_sec = 7 * 3600;
const int daylightOffset_sec = 0;
```

**Change Read Interval:**

```cpp
// File: src/main.cpp (Line 40-41)
// Change 5000 to 10000 for 10 seconds interval
const unsigned long READ_INTERVAL = 5000;
const unsigned long SAVE_INTERVAL = 5000;
```

After any change: Re-upload using `pio run -t upload`

---

### ❓ Troubleshooting

**Problem: Sensor reading shows "--"**
- Check DHT22 wiring
- Verify pin D1 connection
- Look for "[ERROR]" in serial log

**Problem: No WiFi with name "DHT22_Sensor"**
- Check serial monitor for setup messages
- Press ESP8266 reset button
- Wait 30 seconds then scan again

**Problem: Cannot access dashboard**
- Verify connected to WiFi
- Try different browser
- Clear browser cache
- Restart ESP8266 (hold RST button 1s)

**Problem: Dashboard loads but no data**
- Wait 1-2 minutes for first reading
- Check browser console (F12)
- Verify `/api/sensor` endpoint works

**Problem: Storage showing 0 records**
- Check LittleFS initialization in serial
- Verify sensor readings are taken
- Try manual refresh in browser

---

### 📞 Need Help?

**Check Serial Monitor:**
- Most errors printed here
- Look for `[ERROR]` messages
- Copy error message for debugging

**API Test (Browser):**
- `/api/sensor` - Current reading
- `/api/history?limit=10` - Last 10 records
- `/api/storage` - Storage info
- `/api/stats` - Statistics

**Factory Reset:**
```bash
pio device monitor
# Send: LittleFS.format(); in Arduino IDE/PlatformIO console
# Or delete /data folder via SPIFFS tool
```

---

### 🎉 Success!

Your DHT22 Data Logger is now:
- ✅ Recording sensor data every 5 seconds
- ✅ Storing 5 days of history locally
- ✅ Displaying real-time dashboard
- ✅ Showing charts and statistics
- ✅ Allowing data export

**Next Steps:**
1. Let it collect data for a few days
2. Analyze trends in the CSV
3. Adjust configuration if needed
4. Create backup of important data

---

## 📚 Additional Resources

- **Main Documentation**: See `README.md`
- **Feature List**: See `FEATURES.md`
- **Library Docs**: See `lib/DataStorage/README.md`
- **Source Code**: `src/main.cpp`

---

**Last Updated**: 2024
**Board**: WEMOS D1 Mini Lite  
**Sensor**: DHT22
**Storage**: 5 days @ 5-second intervals
