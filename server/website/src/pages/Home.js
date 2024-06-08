import React from 'react';
import { useNavigate } from 'react-router-dom';
import './Home.css';

const Home = () => {
    const navigate = useNavigate();

    const handleContinueClick = () => {
        navigate('/control');
    };

    return (
        <div className="home-container">
            <div className="home-content">
                <h1 className="home-title">Hello. I'm TED</h1>
                <p className="home-paragraph">
                    I am an autonomous robot ready to help you map and navigate your terrain.
                </p>
                <button className="home-button" onClick={handleContinueClick}>Continue</button>
            </div>
        </div>
    );
};

export default Home;
