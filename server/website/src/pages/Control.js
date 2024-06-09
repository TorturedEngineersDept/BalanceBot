import React, { useEffect, useContext, useState, useRef } from 'react';
import './Control.css';
import TopBar from '../components/TopBar';
import WASDControl from '../components/WASDControl';
import { initializeMQTT, sendCLICommand } from '../utils/mqttServiceControl';
import { GlobalContext } from '../context/GlobalState';

const Control = () => {
    const { runId, batteryPercentage, setBatteryPercentage } = useContext(GlobalContext);
    const [terminalMessages, setTerminalMessages] = useState([]);
    const [cliInput, setCliInput] = useState('');
    const terminalRef = useRef(null);
    const terminalEndRef = useRef(null);
    const [isManualScroll, setIsManualScroll] = useState(false);

    const handleDebugMessage = (message) => {
        setTerminalMessages(prevMessages => {
            const messages = [...prevMessages, { ...message, type: 'received' }];
            return messages.length > 10 ? messages.slice(messages.length - 10) : messages;
        });
    };

    useEffect(() => {
        if (runId) {
            initializeMQTT(setBatteryPercentage, handleDebugMessage, runId);
        }
    }, [runId, setBatteryPercentage]);

    useEffect(() => {
        if (!isManualScroll && terminalEndRef.current) {
            terminalEndRef.current.scrollIntoView({ behavior: 'smooth' });
        }
    }, [terminalMessages, isManualScroll]);

    const handleScroll = () => {
        if (terminalRef.current) {
            const { scrollTop, scrollHeight, clientHeight } = terminalRef.current;
            const isAtBottom = scrollHeight - scrollTop <= clientHeight + 50;
            setIsManualScroll(!isAtBottom);
        }
    };

    const handleSendCLICommand = () => {
        if (cliInput.trim() !== '') {
            const message = { timestamp: new Date().toLocaleTimeString(), text: cliInput, type: 'sent' };
            setTerminalMessages(prevMessages => {
                const messages = [...prevMessages, message];
                return messages.length > 10 ? messages.slice(messages.length - 10) : messages;
            });
            sendCLICommand(cliInput, runId);
            setCliInput('');
            setIsManualScroll(false); // Reset manual scroll state when sending a command
        }
    };

    const handleKeyDown = (e) => {
        if (e.key === 'Enter') {
            e.preventDefault();
            handleSendCLICommand();
        }
    };

    return (
        <div className="control-container">
            <div className="control-left">
                <div className="camera-feed">
                    <TopBar batteryPercentage={batteryPercentage} />
                    <h2>Camera Feed</h2>
                    {/* Empty frame for Camera Feed */}
                </div>
                <div className="terminal-container">
                    <h2 className="terminal-title">TERMINAL</h2>
                    <div className="terminal" ref={terminalRef} onScroll={handleScroll} onWheel={() => setIsManualScroll(true)}>
                        <div className="terminal-messages">
                            {terminalMessages.map((msg, index) => (
                                <div key={index} className={`terminal-message ${msg.type}`}>
                                    <span className="timestamp">{msg.timestamp}</span> - <span className="message-text">{msg.text}</span>
                                </div>
                            ))}
                            <div ref={terminalEndRef} />
                        </div>
                    </div>
                    <div className="cli">
                        <input
                            type="text"
                            value={cliInput}
                            onChange={(e) => setCliInput(e.target.value)}
                            onKeyDown={handleKeyDown}
                            placeholder="Enter command"
                        />
                        <button onClick={handleSendCLICommand}>Send</button>
                    </div>
                </div>
            </div>
            <div className="control-right">
                <div className="map">
                    <h2>Map</h2>
                    {/* Empty frame for Map */}
                </div>
            </div>
            <WASDControl />
        </div>
    );
};

export default Control;
