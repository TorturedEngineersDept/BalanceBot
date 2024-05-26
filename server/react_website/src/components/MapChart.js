import React, { useEffect } from 'react';
import { initializeMap } from '../map';

function MapChart() {
    useEffect(() => {
        initializeMap();
    }, []);

    return <canvas id="map"></canvas>;
}

export default MapChart;