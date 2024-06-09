import { useEffect, useState, useContext } from 'react';
import { SendDirections } from '../utils/mqttServiceControl'
import './WASDControl.css';
import { GlobalContext } from '../context/GlobalState';


const WASDControl = () => {
    const { runId } = useContext(GlobalContext);
    const [keysPressed, setKeysPressed] = useState({});
    const [lastKeyPressed, setLastKeyPressed] = useState('');

    useEffect(() => {
        const handleKeyDown = (event) => {
            setKeysPressed((keysPressed) => ({ ...keysPressed, [event.key]: true }));
        };

        const handleKeyUp = (event) => {
            setKeysPressed((keysPressed) => ({ ...keysPressed, [event.key]: false }));
        };

        window.addEventListener('keydown', handleKeyDown);
        window.addEventListener('keyup', handleKeyUp);

        return () => {
            window.removeEventListener('keydown', handleKeyDown);
            window.removeEventListener('keyup', handleKeyUp);
        };
    }, []);

    useEffect(() => {
        const performAction = () => {
            if (keysPressed['Shift'] && keysPressed['W']) {
                if (lastKeyPressed !== 'W') {
                    setLastKeyPressed('W');
                    SendDirections('W', runId);
                }
            }
            else if (keysPressed['Shift'] && keysPressed['A']) {
                if (lastKeyPressed !== 'A') {
                    setLastKeyPressed('A');
                    SendDirections('A', runId);
                }
            }
            else if (keysPressed['Shift'] && keysPressed['S']) {
                if (lastKeyPressed !== 'S') {
                    setLastKeyPressed('S');
                    SendDirections('S', runId);
                }
            }
            else if (keysPressed['Shift'] && keysPressed['D']) {
                if (lastKeyPressed !== 'D') {
                    setLastKeyPressed('D');
                    SendDirections('D', runId);
                }
            } else {
                if (lastKeyPressed !== '') {
                    setLastKeyPressed('');
                    SendDirections('K', runId);
                }
            }
        };

        performAction();
    }, [keysPressed, lastKeyPressed, runId]);



    return null;
};

export default WASDControl;
