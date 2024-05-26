let canvas, ctx, width, height, radius, x_orig, y_orig;
let coord = { x: 0, y: 0 };
let paint = false;

export function setupController() {
    window.addEventListener('load', initializeCanvas);
}

function initializeCanvas() {
    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');
    resizeCanvas();

    document.addEventListener('mousedown', startDrawing);
    document.addEventListener('mouseup', stopDrawing);
    document.addEventListener('mousemove', draw);

    document.addEventListener('touchstart', startDrawing);
    document.addEventListener('touchend', stopDrawing);
    document.addEventListener('touchcancel', stopDrawing);
    document.addEventListener('touchmove', draw);
    window.addEventListener('resize', resizeCanvas);
}

function resizeCanvas() {
    width = window.innerWidth;
    radius = 100;
    height = radius * 6.5;
    ctx.canvas.width = width;
    ctx.canvas.height = height;
    drawBackground();
    drawJoystick(width / 2, height / 3);
}

function drawBackground() {
    x_orig = width / 2;
    y_orig = height / 3;

    ctx.beginPath();
    ctx.arc(x_orig, y_orig, radius + 20, 0, Math.PI * 2, true);
    ctx.fillStyle = '#ECE5E5';
    ctx.fill();
}

function drawJoystick(x, y) {
    ctx.beginPath();
    ctx.arc(x, y, radius, 0, Math.PI * 2, true);
    ctx.fillStyle = '#F08080';
    ctx.fill();
    ctx.strokeStyle = '#F6ABAB';
    ctx.lineWidth = 8;
    ctx.stroke();
}

function getPosition(event) {
    const mouse_x = event.clientX || (event.touches && event.touches[0].clientX);
    const mouse_y = event.clientY || (event.touches && event.touches[0].clientY);
    coord.x = mouse_x - canvas.offsetLeft;
    coord.y = mouse_y - canvas.offsetTop;
}

function isItInTheCircle() {
    const current_radius = Math.sqrt(Math.pow(coord.x - x_orig, 2) + Math.pow(coord.y - y_orig, 2));
    return radius >= current_radius;
}

function startDrawing(event) {
    paint = true;
    getPosition(event);
    if (isItInTheCircle()) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        drawBackground();
        drawJoystick(coord.x, coord.y);
        draw(event);
    }
}

function stopDrawing() {
    paint = false;
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    drawBackground();
    drawJoystick(width / 2, height / 3);
    resetCoordinates();
}

function draw(event) {
    if (paint) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        drawBackground();
        let angle_in_degrees, x, y;
        const angle = Math.atan2(coord.y - y_orig, coord.x - x_orig);

        if (Math.sign(angle) === -1) {
            angle_in_degrees = Math.round(-angle * 180 / Math.PI);
        } else {
            angle_in_degrees = Math.round(360 - angle * 180 / Math.PI);
        }

        if (isItInTheCircle()) {
            drawJoystick(coord.x, coord.y);
            x = coord.x;
            y = coord.y;
        } else {
            x = radius * Math.cos(angle) + x_orig;
            y = radius * Math.sin(angle) + y_orig;
            drawJoystick(x, y);
        }

        getPosition(event);

        const speed = Math.round(100 * Math.sqrt(Math.pow(x - x_orig, 2) + Math.pow(y - y_orig, 2)) / radius);
        const x_relative = Math.round(x - x_orig);
        const y_relative = Math.round(y - y_orig);

        updateCoordinates(x_relative, y_relative, speed, angle_in_degrees);
        updatePolarCoordinates(speed, -angle);
    }
}

function updateCoordinates(x, y, speed, angle) {
    document.getElementById("x_coordinate").innerText = x;
    document.getElementById("y_coordinate").innerText = y;
    document.getElementById("speed").innerText = speed;
    document.getElementById("angle").innerText = angle;
}

function resetCoordinates() {
    updateCoordinates(0, 0, 0, 0);
    updatePolarCoordinates(0, 0);
}

function updatePolarCoordinates(speed, angle) {
    window.polar_coordinates(speed, angle);
}