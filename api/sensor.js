/**
 * File ini mensimulasikan pengambilan data dari sensor.
 * Di aplikasi nyata, ganti isi fungsi ini untuk melakukan `fetch` 
 * ke alamat IP ESP32 Anda.
 */
function fetchSensorData() {
    return new Promise(resolve => {
        // Simulasi data dari sensor
        const isRaining = Math.random() < 0.2;
        const temperature = (25 + Math.random() * 8).toFixed(1);
        const humidity = (60 + Math.random() * 30).toFixed(1);
        const pressure = (1000 + Math.random() * 15).toFixed(0);
        const altitude = (44330 * (1.0 - Math.pow(pressure / 1013.25, 0.1903))).toFixed(0);

        // --- Perubahan Utama: Buat data dengan tanggal historis ---
        // Membuat timestamp untuk waktu acak dalam 7 hari terakhir
        const now = new Date();
        const randomPastTime = now.getTime() - Math.random() * 7 * 24 * 60 * 60 * 1000;
        const timestamp = new Date(randomPastTime);

        const data = {
            isRaining,
            temperature,
            humidity,
            pressure,
            altitude,
            timestamp // Sekarang menggunakan timestamp historis acak
        };

        setTimeout(() => {
            resolve(data);
        }, 50); // Dibuat lebih cepat agar riwayat terisi lebih banyak
    });
}
