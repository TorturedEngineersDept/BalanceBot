// src/components/Navbar.js
import React, { useState, useContext } from 'react';
import { NavLink } from 'react-router-dom';
import './Navbar.css';
import { handleLoginSubmit as login } from '../utils/loginUtils';
import { GlobalContext } from '../context/GlobalState';
import logo from '../images/logo.png'; // Import the logo

const Navbar = () => {
    const [botId, setBotId] = useState('');
    const { setRunId } = useContext(GlobalContext); // Get the setRunId function from the context

    const handleLogin = async (e) => {
        e.preventDefault();
        const runId = await login(botId);
        if (runId) {
            setRunId(runId); // Save the RunID to the global state
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
            <div className="logo-container">
                <img src={logo} alt="logo" className="navbar-logo" />
            </div>
            <div className="login-container">
                <form onSubmit={handleLogin} className="login-form">
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
