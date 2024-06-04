import React, { useEffect } from 'react';
import './WASDControl.css';

const WASDControl = () => {
    useEffect(() => {
        const handleKeyDown = (event) => {
            switch (event.key) {
                case 'w':
                    console.log('Move forward');
                    break;
                case 'a':
                    console.log('Move left');
                    break;
                case 's':
                    console.log('Move backward');
                    break;
                case 'd':
                    console.log('Move right');
                    break;
                default:
                    break;
            }
        };

        window.addEventListener('keydown', handleKeyDown);

        return () => {
            window.removeEventListener('keydown', handleKeyDown);
        };
    }, []);

    return (
        <div className="wasd-control">
            <button className="button-w">W</button>
            <div className="button-side">
                <button className="button-a">A</button>
                <button className="button-d">D</button>
            </div>
            <button className="button-s">S</button>
        </div>
    );
};

export default WASDControl;