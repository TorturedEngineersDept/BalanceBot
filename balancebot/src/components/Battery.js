import React from 'react';
import batteryImage from '../assets/images/battery.png'; // Update with actual image path

const Battery = () => (
    <img src={batteryImage} alt="Battery" className="Battery" style={{ width: 159, height: 75, position: 'absolute', left: 539, top: 281 }} />
);

export default Battery;
