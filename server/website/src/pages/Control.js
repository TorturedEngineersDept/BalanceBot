import React, { useEffect, useContext } from 'react';
import './Control.css';
import TopBar from '../components/TopBar';
import WASDControl from '../components/WASDControl';
import { initializeMQTT } from '../utils/mqttServiceControl';
import { GlobalContext } from '../context/GlobalState';

const Control = () => {
    const { runId, batteryPercentage, setBatteryPercentage } = useContext(GlobalContext);

    useEffect(() => {
        if (runId) {
            initializeMQTT(setBatteryPercentage, runId, 'esp32/battery');
        }
    }, [runId, setBatteryPercentage]);

    return (
        <div className="control-container">
            <div className="control-left">
                <div className="camera-feed">
                    <TopBar batteryPercentage={batteryPercentage} />
                    <h2>Camera Feed</h2>
                    {/* Empty frame for Camera Feed */}
                </div>
                <div className="joystick">
                    <h2>WASD</h2>
                    <WASDControl />
                </div>
                <div className="cli">
                    <input type="text" placeholder="Text field area" />
                    <button>Send</button>
                </div>
            </div>
            <div className="control-right">
                <div className="map">
                    <h2>Map</h2>
                    {/* Empty frame for Map */}
                </div>
            </div>
        </div>
    );
};

export default Control;
