import React, { useEffect, useContext, useState, useRef, useCallback } from 'react';
import './Control.css';
import TopBar from '../components/TopBar';
import WASDControl from '../components/WASDControl';
import { initializeMQTT, sendCLICommand } from '../utils/mqttServiceControl';
import { GlobalContext } from '../context/GlobalState';
import { fetchData } from '../utils/fetchTerminalCommands';

const Control = () => {
    const { runId, batteryPercentage, setBatteryPercentage } = useContext(GlobalContext);
    const [terminalMessages, setTerminalMessages] = useState([]);
    const [cliInput, setCliInput] = useState('');
    const terminalRef = useRef(null);
    const terminalEndRef = useRef(null);
    const [isManualScroll, setIsManualScroll] = useState(false);
    const [mapInfo, setMapInfo] = useState(null);
    const [incidentImage, setIncidentImage] = useState(null);

    // Handler for incoming debug messages
    const handleDebugMessage = (message) => {
        setTerminalMessages(prevMessages => {
            const messages = [...prevMessages, { ...message, type: 'received' }];
            return messages.length > 20 ? messages.slice(messages.length - 20) : messages;
        });
    };

    // Fetch initial data and initialize MQTT on component mount or runId change
    useEffect(() => {
        if (runId) {
            fetchData(runId)
                .then(initialData => {
                    console.log('Fetched initial data:', initialData);
                    setTerminalMessages(initialData);
                })
                .catch(error => {
                    console.error('Error fetching initial data:', error);
                });
            initializeMQTT(setBatteryPercentage, handleDebugMessage, runId);
        }

        // Fetch map information from JSON file
        fetch('/data/map_processed/map.json')
            .then(response => response.json())
            .then(data => setMapInfo(data))
            .catch(error => console.error('Error loading map information:', error));
    }, [runId, setBatteryPercentage]);

    // Scroll terminal to the latest message if not in manual scroll mode
    useEffect(() => {
        if (!isManualScroll && terminalEndRef.current) {
            terminalEndRef.current.scrollIntoView({ behavior: 'smooth' });
        }
    }, [terminalMessages, isManualScroll]);

    // Handle scroll to determine if the user has manually scrolled
    const handleScroll = () => {
        if (terminalRef.current) {
            const { scrollTop, scrollHeight, clientHeight } = terminalRef.current;
            const isAtBottom = scrollHeight - scrollTop <= clientHeight + 50;
            setIsManualScroll(!isAtBottom);
        }
    };

    // Handle touch move to set manual scroll mode
    const handleTouchMove = () => {
        setIsManualScroll(true);
    };

    // Send CLI command and add the message to the terminal
    const handleSendCLICommand = () => {
        if (cliInput.trim() !== '') {
            const message = { timestamp: new Date().toLocaleTimeString(), text: cliInput, type: 'sent' };
            setTerminalMessages(prevMessages => {
                const messages = [...prevMessages, message];
                return messages.length > 20 ? messages.slice(messages.length - 20) : messages;
            });
            sendCLICommand(cliInput, runId);
            setCliInput('');
            setIsManualScroll(false); // Reset manual scroll state when sending a command
        }
    };

    // Handle Enter key press to send CLI command
    const handleKeyDown = (e) => {
        if (e.key === 'Enter') {
            e.preventDefault();
            handleSendCLICommand();
        }
    };

    // Function to convert map coordinates to pixel positions
    const mapToPixel = (x, y, mapImage, mapResolution, mapOrigin) => {
        const pixelX = (x - mapOrigin[0]) / mapResolution;
        const pixelY = mapImage.height - (y - mapOrigin[1]) / mapResolution; // Invert Y axis
        return { pixelX, pixelY };
    };

    // Memoized function to add waypoints to the map
    const addWaypoints = useCallback((mapImage, mapResolution, mapOrigin, waypoints) => {
        const mapContainer = document.getElementById('map-container');

        waypoints.forEach((waypoint, index) => {
            const { x, y, image } = waypoint;
            const { pixelX, pixelY } = mapToPixel(x, y, mapImage, mapResolution, mapOrigin);

            const waypointElement = document.createElement('div');
            waypointElement.classList.add('waypoint');
            waypointElement.style.left = `${pixelX}px`;
            waypointElement.style.top = `${pixelY}px`;

            waypointElement.addEventListener('click', () => {
                setIncidentImage(image);
            });

            mapContainer.appendChild(waypointElement);
        });
    }, []);

    useEffect(() => {
        if (mapInfo) {
            const mapImageElement = document.getElementById('map');
            mapImageElement.src = '/data/map_processed/map.png';
            mapImageElement.onload = () => {
                addWaypoints(mapImageElement, mapInfo.resolution, mapInfo.origin, mapInfo.waypoints);
            };
        }
    }, [mapInfo, addWaypoints]);

    return (
        <div className="control-container">
            <div className="control-left">
                <div className="camera-feed">
                    <TopBar batteryPercentage={batteryPercentage} />
                    {incidentImage ? (
                        <img id="incident-image" src={incidentImage} alt="Incident view" />
                    ) : (
                        <p>Select an incident to view.</p>
                    )}
                </div>
                <div className="terminal-container">
                    <h2 className="terminal-title">TERMINAL</h2>
                    <div
                        className="terminal"
                        ref={terminalRef}
                        onScroll={handleScroll}
                        onWheel={() => setIsManualScroll(true)}
                        onTouchMove={handleTouchMove}
                    >
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
                    <div id="map-container">
                        <img id="map" src="" alt="Robot navigation map" />
                        {/* Waypoints will be added here */}
                    </div>
                </div>
            </div>
            <WASDControl />
        </div>
    );
};

export default Control;
