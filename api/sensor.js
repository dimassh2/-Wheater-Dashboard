/**
 * File ini untuk mengambil data dari ESP32.
 * Ganti URL_ESP32 dengan IP address ESP32 Anda.
 */

// GANTI IP ADDRESS INI DENGAN IP ESP32 ANDA
const ESP32_IP = "192.168.1.100"; // Contoh IP, ganti dengan IP ESP32 Anda
const ESP32_URL = `http://${ESP32_IP}/sensor`;

async function fetchSensorData() {
    try {
        // Coba ambil data dari ESP32
        const response = await fetch(ESP32_URL, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            },
            // Timeout 5 detik
            signal: AbortSignal.timeout(5000)
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const data = await response.json();
        
        // Konversi timestamp ke Date object
        data.timestamp = new Date();
        
        // Pastikan data dalam format yang benar
        data.temperature = parseFloat(data.temperature);
        data.humidity = parseFloat(data.humidity);
        data.pressure = parseFloat(data.pressure);
        data.altitude = parseFloat(data.altitude);
        data.isRaining = Boolean(data.isRaining);

        console.log("Data berhasil diambil dari ESP32:", data);
        return data;

    } catch (error) {
        console.warn("Gagal mengambil data dari ESP32:", error.message);
        console.log("Menggunakan data simulasi...");
        
        // Fallback ke data simulasi jika ESP32 tidak tersedia
        return generateSimulatedData();
    }
}

function generateSimulatedData() {
    // Simulasi data jika ESP32 tidak tersedia
    const isRaining = Math.random() < 0.2;
    const temperature = (25 + Math.random() * 8).toFixed(1);
    const humidity = (60 + Math.random() * 30).toFixed(1);
    const pressure = (1000 + Math.random() * 15).toFixed(0);
    const altitude = (44330 * (1.0 - Math.pow(pressure / 1013.25, 0.1903))).toFixed(0);

    // Buat timestamp acak dalam 7 hari terakhir untuk simulasi
    const now = new Date();
    const randomPastTime = now.getTime() - Math.random() * 7 * 24 * 60 * 60 * 1000;
    const timestamp = new Date(randomPastTime);

    return {
        isRaining,
        temperature: parseFloat(temperature),
        humidity: parseFloat(humidity),
        pressure: parseFloat(pressure),
        altitude: parseFloat(altitude),
        timestamp
    };
}

// Fungsi untuk test koneksi ESP32
async function testESP32Connection() {
    try {
        const response = await fetch(`http://${ESP32_IP}/status`, {
            method: 'GET',
            signal: AbortSignal.timeout(3000)
        });
        
        if (response.ok) {
            const status = await response.json();
            console.log("ESP32 Status:", status);
            return true;
        }
        return false;
    } catch (error) {
        console.log("ESP32 tidak terhubung:", error.message);
        return false;
    }
}

// Test koneksi saat halaman dimuat
document.addEventListener('DOMContentLoaded', async () => {
    console.log(`Mencoba koneksi ke ESP32 di: ${ESP32_URL}`);
    const isConnected = await testESP32Connection();
    
    if (isConnected) {
        console.log("✅ ESP32 terhubung! Menggunakan data real-time.");
    } else {
        console.log("⚠️ ESP32 tidak terhubung. Menggunakan data simulasi.");
        console.log("Pastikan:");
        console.log("1. ESP32 sudah dinyalakan dan terhubung WiFi");
        console.log("2. IP address di sensor.js sudah benar");
        console.log("3. ESP32 dan komputer dalam jaringan yang sama");
    }
});