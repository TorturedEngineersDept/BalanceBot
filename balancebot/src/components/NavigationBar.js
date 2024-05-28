import React from 'react';
import './NavigationBar.css';

const NavigationBar = () => (
    <div className="TabbedNavigation">
        <div className="Navigation Active">Navigation</div>
        <div className="Dashboard Inactive">Dashboard</div>
        <div className="AboutMe Inactive">About Me</div>
    </div>
);

export default NavigationBar;
