// src/components/BatteryGraph.js
import React, { useEffect, useState, useContext } from 'react';
import { GlobalContext } from '../context/GlobalState';
import { fetchData } from '../utils/dataService';
import { initializeMQTT } from '../utils/mqttServiceControl';

const BatteryGraph = () => {
    const { runId } = useContext(GlobalContext);
    const [batteryData, setBatteryData] = useState([]);

    useEffect(() => {
        if (runId) {
            fetchData(runId).then(initialData => {
                setBatteryData(initialData);
            }).catch(error => {
                console.error('Error fetching initial data:', error);
            });

            initializeMQTT(setBatteryData);
        }
    }, [runId]);

    return (
        <div>
            {/* Render your graph here using the batteryData state */}
            <h1>Battery Graph</h1>
            {batteryData.map((dataPoint, index) => (
                <div key={index}>
                    Time: {dataPoint.x}, Battery: {dataPoint.y}
                </div>
            ))}
        </div>
    );
};

export default BatteryGraph;
