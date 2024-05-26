import React from 'react';
import batteryImage from '../assets/images/battery.png';

function Battery() {
    return (
        <div className="container">
            <div id="rectangle"></div>
            <img src={batteryImage} alt="Battery Image" width="128" height="69" />
        </div>
    );
}

export default Battery;