import React, { useState, useEffect } from 'react';
import './Dashboard.css';
import CanvasJSReact from '@canvasjs/react-charts';
import { fetchData, initializeMQTT } from '../components/BatteryGraph';

const CanvasJSChart = CanvasJSReact.CanvasJSChart;

const Dashboard = () => {
    const [selectedSection, setSelectedSection] = useState('Inner Loop PID');
    const [batteryData, setBatteryData] = useState([]);
    const [powerData, setPowerData] = useState([]);
    const [time, setTime] = useState(new Date().getTime());

    const handleSectionChange = (event) => {
        setSelectedSection(event.target.value);
    };

    // TODO: interface this with real data in the backend.
    useEffect(() => {

        // Fetch battery readings from DynamoDB database
        const runId = '2';
        fetchData(runId)
            .then(initialData => {
                console.log('Fetched initial data:', initialData);
                setBatteryData(initialData);
            })
            .catch(error => {
                console.error('Error fetching initial data:', error);
            });

        // MQTT Client setup
        initializeMQTT(setBatteryData);

        // const interval = setInterval(() => {
        //     setTime(prevTime => prevTime + 1000);
        //     const newPowerData = { x: new Date(time), y: Math.random() * 100 };
        //     setPowerData(prevData => [...prevData, newPowerData].slice(-20));
        // }, 1000);


        return () => {
            // clearInterval(interval);
        };
    }, [time]);

    const batteryOptions = {
        title: {
            text: "Battery Usage"
        },
        data: [{
            type: "line",
            dataPoints: batteryData
        }]
    };

    const powerOptions = {
        title: {
            text: "Power Consumption"
        },
        data: [{
            type: "line",
            dataPoints: powerData
        }]
    };

    return (
        <div className="dashboard-container">
            <div className="dashboard-header">
                <div className="chart">
                    <CanvasJSChart options={batteryOptions} />
                </div>
                <div className="chart">
                    <CanvasJSChart options={powerOptions} />
                </div>
            </div>
            <div className="section">
                <div className="section-left">
                    <h2>Balancing PID Tuning</h2>
                    <select onChange={handleSectionChange} className="dropdown">
                        <option value="Inner Loop PID">Inner Loop PID</option>
                        <option value="Outer Loop PID">Outer Loop PID</option>
                        <option value="Miscellaneous">Miscellaneous</option>
                    </select>

                    {selectedSection === 'Inner Loop PID' && (
                        <>
                            <div className="input-group">
                                <label>Proportional Gain</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                            <div className="input-group">
                                <label>Integral Gain</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                            <div className="input-group">
                                <label>Derivative Gain</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                        </>
                    )}

                    {selectedSection === 'Outer Loop PID' && (
                        <>
                            <div className="input-group">
                                <label>Proportional Gain</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                            <div className="input-group">
                                <label>Integral Gain</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                            <div className="input-group">
                                <label>Derivative Gain</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                        </>
                    )}

                    {selectedSection === 'Miscellaneous' && (
                        <div className="input-group">
                            <label>Setpoint</label>
                            <input type="text" placeholder="Placeholder" />
                        </div>
                    )}

                    <button className="button">Submit</button>
                </div>
                <div className="section-right">
                    <h2>Server Connection</h2>
                    <button className="test-button">Test Server Connectivity</button>
                    <p>Ping: -- ms</p>
                    <p>RTT: -- ms</p>
                </div>
            </div>
        </div>
    );
};

export default Dashboard;
