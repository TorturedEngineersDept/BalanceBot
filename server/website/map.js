// Initialize Chart.js

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
                min: -2000, // Set the minimum value for the x-axis
                max: 2000   // Set the maximum value for the x-axis
            },
            y: {
                title: {
                    display: true,
                    text: 'Y'
                },
                min: -2000, // Set the minimum value for the x-axis
                max: 2000   // Set the maximum value for the x-axis
            }
        }
    }
});


// ----------------- HANDLE COORDINATES -----------------

function handleCoordinates(x, y) {
    chart.data.datasets[0].data.push({ x: x, y: y });
    chart.update('none');
}

// Ensure handleCoordinates is globally accessible
window.handleCoordinates = handleCoordinates;