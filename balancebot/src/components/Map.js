import React, { useRef, useEffect } from 'react';
import { initializeChart } from '../utils/handleCoordinates';
import './Map.css';

const Map = () => {
    const canvasRef = useRef(null);

    useEffect(() => {
        if (canvasRef.current) {
            initializeChart();
        }

        // Cleanup function to destroy the chart on component unmount
        return () => {
            if (window.chart) {
                window.chart.destroy();
            }
        };
    }, []);

    return (
        <div className="map-container">
            <canvas ref={canvasRef} id="map" width="958" height="995"></canvas>
        </div>
    );
};

export default Map;
