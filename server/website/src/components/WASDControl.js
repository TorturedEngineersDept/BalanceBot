import React, { useEffect, useState } from 'react';
import './WASDControl.css';

const WASDControl = () => {
    const [keysPressed, setKeysPressed] = useState({});

    useEffect(() => {
        const handleKeyDown = (event) => {
            setKeysPressed(keysPressed => ({ ...keysPressed, [event.key]: true }));
        };

        const handleKeyUp = (event) => {
            setKeysPressed(keysPressed => ({ ...keysPressed, [event.key]: false }));
        };

        window.addEventListener('keydown', handleKeyDown);
        window.addEventListener('keyup', handleKeyUp);

        return () => {
            window.removeEventListener('keydown', handleKeyDown);
            window.removeEventListener('keyup', handleKeyUp);
        };
    }, []);

    return (
        <div className="wasd-control">
            <button className={`button-w ${keysPressed['w'] ? 'active' : ''}`}>W</button>
            <div className="button-side">
                <button className={`button-a ${keysPressed['a'] ? 'active' : ''}`}>A</button>
                <button className={`button-d ${keysPressed['d'] ? 'active' : ''}`}>D</button>
            </div>
            <button className={`button-s ${keysPressed['s'] ? 'active' : ''}`}>S</button>
        </div>
    );
};

export default WASDControl;