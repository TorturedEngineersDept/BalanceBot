import React, { useState, useEffect, useContext } from 'react';
import './Dashboard.css';
import CanvasJSReact from '@canvasjs/react-charts';
import { fetchData } from '../utils/fetchBatteryData';
import { initializeMQTT, sendTuning } from '../utils/mqttServiceDashboard';
import { GlobalContext } from '../context/GlobalState';
import BatteryGraph from '../components/BatteryGraph';
import PowerGraph from '../components/PowerGraph';

const CanvasJSChart = CanvasJSReact.CanvasJSChart;

const Dashboard = () => {
    const {
        runId,
        setBatteryPercentage,
        innerKp, setInnerKp,
        innerKd, setInnerKd,
        innerKi, setInnerKi,
        outerKp, setOuterKp,
        outerKd, setOuterKd,
        outerKi, setOuterKi,
        rotationSetpoint, setRotationSetpoint,
        velocitySetpoint, setVelocitySetpoint,
        tiltSetpoint, setTiltSetpoint
    } = useContext(GlobalContext);
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

            initializeMQTT(setBatteryData, runId);
        }
    }, [runId, setBatteryPercentage]);

    useEffect(() => {
        console.log('Battery data updated:', batteryData);
    }, [batteryData]);

    const handleInputChange = (event) => {
        const { name, value } = event.target;
        switch (name) {
            case 'innerKp':
                setInnerKp(value === '' ? innerKp : value);
                break;
            case 'innerKi':
                setInnerKi(value === '' ? innerKi : value);
                break;
            case 'innerKd':
                setInnerKd(value === '' ? innerKd : value);
                break;
            case 'outerKp':
                setOuterKp(value === '' ? outerKp : value);
                break;
            case 'outerKi':
                setOuterKi(value === '' ? outerKi : value);
                break;
            case 'outerKd':
                setOuterKd(value === '' ? outerKd : value);
                break;
            case 'rotationSetpoint':
                setRotationSetpoint(value === '' ? rotationSetpoint : value);
                break;
            case 'velocitySetpoint':
                setVelocitySetpoint(value === '' ? velocitySetpoint : value);
                break;
            case 'tiltSetpoint':
                setTiltSetpoint(value === '' ? tiltSetpoint : value);
                break;
            default:
                break;
        }
    };

    const handleSubmit = (event) => {
        event.preventDefault();
        sendTuning(
            runId,
            parseFloat(innerKp), parseFloat(innerKi), parseFloat(innerKd),
            parseFloat(outerKp), parseFloat(outerKi), parseFloat(outerKd),
            parseFloat(rotationSetpoint), parseFloat(velocitySetpoint), parseFloat(tiltSetpoint)
        );
    };

    return (
        <div className="dashboard-container">
            <div className="dashboard-header">
                <div className="chart">
                    <BatteryGraph batteryData={batteryData} setBatteryData={setBatteryData} />
                </div>
                <div className="chart">
                    <PowerGraph />
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

                    <form onSubmit={handleSubmit}>

                        {selectedSection === 'Inner Loop PID' && (
                            <>
                                <div className="input-group">
                                    <label>Proportional Gain</label>
                                    <input type="number" step="0.0001" name="innerKp" value={innerKp}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                                <div className="input-group">
                                    <label>Integral Gain</label>
                                    <input type="number" step="0.0001" name="innerKi" value={innerKi}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                                <div className="input-group">
                                    <label>Derivative Gain</label>
                                    <input type="number" step="0.0001" name="innerKd" value={innerKd}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                            </>
                        )}

                        {selectedSection === 'Outer Loop PID' && (
                            <>
                                <div className="input-group">
                                    <label>Proportional Gain</label>
                                    <input type="number" step="0.0001" name="outerKp" value={outerKp}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                                <div className="input-group">
                                    <label>Integral Gain</label>
                                    <input type="number" step="0.0001" name="outerKi" value={outerKi}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                                <div className="input-group">
                                    <label>Derivative Gain</label>
                                    <input type="number" step="0.0001" name="outerKd" value={outerKd}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                            </>
                        )}

                        {selectedSection === 'Miscellaneous' && (
                            <>
                                <div className="input-group">
                                    <label>Angle Setpoint</label>
                                    <input type="number" step="0.0001" name="tiltSetpoint" value={tiltSetpoint}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                                <div className="input-group">
                                    <label>Velocity Setpoint</label>
                                    <input type="number" step="0.0001" name="velocitySetpoint" value={velocitySetpoint}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                                <div className="input-group">
                                    <label>Rotation Setpoint</label>
                                    <input type="number" step="0.0001" name="rotationSetpoint" value={rotationSetpoint}
                                        onChange={handleInputChange} placeholder="Placeholder" />
                                </div>
                            </>
                        )}

                        <button type="submit" className="button">Submit</button>
                    </form>
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
