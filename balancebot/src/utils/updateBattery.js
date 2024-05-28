const max_length = 100;

export function updateBattery(percentage) {
    if (percentage < 0 || percentage > 100) {
        console.error('Invalid battery percentage:', percentage);
        return;
    }

    const length = (percentage * max_length) / 100;
    console.log('Calculated rectangle length:', length);

    const rectangle = document.getElementById('rectangle');
    rectangle.style.width = length + 'px';
    console.log('Rectangle width set to:', length + 'px');

    if (percentage >= 50) {
        rectangle.style.backgroundColor = 'green';
    } else if (percentage >= 20) {
        rectangle.style.backgroundColor = 'orange';
    } else {
        rectangle.style.backgroundColor = 'red';
    }
    console.log('Rectangle color set to:', rectangle.style.backgroundColor);
}

// Ensure updateBattery is globally accessible
window.updateBattery = updateBattery;
