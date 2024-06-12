// Utility function to convert Unix timestamp to HH:MM:SS
const formatTimestamp = (unixTimestamp) => {
    const date = new Date(unixTimestamp * 1000); // Convert seconds to milliseconds
    return date.toISOString().substr(11, 8); // Extract HH:MM:SS
};

// Function to fetch initial data from API Gateway
export const fetchData = async (runId) => {
    try {
        const response = await fetch(`https://rts358y5pk.execute-api.eu-west-2.amazonaws.com/prod/get-terminal?RunId=${runId}`);
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        return data.map(item => ({
            timestamp: formatTimestamp(item.Timestamp),
            text: item.Message,
            type: item.MessageType
        }));
    } catch (error) {
        console.error('Failed to fetch data:', error);
        throw error;
    }
};