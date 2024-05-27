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
            <NavigationBar />
            <div className="NavigationBody">
                <div className="left">
                    <div className="NavigationControl">
                        <div className="Title">
                            <div className="JoystickBatteryContainer">
                                <div className="JoystickContainer">
                                    <Joystick />
                                </div>
                                <div className="ButtonBatteryContainer">
                                    <PrimaryButton />
                                    <Battery />
                                </div>
                            </div>
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
