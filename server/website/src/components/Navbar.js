import React from 'react';
import { NavLink } from 'react-router-dom';
import './Navbar.css'; // Import the CSS file for styling

const Navbar = () => {
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
        </nav>
    );
};

export default Navbar;
