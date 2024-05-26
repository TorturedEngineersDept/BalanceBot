import { Chart } from 'chart.js';

export function initializeMap() {
    const ctx_map = document.getElementById('map').getContext('2d');
    const chart = new Chart(ctx_map, {
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

    window.handleCoordinates = function (x, y) {
        chart.data.datasets[0].data.push({ x: x, y: y });
        chart.update('none');
    };
}

// Export handleCoordinates to be used in mqtt.js
export function handleCoordinates(x, y) {
    window.handleCoordinates(x, y);
}