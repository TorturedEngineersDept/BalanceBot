import React, { useState, useEffect } from 'react';
import './Control.css';
import Joystick from '../components/Joystick'; // Adjust the import path as needed
import TopBar from '../components/TopBar'; // Adjust the import path as needed

const Control = () => {
    const [batteryPercentage, setBatteryPercentage] = useState(0);

    useEffect(() => {
        const interval = setInterval(() => {
            setBatteryPercentage(prev => (prev >= 100 ? 0 : prev + 1));
        }, 50); // Update every 50ms to animate faster

        return () => clearInterval(interval);
    }, []);

    return (
        <div className="control-container">
            <div className="control-left">
                <div className="camera-feed">
                    <TopBar batteryPercentage={batteryPercentage} />
                    <h2>Camera Feed</h2>
                    {/* Empty frame for Camera Feed */}
                </div>
                <div className="joystick">
                    <h2>Joystick</h2>
                    <Joystick />
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
