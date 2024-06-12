import React, { useEffect, useContext } from 'react';
import CanvasJSReact from '@canvasjs/react-charts';
import './graph.css';

const CanvasJSChart = CanvasJSReact.CanvasJSChart;

const BatteryGraph = ({ batteryData, setBatteryData }) => {
    useEffect(() => {
        console.log("Battery data updated:", batteryData);
    }, [batteryData]);

    const batteryOptions = {
        title: {
            text: "Battery Usage"
        },
        axisX: {
            title: "Time",
            valueFormatString: "HH:mm:ss"
        },
        axisY: {
            title: "Battery",
            maximum: 100,
            suffix: "%"
        },
        data: [{
            type: "line",
            xValueType: "dateTime",
            dataPoints: batteryData.slice(-20)
        }]
    };

    return (
        <div className="graph-container">
            <CanvasJSChart options={batteryOptions} containerProps={{ height: "100%", width: "100%" }} />
        </div>
    );
};

export default BatteryGraph;
