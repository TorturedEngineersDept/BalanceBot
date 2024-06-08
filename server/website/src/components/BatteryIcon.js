import React from 'react';
import BatteryGauge from 'react-battery-gauge';

const BatteryIcon = ({ batteryPercentage }) => {
    return (
        <BatteryGauge
            value={batteryPercentage}
            orientation="horizontal"
            size={30}
            fill="green"
            customization={{
                batteryBody: {
                    strokeColor: '#fff', // Set the border color to white
                    strokeWidth: 6,
                    cornerRadius: 3,
                    fill: 'none',
                },
                batteryCap: {
                    strokeColor: '#fff', // Set the border color to white
                    strokeWidth: 6,
                    cornerRadius: 3,
                    fill: 'none',
                },
                batteryMeter: {
                    noOfCells: 1, // Use a single cell to remove any internal text
                    fill: 'green',
                    lowBatteryValue: 15,
                    lowBatteryFill: 'red',
                },
                readingText: {
                    lightContrastColor: 'transparent', // Hide the text inside the battery
                    darkContrastColor: 'transparent',  // Hide the text inside the battery
                    lowBatteryColor: 'transparent', // Hide the text inside the battery
                },
            }}
        />
    );
};

export default BatteryIcon;
