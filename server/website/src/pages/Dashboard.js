import React, { useState, useEffect, useContext } from 'react';
import './Dashboard.css';
import CanvasJSReact from '@canvasjs/react-charts';
import { fetchData } from '../utils/dataService';
import { initializeMQTT } from '../utils/mqttServiceDashboard';
import { GlobalContext } from '../context/GlobalState';

const CanvasJSChart = CanvasJSReact.CanvasJSChart;

const Dashboard = () => {
    const { runId, setBatteryPercentage } = useContext(GlobalContext);
    const [selectedSection, setSelectedSection] = useState('Inner Loop PID');
    const [batteryData, setBatteryData] = useState([]);
    const [powerData, setPowerData] = useState([]);

    const handleSectionChange = (event) => {
        setSelectedSection(event.target.value);
    };

    useEffect(() => {
        if (runId) {
            fetchData(runId)
                .then(initialData => {
                    console.log('Fetched initial data:', initialData);
                    setBatteryData(initialData);
                })
                .catch(error => {
                    console.error('Error fetching initial data:', error);
                });

            initializeMQTT(setBatteryPercentage, setBatteryData, runId, 'esp32/battery/dashboard');
        }
    }, [runId, setBatteryPercentage]);

    useEffect(() => {
        console.log('Battery data updated:', batteryData);
    }, [batteryData]);

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
                        <>
                            <div className="input-group">
                                <label>Angle Setpoint</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                            <div className="input-group">
                                <label>Velocity Setpoint</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                            <div className="input-group">
                                <label>Complementary Filter Coefficient</label>
                                <input type="text" placeholder="Placeholder" />
                            </div>
                        </>
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
