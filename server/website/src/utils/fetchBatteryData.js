export const fetchBatteryData = async (runId) => {
    try {
        const response = await fetch(`https://rts358y5pk.execute-api.eu-west-2.amazonaws.com/prod/get-battery?RunId=${runId}`);
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        return data.map(item => ({
            x: new Date(item.Timestamp * 1000), // Convert epoch time to JavaScript Date object
            y: item.Battery
        }));
    } catch (error) {
        console.error('Failed to fetch data:', error);
        throw error;
    }
};
