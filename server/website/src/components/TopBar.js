import React, { useState, useEffect } from 'react';
import BatteryIcon from './BatteryIcon'; // Adjust the import path as needed
import './TopBar.css';

const formatTime = (date) => {
    return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
};

const TopBar = ({ batteryPercentage }) => {
    const [time, setTime] = useState(formatTime(new Date()));

    useEffect(() => {
        const timer = setInterval(() => {
            setTime(formatTime(new Date()));
        }, 60000); // Update every minute

        return () => clearInterval(timer);
    }, []);

    return (
        <div className="top-bar">
            <div className="right-items">
                <div className="time-display">{time}</div>
                <div className="battery-display">
                    <div className="battery-percentage">{batteryPercentage}%</div>
                    <BatteryIcon batteryPercentage={batteryPercentage} />
                </div>
            </div>
        </div>
    );
};

export default TopBar;
