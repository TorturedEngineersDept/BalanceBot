import React, { useEffect } from 'react';
import NavigationBar from '../components/NavigationBar';
import Map from '../components/Map';
import ToggleSwitch from '../components/ToggleSwitch';
import Joystick from '../components/Joystick';
import { initializeMQTT } from '../utils/mqtt';
import BotSpecs from '../subsystems/BotSpecs'; import './ControlPage.css';

const ControlPage = () => {
    useEffect(() => {
        initializeMQTT();
    }, []);

    return (
        <div className="ControlPage">
            <div className="HyperLinksBar">
                <NavigationBar />
            </div>
            <div className="ControlPageBody">
                <div className="left">
                    <div className="NavigationControl">
                        <div className="JoystickContainer">
                            <Joystick />
                        </div>
                        <div className="BotSpecs">
                            <BotSpecs />
                        </div>
                    </div>
                    <div className="Camera">CAMERA</div>
                </div>
                <div className="MapContainer">
                    <Map />
                </div>
            </div>
        </div>
    );
};

export default ControlPage;
