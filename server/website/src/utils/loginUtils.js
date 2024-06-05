export const handleLoginSubmit = async (botId) => {
    const data = { BotId: botId };

    try {
        const response = await fetch('https://rts358y5pk.execute-api.eu-west-2.amazonaws.com/prod/get-runid-ui', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data),
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const result = await response.json();
        if (result.RunId) {
            console.log('RunID:', result.RunId);
            return result.RunId;
        } else {
            console.error('RunID not found in response');
            return null;
        }
    } catch (error) {
        console.error('Fetch error:', error.message);
        return null;
    }
};
