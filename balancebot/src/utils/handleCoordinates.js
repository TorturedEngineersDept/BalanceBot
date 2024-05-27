import Chart from 'chart.js/auto';

let chart;

export function initializeChart() {
    const ctx_map = document.getElementById('map').getContext('2d');
    if (chart) {
        chart.destroy(); // Destroy the existing chart instance
    }
    chart = new Chart(ctx_map, {
        type: 'line',
        data: {
            datasets: [{
                label: 'Position',
                data: [],
                borderColor: 'rgba(75, 192, 192, 1)',
                borderWidth: 1,
                fill: false
            }]
        },
        options: {
            scales: {
                x: {
                    type: 'linear',
                    position: 'bottom',
                    title: {
                        display: true,
                        text: 'X'
                    },
                    min: -2000,
                    max: 2000
                },
                y: {
                    title: {
                        display: true,
                        text: 'Y'
                    },
                    min: -2000,
                    max: 2000
                }
            }
        }
    });
}

export function handleCoordinates(x, y) {
    if (chart) {
        chart.data.datasets[0].data.push({ x, y });
        chart.update('none');
    } else {
        console.error('Chart is not initialized');
    }
}
