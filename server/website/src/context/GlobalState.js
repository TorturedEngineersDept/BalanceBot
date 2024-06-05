// src/context/GlobalState.js
import React, { createContext, useState } from 'react';

export const GlobalContext = createContext();

export const GlobalProvider = ({ children }) => {
    const [runId, setRunId] = useState('');
    const [batteryPercentage, setBatteryPercentage] = useState(0);

    return (
        <GlobalContext.Provider value={{ runId, setRunId, batteryPercentage, setBatteryPercentage }}>
            {children}
        </GlobalContext.Provider>
    );
};
