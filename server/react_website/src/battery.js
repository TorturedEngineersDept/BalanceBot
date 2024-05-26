const max_length = 100;

export function updateBattery(percentage) {
    if (percentage < 0 || percentage > 100) {
        console.error('Invalid battery percentage:', percentage);
        return;
    }

    const length = (percentage * max_length) / 100;
    document.getElementById("rectangle").style.width = length + "px";
}