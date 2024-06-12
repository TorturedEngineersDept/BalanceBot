import React, { useEffect, useContext } from 'react';
import CanvasJSReact from '@canvasjs/react-charts';
import './graph.css';

const CanvasJSChart = CanvasJSReact.CanvasJSChart;

const PowerGraph = ({ powerData, setPowerData }) => {
    useEffect(() => {
        console.log("Power data updated:", powerData);
    }, [powerData]);

    const powerOptions = {
        title: {
            text: "Power Usage"
        },
        axisX: {
            title: "Time",
            valueFormatString: "HH:mm:ss"
        },
        axisY: {
            title: "Power",
            suffix: "W"
        },
        data: [{
            type: "line",
            xValueType: "dateTime",
            dataPoints: powerData.slice(-20)
        }]
    };

    return (
        <div className="graph-container">
            <CanvasJSChart options={powerOptions} containerProps={{ height: "100%", width: "100%" }} />
        </div>
    );
};

export default PowerGraph;
