import React, { useEffect, useState, useContext } from 'react';
import CanvasJSReact from '@canvasjs/react-charts';
import { GlobalContext } from '../context/GlobalState';
import { fetchData } from '../utils/fetchBatteryData';
import { initializeMQTT } from '../utils/mqttServiceControl';
import './graph.css';

const CanvasJSChart = CanvasJSReact.CanvasJSChart;

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

    const batteryOptions = {
        title: {
            text: "Battery Usage"
        },
        axisX: {
            title: "Time"
        },
        axisY: {
            title: "Battery",
            maximum: 100 // Set your maximum y-axis value here
        },
        data: [{
            type: "line",
            dataPoints: batteryData
        }]
    };

    return (
        <div className="graph-container">
            <CanvasJSChart options={batteryOptions} containerProps={{ height: "100%", width: "100%" }} />
        </div>
    );
};

export default BatteryGraph;
