// Get the length value from mqtt.js and assign it to a variable
const max_length = 100;

function update_battery(percentage) {
    // Validate the percentage value
    if (percentage < 0 || percentage > 100) {
        console.error('Invalid battery percentage:', percentage);
        return;
    }

    // Calculate the length of the rectangle
    var length = (percentage * max_length) / 100;
    console.log('Calculated rectangle length:', length); // Log calculated length

    // Set the width of the rectangle dynamically
    document.getElementById("rectangle").style.width = length + "px";
    console.log('Rectangle width set to:', length + "px"); // Log applied width
}

// Ensure update_battery is globally accessible
window.update_battery = update_battery;
