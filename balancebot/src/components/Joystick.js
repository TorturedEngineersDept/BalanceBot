import React, { useEffect, useRef, useState } from 'react';

const Joystick = () => {
    const [paint, setPaint] = useState(false);
    const canvasRef = useRef(null);
    const containerRef = useRef(null);

    useEffect(() => {
        let canvas = canvasRef.current;
        let ctx = canvas.getContext('2d');
        let width, height, radius, x_orig, y_orig;
        let coord = { x: 0, y: 0 };

        function resize() {
            const container = containerRef.current;
            if (container) {
                const rect = container.getBoundingClientRect();
                width = rect.width;
                height = rect.height;
                radius = Math.min(width, height) / 4.5;
                canvas.width = width;
                canvas.height = height;
                background();
                joystick(width / 2, height / 2);
            }
        }

        function background() {
            x_orig = width / 2;
            y_orig = height / 2;

            ctx.beginPath();
            ctx.arc(x_orig, y_orig, radius + 20, 0, Math.PI * 2, true);
            ctx.fillStyle = '#ECE5E5';
            ctx.fill();
        }

        function joystick(x, y) {
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
            if (event.target === canvas) {
                setPaint(true);
                getPosition(event);
                if (isItInTheCircle()) {
                    ctx.clearRect(0, 0, canvas.width, canvas.height);
                    background();
                    joystick(coord.x, coord.y);
                    draw(event);
                }
            }
        }

        function stopDrawing() {
            setPaint(false);
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            background();
            joystick(width / 2, height / 2);
            window.polar_coordinates(0, 0);
        }

        function draw(event) {
            if (!paint) return;
            getPosition(event);
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            background();
            const angle = Math.atan2((coord.y - y_orig), (coord.x - x_orig));
            const angle_in_degrees = Math.sign(angle) === -1 ? Math.round(-angle * 180 / Math.PI) : Math.round(360 - angle * 180 / Math.PI);
            let x, y, speed;

            if (isItInTheCircle()) {
                joystick(coord.x, coord.y);
                x = coord.x;
                y = coord.y;
            } else {
                x = radius * Math.cos(angle) + x_orig;
                y = radius * Math.sin(angle) + y_orig;
                joystick(x, y);
            }

            // Add a threshold to ignore very small movements
            const threshold = 2; // pixels
            const distance_from_center = Math.sqrt(Math.pow(x - x_orig, 2) + Math.pow(y - y_orig, 2));

            if (distance_from_center < threshold) {
                speed = 0;
                x = x_orig;
                y = y_orig;
            } else {
                speed = Math.round(100 * distance_from_center / radius);
            }

            window.polar_coordinates(speed, -angle);
        }

        resize();
        window.addEventListener('resize', resize);
        canvas.addEventListener('mousedown', startDrawing);
        canvas.addEventListener('mouseup', stopDrawing);
        canvas.addEventListener('mousemove', draw);
        canvas.addEventListener('touchstart', startDrawing);
        canvas.addEventListener('touchend', stopDrawing);
        canvas.addEventListener('touchcancel', stopDrawing);
        canvas.addEventListener('touchmove', draw);

        return () => {
            window.removeEventListener('resize', resize);
            canvas.removeEventListener('mousedown', startDrawing);
            canvas.removeEventListener('mouseup', stopDrawing);
            canvas.removeEventListener('mousemove', draw);
            canvas.removeEventListener('touchstart', startDrawing);
            canvas.removeEventListener('touchend', stopDrawing);
            canvas.removeEventListener('touchcancel', stopDrawing);
            canvas.removeEventListener('touchmove', draw);
        };
    }, [paint]);

    return (
        <div ref={containerRef} style={{ position: 'relative', width: '100%', height: '100%', zIndex: 10 }}>
            <canvas ref={canvasRef} style={{ width: '100%', height: '100%' }} />
        </div>
    );
};

export default Joystick;
