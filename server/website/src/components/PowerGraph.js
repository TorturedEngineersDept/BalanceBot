import React, { useEffect, useState, useContext } from 'react';
import CanvasJSReact from '@canvasjs/react-charts';
import { GlobalContext } from '../context/GlobalState';
import { fetchData } from '../utils/fetchBatteryData';
import { initializeMQTT } from '../utils/mqttServiceControl';
import './graph.css';

const CanvasJSChart = CanvasJSReact.CanvasJSChart;

const PowerGraph = () => {
    const { runId } = useContext(GlobalContext);
    const [powerData, setPowerData] = useState([]);

    useEffect(() => {
        if (runId) {
            fetchData(runId).then(initialData => {
                setPowerData(initialData);
            }).catch(error => {
                console.error('Error fetching initial data:', error);
            });

            initializeMQTT(setPowerData);
        }
    }, [runId]);

    const powerOptions = {
        title: {
            text: "Power Consumption"
        },
        axisX: {
            title: "Time"
        },
        axisY: {
            title: "Power"
        },
        data: [{
            type: "line",
            dataPoints: powerData
        }]
    };

    return (
        <div className="graph-container">
            <CanvasJSChart options={powerOptions} containerProps={{ height: "100%", width: "100%" }} />
        </div>
    );
};

export default PowerGraph;
