// src/context/GlobalState.js
import React, { createContext, useState } from 'react';

export const GlobalContext = createContext();

export const GlobalProvider = ({ children }) => {
    const [runId, setRunId] = useState('');
    const [batteryPercentage, setBatteryPercentage] = useState(0);
    const [innerKp, setInnerKp] = useState('');
    const [innerKi, setInnerKi] = useState('');
    const [innerKd, setInnerKd] = useState('');
    const [outerKp, setOuterKp] = useState('');
    const [outerKi, setOuterKi] = useState('');
    const [outerKd, setOuterKd] = useState('');
    const [compCoeff, setCompCoeff] = useState('');
    const [velocitySetpoint, setVelocitySetpoint] = useState('');
    const [tiltSetpoint, setTiltSetpoint] = useState('');

    return (
        <GlobalContext.Provider value={{
            runId, setRunId,
            batteryPercentage, setBatteryPercentage,
            innerKp, setInnerKp,
            innerKi, setInnerKi,
            innerKd, setInnerKd,
            outerKp, setOuterKp,
            outerKi, setOuterKi,
            outerKd, setOuterKd,
            compCoeff, setCompCoeff,
            velocitySetpoint, setVelocitySetpoint,
            tiltSetpoint, setTiltSetpoint
        }}>
            {children}
        </GlobalContext.Provider>
    );
};
