document.addEventListener('DOMContentLoaded', () => {

    const historyData = [];
    const MAX_HISTORY_POINTS = 200; // Perbanyak agar ada data untuk beberapa hari
    const MAX_REALTIME_POINTS = 15;

    const dom = {
        timestamp: document.getElementById('timestamp'),
        weatherCard: document.getElementById('weather-condition-card'),
        iconContainer: document.getElementById('icon-container'),
        weatherText: document.getElementById('weather-condition-text'),
        tempValue: document.getElementById('temp-value'),
        humidityValue: document.getElementById('humidity-value'),
        pressureValue: document.getElementById('pressure-value'),
        altitudeValue: document.getElementById('altitude-value'),
        historyButton: document.getElementById('history-button'),
        closeModalButton: document.getElementById('close-modal-button'),
        historyModal: document.getElementById('history-modal'),
        historyTableBody: document.getElementById('history-table-body'),
        historyDatePicker: document.getElementById('history-date-picker'),
        filterHistoryButton: document.getElementById('filter-history-button'),
    };

    const icons = {
        sunny: `<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="5"></circle><line x1="12" y1="1" x2="12" y2="3"></line><line x1="12" y1="21" x2="12" y2="23"></line><line x1="4.22" y1="4.22" x2="5.64" y2="5.64"></line><line x1="18.36" y1="18.36" x2="19.78" y2="19.78"></line><line x1="1" y1="12" x2="3" y2="12"></line><line x1="21" y1="12"x2="23" y2="12"></line><line x1="4.22" y1="19.78" x2="5.64" y2="18.36"></line><line x1="18.36" y1="5.64" x2="19.78" y2="4.22"></line></svg>`,
        rainy: `<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M22 12a10.3 10.3 0 00-20 0"></path><path d="M12 3v1m-5 5l-1-1m12 0l1-1m-7 15v1M6 21l-1 1m14 0l1 1"></path><path d="M16 14.5a4.5 4.5 0 00-9 0"></path><path d="M12 12v10"></path></svg>`,
        cloudy: `<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M18 10h-1.26A8 8 0 1 0 9 20h9a5 5 0 0 0 0-10z"></path></svg>`,
        loading: `<svg class="animate-spin" xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24"><circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle><path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path></svg>`
    };
    dom.iconContainer.innerHTML = icons.loading;

    const chartFont = { family: "'Press-Start-2P'", size: 8 };
    const chartOptions = { responsive: true, maintainAspectRatio: false, scales: { x: { ticks: { color: '#fefce8', font: chartFont }, grid: { color: 'rgba(255, 255, 255, 0.2)' } }, y: { ticks: { color: '#fefce8', font: chartFont }, grid: { color: 'rgba(255, 255, 255, 0.2)' } } }, plugins: { legend: { display: true, labels: { color: 'white', font: { ...chartFont, size: 10 } } } } };
    const dualAxisOptions = { ...chartOptions, scales: { x: chartOptions.scales.x, y: { ...chartOptions.scales.y, type: 'linear', position: 'left', ticks: { color: '#d1d5db' }}, y1: { ...chartOptions.scales.y, type: 'linear', position: 'right', grid: { drawOnChartArea: false }, ticks: { color: '#ea580c' }} } };

    const realtimeChart1 = new Chart(document.getElementById('chart-temp-humidity'), { type: 'line', options: chartOptions, data: { datasets: [ { label: 'Suhu', data: [], borderColor: '#dc2626', backgroundColor: '#dc262633', tension: 0.2, fill: true }, { label: 'Lembap', data: [], borderColor: '#2563eb', backgroundColor: '#2563eb33', tension: 0.2, fill: true } ]}});
    const realtimeChart2 = new Chart(document.getElementById('chart-pressure-altitude'), { type: 'line', options: dualAxisOptions, data: { datasets: [ { label: 'Tekanan', data: [], yAxisID: 'y', borderColor: '#d1d5db', backgroundColor: '#d1d5db33', tension: 0.2, fill: true }, { label: 'MDPL', data: [], yAxisID: 'y1', borderColor: '#ea580c', backgroundColor: '#ea580c33', tension: 0.2, fill: true } ]}});
    const historyChart1 = new Chart(document.getElementById('history-chart-1'), { type: 'line', options: chartOptions, data: { datasets: [ { label: 'Suhu', data: [], borderColor: '#dc2626', backgroundColor: '#dc262633' }, { label: 'Lembap', data: [], borderColor: '#2563eb', backgroundColor: '#2563eb33' } ]}});
    const historyChart2 = new Chart(document.getElementById('history-chart-2'), { type: 'line', options: dualAxisOptions, data: { datasets: [ { label: 'Tekanan', data: [], yAxisID: 'y', borderColor: '#d1d5db', backgroundColor: '#d1d5db33' }, { label: 'MDPL', data: [], yAxisID: 'y1', borderColor: '#ea580c', backgroundColor: '#ea580c33' } ]}});
    
    async function mainLoop() {
        try {
            const data = await fetchSensorData();
            
            if (data.isRaining) data.condition = 'HUJAN'; else if (data.temperature > 29) data.condition = 'CERAH'; else data.condition = 'BERAWAN';

            historyData.push(data);
            if (historyData.length > MAX_HISTORY_POINTS) historyData.shift();

            // Hanya update UI dengan data terbaru
            const latestData = historyData.sort((a,b) => b.timestamp - a.timestamp)[0];
            updateUI(latestData);
            updateRealtimeCharts();

        } catch (error) { console.error("Gagal mengambil data sensor:", error); dom.timestamp.textContent = "Koneksi Gagal!"; }
    }
    
    function updateUI(data) {
        dom.timestamp.textContent = `Update: ${data.timestamp.toLocaleString('id-ID')}`;
        dom.tempValue.textContent = data.temperature;
        dom.humidityValue.textContent = data.humidity;
        dom.pressureValue.textContent = data.pressure;
        dom.altitudeValue.textContent = data.altitude;

        dom.weatherText.textContent = data.condition;
        if (data.condition === 'HUJAN') { dom.iconContainer.innerHTML = icons.rainy; dom.weatherCard.style.backgroundColor = '#93c5fd'; } 
        else if (data.condition === 'CERAH') { dom.iconContainer.innerHTML = icons.sunny; dom.weatherCard.style.backgroundColor = '#fde047'; } 
        else { dom.iconContainer.innerHTML = icons.cloudy; dom.weatherCard.style.backgroundColor = '#e5e7eb'; }
    }

    function updateRealtimeCharts() {
        const sortedHistory = [...historyData].sort((a,b) => a.timestamp - b.timestamp);
        const realtimeSlice = sortedHistory.slice(-MAX_REALTIME_POINTS);
        const labels = realtimeSlice.map(d => d.timestamp.toLocaleTimeString('id-ID', { minute: '2-digit', second: '2-digit' }));

        function updateChart(chart, chartLabels, datasets) { chart.data.labels = chartLabels; datasets.forEach((dataset, i) => chart.data.datasets[i].data = dataset); chart.update('none'); }

        updateChart(realtimeChart1, labels, [ realtimeSlice.map(d => d.temperature), realtimeSlice.map(d => d.humidity) ]);
        updateChart(realtimeChart2, labels, [ realtimeSlice.map(d => d.pressure), realtimeSlice.map(d => d.altitude) ]);
    }
    
    // --- FUNGSI RIWAYAT YANG DIPERBARUI ---
    function updateHistoryView(filterDateString) {
        let dataToShow = historyData;

        // Filter data jika tanggal dipilih
        if (filterDateString) {
            dataToShow = historyData.filter(d => d.timestamp.toISOString().startsWith(filterDateString));
        }
        
        // Urutkan data berdasarkan waktu untuk grafik
        dataToShow.sort((a, b) => a.timestamp - b.timestamp);

        // 1. Isi Tabel Riwayat
        dom.historyTableBody.innerHTML = '';
        if (dataToShow.length === 0) {
            dom.historyTableBody.innerHTML = '<tr><td colspan="4" style="text-align:center;">Tidak ada data untuk tanggal ini.</td></tr>';
        } else {
            [...dataToShow].reverse().forEach(data => {
                const row = document.createElement('tr');
                const timeFormat = { day: '2-digit', month: 'short', hour: '2-digit', minute: '2-digit' };
                row.innerHTML = `<td>${data.timestamp.toLocaleDateString('id-ID', timeFormat)}</td><td>${data.condition}</td><td>${data.temperature}°C</td><td>${data.humidity}%</td>`;
                dom.historyTableBody.appendChild(row);
            });
        }
        
        // 2. Update Grafik Riwayat
        const labels = dataToShow.map(d => d.timestamp.toLocaleTimeString('id-ID', { hour: '2-digit', minute: '2-digit' }));
        historyChart1.data.labels = labels;
        historyChart1.data.datasets[0].data = dataToShow.map(d => d.temperature);
        historyChart1.data.datasets[1].data = dataToShow.map(d => d.humidity);
        
        historyChart2.data.labels = labels;
        historyChart2.data.datasets[0].data = dataToShow.map(d => d.pressure);
        historyChart2.data.datasets[1].data = dataToShow.map(d => d.altitude);
        
        historyChart1.update();
        historyChart2.update();
    }

    // --- EVENT LISTENERS ---
    dom.historyButton.addEventListener('click', () => {
        // Saat tombol riwayat utama diklik, tampilkan modal dengan data hari ini
        const today = new Date().toISOString().split('T')[0];
        dom.historyDatePicker.value = today;
        updateHistoryView(today); // Tampilkan data hari ini secara default
        dom.historyModal.classList.remove('hidden');
    });

    dom.filterHistoryButton.addEventListener('click', () => {
        // Saat tombol filter di dalam modal diklik
        const selectedDate = dom.historyDatePicker.value;
        if(selectedDate) {
            updateHistoryView(selectedDate);
        }
    });

    dom.closeModalButton.addEventListener('click', () => dom.historyModal.classList.add('hidden'));
    
    // Jalankan loop utama dengan interval lebih cepat untuk mengisi riwayat
    setInterval(mainLoop, 500);
    // Panggil beberapa kali di awal untuk mengisi data dengan cepat
    for(let i=0; i<50; i++) { mainLoop(); }
});
