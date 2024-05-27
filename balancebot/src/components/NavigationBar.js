import React from 'react';

const NavigationBar = () => (
    <div className="TabbedNavigation" style={{ left: 68, top: 45, position: 'absolute', justifyContent: 'flex-start', alignItems: 'flex-start', gap: 32, display: 'inline-flex' }}>
        <div className="Navigation" style={{ color: 'black', fontSize: 28, fontFamily: 'Inter', fontWeight: '600', lineHeight: '30.80px', wordWrap: 'break-word' }}>Navigation</div>
        <div className="Dashboard" style={{ color: '#C0C0C0', fontSize: 28, fontFamily: 'Inter', fontWeight: '600', lineHeight: '30.80px', wordWrap: 'break-word' }}>Dashboard</div>
        <div className="AboutMe" style={{ color: '#C0C0C0', fontSize: 28, fontFamily: 'Inter', fontWeight: '600', lineHeight: '30.80px', wordWrap: 'break-word' }}>About Me</div>
    </div>
);

export default NavigationBar;