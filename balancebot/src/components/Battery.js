import React, { useEffect } from 'react';
import batteryImage from '../assets/images/battery.png'; // Update with actual image path
import { updateBattery } from '../utils/updateBattery';
import './Battery.css';

const Battery = () => {
    useEffect(() => {
        // Example of how to call updateBattery
        updateBattery(60); // Set initial battery percentage to 50%
    }, []);

    return (
        <div className="BatteryContainer">
            <div id="rectangle" className="BatteryFilling"></div>
            <img src={batteryImage} alt="Battery" className="BatteryImage" />
        </div>
    );
};

export default Battery;
