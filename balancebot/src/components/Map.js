import React, { useEffect, useRef } from 'react';
import { Chart } from 'chart.js/auto';

const Map = () => {
    const chartRef = useRef(null);
    const canvasRef = useRef(null);

    useEffect(() => {
        const resize = () => {
            const container = document.querySelector('.MapContainer');
            if (container) {
                const rect = container.getBoundingClientRect();
                const canvas = canvasRef.current;
                canvas.width = rect.width;
                canvas.height = rect.height;
            }
        };

        const ctx_map = canvasRef.current.getContext('2d');

        // Destroy previous chart instance if it exists
        if (chartRef.current) {
            chartRef.current.destroy();
        }

        chartRef.current = new Chart(ctx_map, {
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
            chartRef.current.data.datasets[0].data.push({ x, y });
            chartRef.current.update('none');
        };

        resize();
        window.addEventListener('resize', resize);

        return () => {
            // Cleanup chart instance and event listener on component unmount
            if (chartRef.current) {
                chartRef.current.destroy();
            }
            window.removeEventListener('resize', resize);
        };
    }, []);

    return <canvas ref={canvasRef} style={{ width: '100%', height: '100%' }} />;
};

export default Map;
