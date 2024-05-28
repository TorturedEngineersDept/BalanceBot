import React from "react";
import "./Dashboard.css";
import NavigationBar from "../components/NavigationBar";

const DashBoard = () => {
    return (
        <div className="dash-board">
            <div className="div">
                <NavigationBar />
                <div className="line-cart">
                    <div className="text-wrapper-3">Battery Usage</div>
                    <div className="overlap-group">
                        <img className="lines" alt="Lines" src="https://c.animaapp.com/m7Msw9ve/img/lines.svg" />
                        <div className="y-axis">
                            <div className="text-wrapper-4">$50,000</div>
                            <div className="text-wrapper-5">$45,000</div>
                            <div className="text-wrapper-5">$40,000</div>
                            <div className="text-wrapper-5">$35,000</div>
                            <div className="text-wrapper-5">$30,000</div>
                            <div className="text-wrapper-5">$25,000</div>
                        </div>
                        <img className="graph" alt="Graph" src="https://c.animaapp.com/m7Msw9ve/img/graph.svg" />
                        <div className="dot" />
                        <div className="glow" />
                    </div>
                    <div className="x-axis">
                        <div className="frame">
                            <div className="text-wrapper-6">23 Nov</div>
                        </div>
                        <div className="text-wrapper-7">24</div>
                        <div className="text-wrapper-7">25</div>
                        <div className="text-wrapper-7">26</div>
                        <div className="text-wrapper-7">27</div>
                        <div className="text-wrapper-7">28</div>
                        <div className="text-wrapper-7">29</div>
                        <div className="text-wrapper-7">30</div>
                    </div>
                </div>
                <div className="text-wrapper-8">Balancing PID Tuning</div>
                <div className="overlap">
                    <div className="text-wrapper-9">Server Connection</div>
                    <div className="overlap-2">
                        <img className="line" alt="Line" src="https://c.animaapp.com/m7Msw9ve/img/line-3.svg" />
                        <div className="flexcontainer">
                            <p className="text">
                                <span className="span">
                                    Ping: -- ms
                                    <br />
                                </span>
                            </p>
                            <p className="text">
                                <span className="span">RTT: -- ms</span>
                            </p>
                        </div>
                    </div>
                    <div className="element-button-primary">
                        <div className="text-wrapper-10">Test Server Connectivity</div>
                    </div>
                </div>
                <div className="element-input-field-with">
                    <div className="text-wrapper-11">Proportional Gain</div>
                    <input className="field" placeholder="Placeholder" type="text" />
                </div>
                <div className="element-input-field-with-2">
                    <div className="text-wrapper-11">Derivative Gain</div>
                    <input className="field" placeholder="Placeholder" type="text" />
                </div>
                <div className="element-input-field-with-3">
                    <div className="text-wrapper-11">Integral Gain</div>
                    <input className="field" placeholder="Placeholder" type="text" />
                </div>
                <button className="element-button">
                    <div className="text-wrapper-12">Submit</div>
                </button>
                <div className="line-wrapper">
                    <img className="img" alt="Line" src="https://c.animaapp.com/m7Msw9ve/img/line-2.svg" />
                </div>
            </div>
        </div>
    );
};

export default DashBoard;
