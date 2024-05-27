import Chart from 'chart.js/auto';

export function handleCoordinates(x, y) {
    const ctx_map = document.getElementById('map').getContext('2d');
    const chart = new Chart(ctx_map, {
        type: 'line',
        data: {
            datasets: [{
                label: 'Position',
                data: [{ x, y }],
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

    chart.data.datasets[0].data.push({ x, y });
    chart.update('none');
}

// Ensure handleCoordinates is globally accessible
window.handleCoordinates = handleCoordinates;
