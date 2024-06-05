import React, { useState } from 'react';
import { NavLink } from 'react-router-dom';
import './Navbar.css'; // Import the CSS file for styling

const Navbar = () => {
    const [botId, setBotId] = useState('');

    const handleLoginSubmit = async (e) => {
        e.preventDefault();

        const data = { BotId: botId };

        try {
            const response = await fetch('https://rts358y5pk.execute-api.eu-west-2.amazonaws.com/prod/get-runid-ui', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    // You might need to add other headers if your server expects them
                },
                body: JSON.stringify(data),
            });

            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }

            const result = await response.json();
            if (result.RunId) {
                console.log('RunID:', result.RunId);
                // Handle the RunID as needed
            } else {
                console.error('RunID not found in response');
            }
        } catch (error) {
            console.error('Fetch error:', error.message);
        }
    };

    return (
        <nav className="navbar">
            <ul className="nav-links">
                <li>
                    <NavLink to="/" exact activeClassName="active">
                        Home
                    </NavLink>
                </li>
                <li>
                    <NavLink to="/control" activeClassName="active">
                        Control
                    </NavLink>
                </li>
                <li>
                    <NavLink to="/dashboard" activeClassName="active">
                        Dashboard
                    </NavLink>
                </li>
            </ul>
            <div className="login-container">
                <form onSubmit={handleLoginSubmit} className="login-form">
                    <input
                        type="text"
                        placeholder="Bot ID"
                        value={botId}
                        onChange={(e) => setBotId(e.target.value)}
                    />
                    <button type="submit">Login</button>
                </form>
            </div>
        </nav>
    );
};

export default Navbar;
