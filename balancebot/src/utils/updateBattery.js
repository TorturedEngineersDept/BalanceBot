const max_length = 100;

export function update_battery(percentage) {
    if (percentage < 0 || percentage > 100) {
        console.error('Invalid battery percentage:', percentage);
        return;
    }

    const length = (percentage * max_length) / 100;
    console.log('Calculated rectangle length:', length);
    document.getElementById('rectangle').style.width = length + 'px';
    console.log('Rectangle width set to:', length + 'px');
}

// Ensure update_battery is globally accessible
window.update_battery = update_battery;
