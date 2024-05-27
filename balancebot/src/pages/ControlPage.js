import React, { useEffect } from 'react';
import NavigationBar from '../components/NavigationBar';
import Map from '../components/Map';
import PrimaryButton from '../components/PrimaryButton';
import Joystick from '../components/Joystick';
import Battery from '../components/Battery';
import { initializeMQTT } from '../utils/mqtt';
import './ControlPage.css';

const ControlPage = () => {
    useEffect(() => {
        initializeMQTT();
    }, []);

    return (
        <div className="ControlPage">
            <div className="left">
                <NavigationBar />
                <div id="rectangle" className="Rectangle12" />
                <PrimaryButton />
                <Battery />
                <div className="JoystickContainer">
                    <Joystick />
                </div>
                <div className="Camera">CAMERA</div>
            </div>
            <div className="MapContainer">
                <Map />
            </div>
        </div>
    );
};

export default ControlPage;
