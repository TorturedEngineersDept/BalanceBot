// ToggleSwitch.js
import React, { useState } from 'react';
import './ToggleSwitch.css';

const ToggleSwitch = () => {
    const [isOn, setIsOn] = useState(false);

    const handleToggle = () => {
        setIsOn(!isOn);
        // Handle additional logic here, such as making an API call
        console.log("Autonomous Drive is now", !isOn ? "ON" : "OFF");
    };

    return (
        <label className="toggle-switch">
            <input type="checkbox" checked={isOn} onChange={handleToggle} />
            <span className="slider"></span>
            <span className="label">{isOn ? 'Autonomous Drive ON' : 'Autonomous Drive OFF'}</span>
        </label>
    );
};

export default ToggleSwitch;
