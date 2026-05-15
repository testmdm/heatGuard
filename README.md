# 🌡️ heatGuard - Smart Temperature & Humidity Monitor

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Active-green)](https://platformio.org/)
[![ESP8266](https://img.shields.io/badge/ESP8266-WEMOS%20D1%20Mini-blue)](https://www.wemos.cc/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)]()

**heatGuard** adalah sistem monitoring suhu dan kelembaban berbasis IoT yang canggih menggunakan WEMOS D1 Mini, sensor DHT22, dan integrasi cloud Blynk. Ideal untuk monitoring ruangan, greenhouse, gudang, atau aplikasi IoT lainnya.

## 📚 Daftar Isi

- [🎯 Fitur Utama](#-fitur-utama)
- [📦 Hardware yang Dibutuhkan](#-hardware-yang-dibutuhkan)
- [🔧 Panduan Merakit (Assembly Guide)](#-panduan-merakit-assembly-guide)
- [💻 Setup Software](#-setup-software)
- [🚀 Instalasi & Konfigurasi](#-instalasi--konfigurasi)
- [📱 Penggunaan Dashboard](#-penggunaan-dashboard)
- [⚡ Power Management](#-power-management)
- [🐛 Troubleshooting](#-troubleshooting)
- [📄 Lisensi](#-lisensi)

---

## 🎯 Fitur Utama

### ✨ Sensor & Monitoring
- ✅ **Real-time Monitoring**: Pembacaan suhu dan kelembaban secara real-time
- ✅ **Heat Index Calculation**: Menghitung indeks panas otomatis
- ✅ **Akurasi Tinggi**: Sensor DHT22 dengan presisi ±0.5°C dan ±2% RH

### ☁️ Konektivitas Cloud
- ✅ **Blynk Integration**: Sinkronisasi data ke cloud Blynk secara otomatis
- ✅ **Remote Access**: Monitor dari mana saja via aplikasi Blynk
- ✅ **Auto Reconnect**: Koneksi otomatis WiFi dan Blynk setiap bangun

### 💾 Data Storage
- ✅ **Penyimpanan 5 Hari**: Buffer circular menyimpan 86,400 data point
- ✅ **Interval 5 Detik**: Pembacaan data setiap 5 detik
- ✅ **Web Dashboard**: Interface modern di `http://192.168.4.1`

### 📊 Visualization & Export
- ✅ **Interactive Charts**: Grafik trend 24 jam dan perbandingan harian
- ✅ **Data Table**: Tampilan riwayat 50 data terbaru
- ✅ **Export CSV**: Download data untuk analisis lebih lanjut

### ⚡ Efisiensi Energi
- ✅ **Light Sleep Mode**: Konsumsi hanya 1-2mA saat sleep
- ✅ **Battery Optimized**: Rata-rata konsumsi 5-10mA
- ✅ **LiPo Battery Support**: Cocok untuk aplikasi portable

---

## 📦 Hardware yang Dibutuhkan

### Komponen Utama
| No | Komponen | Spesifikasi | Qty | Catatan |
|----|----------|------------|-----|---------|
| 1 | **Microcontroller** | WEMOS D1 Mini Lite (ESP8266) | 1 | Socket headers included |
| 2 | **Sensor Suhu/Kelembaban** | DHT22 (AM2302) | 1 | 0.5°C accuracy |
| 3 | **Resistor** | 4.7kΩ | 1 | Pull-up untuk data line DHT22 |
| 4 | **Kabel Jumper** | Male-to-Male | 6-8 | Sesuai kebutuhan koneksi |
| 5 | **Baterai** | 3.7V LiPo 2000mAh (optional) | 1 | Untuk deep sleep testing |
| 6 | **USB Cable** | Micro USB | 1 | Untuk upload code |
| 7 | **Breadboard** | Mini Breadboard (optional) | 1 | Untuk prototyping |

### Komponen Tambahan (Optional)
- LED status indicator (3mm, 330Ω resistor)
- TP4056 Charging Module (jika menggunakan baterai)
- Relay module (untuk smart control heating/cooling)
- Casing 3D printed atau plastic box

---

## 🔧 Panduan Merakit (Assembly Guide)

### Langkah 1: Persiapan Komponen

```
Periksa semua komponen:
✓ WEMOS D1 Mini Lite terdeteksi baik
✓ DHT22 sensor bersih dan utuh
✓ Kabel jumper 6-8 buah
✓ Resistor 4.7kΩ
✓ USB Cable untuk upload
```

### Langkah 2: Skema Koneksi

```
WEMOS D1 Mini (ESP8266) ←→ DHT22 Sensor
═════════════════════════════════════

Pin Connections:
┌─────────────────────────────────────────┐
│         WEMOS D1 Mini Lite              │
│  ┌─────────────────────────────────┐    │
│  │ [USB]      3V3  GND  D0  D1 ... │    │
│  │             ▲    │   │   ▲      │    │
│  │             │    │   │   │      │    │
│  │             │    │   │   └──────┼────┼──→ DHT22 Data Pin (Pin 2)
│  │             │    │   │          │    │
│  │             │    │   └──────────┼────┼──→ Resistor 4.7kΩ (Pull-up)
│  │             │    │              │    │
│  │             └────┼──────────────┼────┼──→ DHT22 GND (Pin 4)
│  │                  │              │    │
│  │                  └──────────────┼────┼──→ DHT22 GND alternate
│  │                                 │    │
│  │                                 └────┼──→ DHT22 VCC (Pin 1) = 3V3
│  └─────────────────────────────────┘    │
└─────────────────────────────────────────┘

Resistor 4.7kΩ Configuration:
   3V3 ────[4.7kΩ]──── D1 (GPIO5)
                        │
                    DHT22 Data
```

### Langkah 3: Koneksi Detail Pin-by-Pin

**WEMOS D1 Mini → DHT22 Sensor**

| WEMOS Pin | GPIO | Terhubung ke | DHT22 Pin | Deskripsi |
|-----------|------|-------------|-----------|-----------|
| 3V3 | 3V3 | DHT22 VCC | 1 (VCC) | Voltage 3.3V |
| D1 | GPIO5 | DHT22 Data via 4.7kΩ | 2 (Data) | Digital signal |
| GND | GND | DHT22 GND | 4 (GND) | Ground reference |
| - | - | Resistor 4.7kΩ | - | Pull-up resistor |

**Pinout DHT22 (jika menggunakan single row):**
```
[1-VCC] [2-Data] [3-NC] [4-GND]
   ↑        ↑              ↑
  3V3      D1(+4.7kΩ)     GND
```

### Langkah 4: Merakit di Breadboard (Recommended untuk Prototyping)

**Setup di Mini Breadboard:**

```
Breadboard Layout:
┌─────────────────────────────────────────┐
│ DHT22    D1 Mini         WEMOS           │
│  │ │      │ │ │         │ │ │           │
│  │ │      │ │ │         │ │ │           │
│  │ │      └─┼─┼─────────┼─┘ │           │
│ [+]│─────┐ │ │         │   │           │
│  │ [D]───┼─┤ ├─────────┼─┐ │           │
│  │ [G]───┼─┼─┘         │ │ │           │
│  │ │     │ │           │ │ │           │
└─────────────────────────────────────────┘
    │     │   │           │ │ │
    VCC   D1  GND         3V3 D1 GND
```

**Langkah-langkah:**

1. **Pasang WEMOS D1 pada breadboard**
   - Posisikan USB port menghadap ke luar
   - Pastikan semua pin tertanam dengan baik

2. **Pasang DHT22 sensor**
   - Letakkan DHT22 di area terpisah dari microcontroller
   - Orientasi DHT22: VCC ke atas, GND ke bawah

3. **Buat koneksi power**
   - Kabel merah: WEMOS 3V3 → DHT22 VCC (Pin 1)
   - Kabel hitam: WEMOS GND → DHT22 GND (Pin 4)

4. **Pasang resistor pull-up**
   - Satu kaki: WEMOS 3V3
   - Kaki lain: Node yang terhubung ke DHT22 Data (Pin 2)

5. **Koneksi data line**
   - Kabel: WEMOS D1 (GPIO5) → DHT22 Data (Pin 2) via resistor 4.7kΩ

6. **Verifikasi koneksi**
   - Periksa tidak ada short circuit
   - Pastikan semua kabel tertanam dengan kuat
   - Tidak boleh ada kabel yang lepas atau menyentuh pin lain

### Langkah 5: Tes Fisik Awal

```
Checklist Pre-Power:
□ WEMOS terletak stabil
□ DHT22 tidak miring atau lepas
□ Semua kabel terlihat sehat dan tertanam
□ Tidak ada kabel crossing yang tersentuh
□ Resistor 4.7kΩ terpasang pada pull-up
□ USB cable siap untuk upload
```

### Langkah 6: Koneksi USB & Test Power

```
1. Hubungkan USB cable ke WEMOS
2. Periksa LED pada WEMOS menyala
3. Buka Serial Monitor (115200 baud)
4. Pastikan tidak ada error message
5. Amati pembacaan sensor mulai muncul
```

### Langkah 7: Setup Permanent (Opsional)

Jika ingin permanent setup tanpa breadboard:

```
Solder Direct Connections:
- Potong kaki resistor 4.7kΩ tidak terlalu pendek
- Solder resistor langsung: 3V3 to D1 via resistor
- Solder kabel DHT22:
  • VCC ke 3V3 (kabel merah)
  • Data ke junction resistor (kabel kuning/putih)
  • GND ke GND (kabel hitam)
- Gunakan heat shrink atau isolasi tape
- Tahan dengan zip ties atau mount bracket
```

---

## 💻 Setup Software

### Prerequisites

Pastikan Anda sudah install:
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)
- [Git](https://git-scm.com/) (optional)

### 1. Download Proyek

```bash
# Clone repository
git clone https://github.com/yourusername/heatGuard.git
cd heatGuard

# Atau extract ZIP file jika sudah download
```

### 2. Buka di VS Code

```bash
code .
```

Atau:
- Buka VS Code
- File → Open Folder → pilih folder heatGuard

### 3. Install Dependencies

VS Code akan otomatis mendeteksi dan install PlatformIO environment.

Jika tidak otomatis, buka terminal dan jalankan:

```bash
# Install library dependencies
pio lib install
```

---

## 🚀 Instalasi & Konfigurasi

### Langkah 1: Dapatkan Blynk Credentials

#### 1a. Buat Blynk Template

1. Buka [Blynk Console](https://blynk.cloud/)
2. Login atau daftar akun Blynk
3. Klik **"New Template"**
4. Isi form:
   - **Name**: `HeatGuard`
   - **Hardware**: `ESP8266`
   - **Connection Type**: `WiFi`
5. Klik **"Done"**

#### 1b. Buat Virtual Pins

Di Blynk Console → Devices → New Device → pilih template → ambil Auth Token

```
Virtual Pins Setup:
- V0: Temperature (Gauge 0-50°C)
- V1: Humidity (Gauge 0-100%)
- V2: Heat Index (Gauge 0-50°C)
```

#### 1c. Catat Credentials

```
Dari Blynk Console:
BLYNK_TEMPLATE_ID = "TMPL..."        ← Copy dari template
BLYNK_TEMPLATE_NAME = "HeatGuard"
BLYNK_AUTH_TOKEN = "K3PW5m..."       ← Generate dari device
```

### Langkah 2: Konfigurasi Kode

Edit file `src/main.cpp`:

```cpp
// ============ KONFIGURASI BLYNK ============
#define BLYNK_TEMPLATE_ID "TMPL5eYfW3X-J"        // ← Ganti dengan Template ID Anda
#define BLYNK_TEMPLATE_NAME "HeatGuard 1"        // ← Ganti dengan Template Name
#define BLYNK_AUTH_TOKEN "K3PW5mX6enaSUkwgJxoMFDEJntUz8KC_"  // ← Ganti dengan Auth Token

char ssid[] = "IDMAKER";         // ← Ganti dengan nama WiFi Anda
char pass[] = "idmaker2026";     // ← Ganti dengan password WiFi Anda

// ============ KONFIGURASI DHT22 ============
#define DHTPIN D1      // GPIO5 - jangan ubah jika sudah terpasang
#define DHTTYPE DHT22  // Tipe sensor
```

### Langkah 3: Build dan Upload

```bash
# Build project
pio run

# Upload ke WEMOS D1 Mini
pio run --target upload

# Monitor Serial Output
pio device monitor --baud 115200
```

**Expected Output:**

```
=====================================
  DHT22 Blynk IoT - WEMOS D1
=====================================
[INFO] DHT22 sensor initialized
[INFO] Connecting to WiFi...
[SUCCESS] Connected to Blynk!
[INFO] Light Sleep enabled
[INFO] Publishing cycle started
[INFO] Reading sensor data...
[INFO] Temperature: 25.50°C, Humidity: 60.30%, Heat Index: 24.85°C
[INFO] Data sent to Blynk successfully
[INFO] Entering Light Sleep for 5 minutes...
```

---

## 📱 Penggunaan Dashboard

### Setup Blynk Dashboard

1. Buka **Blynk Mobile App**
2. Klik template **HeatGuard**
3. Klik **"+"** untuk tambah widget
4. Tambahkan widgets:

| Widget | Virtual Pin | Display | Min | Max |
|--------|------------|---------|-----|-----|
| Gauge (Temp) | V0 | °C | 0 | 50 |
| Gauge (Humidity) | V1 | % | 0 | 100 |
| Gauge (Heat Index) | V2 | °C | 0 | 50 |
| Value Display | - | Current values | - | - |
| Graph | V0, V1 | Trend 24h | - | - |

### Web Dashboard

Akses via browser lokal:

```
http://192.168.4.1
```

**Features:**
- Real-time reading
- Temperature & Humidity trends
- 24-hour graph
- Daily statistics
- Data export (CSV)

---

## ⚡ Power Management

### Konsumsi Daya

| Mode | Konsumsi | Durasi | Total/Hari |
|------|----------|--------|-----------|
| Active WiFi Connect | ~80mA | 5-10s | ~170 mA |
| Active Blynk Send | ~50mA | 2-3s | ~50 mA |
| Light Sleep (WiFi on) | 1-2mA | 5 min - 5s | ~4.2 A |
| **Average (5 min cycle)** | **~5-10mA** | - | **~115-230mA/day** |

### Durasi Baterai

Dengan baterai 2000mAh:

```
Durasi = 2000 mAh ÷ 10 mA rata-rata
       = 200 jam
       = ~8-9 hari continuous
```

### Optimasi Daya

1. **Naikkan interval pengiriman** (ubah dari 5 menit ke 15 menit)
2. **Gunakan deep sleep** (ganti light sleep)
3. **Matikan WiFi saat tidak perlu** (connect hanya saat kirim)
4. **Reduce sensor polling** (dari 5s menjadi 30s)

---

## 🐛 Troubleshooting

### ❌ Sensor tidak terbaca / Error pembacaan

**Solusi:**
```
1. Periksa koneksi DHT22:
   - Pin VCC terhubung ke 3V3
   - Pin Data terhubung ke D1 via resistor 4.7kΩ
   - Pin GND terhubung ke GND

2. Periksa resistor pull-up:
   - Resistor 4.7kΩ harus terpasang antara 3V3 dan D1
   - Gunakan multimeter untuk memastikan ohm resistance

3. Reset sensor:
   - Putuskan power selama 10 detik
   - Hubungkan kembali
   - Monitor serial untuk output baru

4. Test DHT22:
   - Ganti dengan sensor DHT22 baru
   - Pastikan tidak ada dry joint pada solder
```

### ❌ WiFi tidak connect

**Solusi:**
```
1. Periksa SSID & Password:
   char ssid[] = "YourWiFiName";
   char pass[] = "YourPassword";

2. Pastikan WiFi 2.4GHz (ESP8266 tidak support 5GHz)

3. Check WiFi signal:
   - Pastikan router dalam jarak ~5 meter
   - Tidak ada interference dari microwave/cordless phone

4. Factory reset WiFi:
   - Ganti WiFi credentials
   - Restart WEMOS
   - Monitor serial untuk connection attempts
```

### ❌ Tidak connect ke Blynk

**Solusi:**
```
1. Verifikasi credentials Blynk:
   #define BLYNK_TEMPLATE_ID "TMPL..."
   #define BLYNK_TEMPLATE_NAME "HeatGuard"
   #define BLYNK_AUTH_TOKEN "K3PW5m..."

2. Pastikan template sudah aktif di Blynk Console

3. Check Blynk server status:
   - Ping blynk.cloud dari computer
   - Pastikan port 80/443 tidak terblock firewall

4. Regenerate Auth Token:
   - Buka Blynk Console
   - Device → Settings → Regenerate Token
   - Update di code dan re-upload
```

### ❌ Serial Monitor tidak muncul output

**Solusi:**
```
1. Pastikan baud rate 115200:
   pio device monitor --baud 115200

2. Check USB cable:
   - Gunakan USB cable yang proper (bukan charging only)
   - Test di port USB lain
   - Ganti USB cable

3. Identifikasi port:
   pio device list

4. Koneksi manual ke port:
   pio device monitor --port /dev/ttyUSB0 --baud 115200
```

### ❌ Upload error / tidak terdeteksi

**Solusi:**
```
1. Identifikasi board:
   pio boards | grep d1_mini

2. Ganti baud rate upload:
   # Edit platformio.ini
   upload_speed = 115200  # coba dari 460800

3. Reset WEMOS:
   - Tekan tombol RST di board
   - Atau tahan GND + RST 3 detik

4. Driver USB:
   - Install CH340 driver (jika diperlukan)
   - Download dari: http://www.wch-ic.com/download/ch341soft_LINUX.html
```

### ❌ Light Sleep tidak bekerja

**Solusi:**
```
1. Pastikan power dari baterai (bukan USB):
   - Light sleep hanya aktif dengan power external
   - USB connection akan skip sleep mode

2. Check RST pin:
   - Pastikan RST pin tidak tertarik ke GND
   - RST pin harus floating atau high

3. Verify dalam code:
   wifi_set_sleep_type(LIGHT_SLEEP_T);
   delay(300000); // 5 menit
```

---

## 📊 Spesifikasi Teknis

### WEMOS D1 Mini Lite
- **Chipset**: ESP8266EX
- **RAM**: 160 KB
- **Flash**: 4 MB
- **WiFi**: 802.11 b/g/n @ 2.4GHz
- **GPIO**: 11 pins
- **ADC**: 1x 10-bit
- **I2C/SPI**: Supported
- **Voltage**: 3.3V
- **Current**: 80mA active, 10µA deep sleep

### DHT22 Sensor
- **Temperature Range**: -40 to +80°C
- **Humidity Range**: 0 to 100% RH
- **Accuracy**: ±0.5°C, ±2% RH
- **Response Time**: < 2s
- **Protocol**: Single-wire digital
- **Voltage**: 3.3-5V DC
- **Power**: 1-1.5mA

---

## 📖 File Structure

```
heatGuard/
├── src/
│   └── main.cpp              # Main firmware code
├── lib/
│   └── DataStorage/          # Data storage library
│       ├── DataStorage.h
│       └── README.md
├── include/
│   └── README
├── test/
│   └── README
├── platformio.ini            # PlatformIO config
├── README.md                 # This file
├── FEATURES.md               # Detailed features
├── QUICKSTART.md             # Quick start guide
├── CHANGELOG.md              # Version history
└── IMPLEMENTATION_COMPLETE.md
```

---

## 📝 Lisensi

Proyek ini dilisensikan di bawah **MIT License** - lihat file [LICENSE](LICENSE) untuk detail.

---

## 🤝 Kontribusi

Kami sangat menerima kontribusi! Silakan:

1. Fork repository ini
2. Buat branch fitur (`git checkout -b feature/AmazingFeature`)
3. Commit perubahan (`git commit -m 'Add some AmazingFeature'`)
4. Push ke branch (`git push origin feature/AmazingFeature`)
5. Buka Pull Request

---

## 📞 Support & Dokumentasi

- **Blynk Docs**: https://docs.blynk.io/
- **PlatformIO Docs**: https://docs.platformio.org/
- **DHT22 Datasheet**: https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf
- **ESP8266 Reference**: https://arduino-esp8266.readthedocs.io/

---

## 🎯 Roadmap

- [ ] Support untuk DHT11 dan DHT12
- [ ] Integration dengan Home Assistant
- [ ] Mobile app native (tidak hanya Blynk)
- [ ] SD Card data logging
- [ ] MQTT protocol support
- [ ] Multiple sensor nodes
- [ ] Email/SMS alerts untuk threshold suhu
- [ ] Deep sleep optimization

---

## ⭐ Acknowledge

Terima kasih kepada:
- [Blynk Platform](https://blynk.io/) untuk IoT connectivity
- [Adafruit](https://github.com/adafruit/DHT-sensor-library) untuk DHT library
- [PlatformIO](https://platformio.org/) untuk build system

---

**Made with ❤️ for IoT Enthusiasts**

**Last Updated**: May 2026 | **Version**: 1.0.0
