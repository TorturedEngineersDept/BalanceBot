import React, { useEffect } from 'react';
import './assets/styles/App.css';
import Coordinates from './components/Coordinates';
import Canvas from './components/Canvas';
import Battery from './components/Battery';
import MapChart from './components/MapChart';
import { setupMQTT } from './mqtt';

function App() {
  useEffect(() => {
    setupMQTT();
  }, []);

  return (
    <div>
      <Coordinates />
      <Canvas />
      <Battery />
      <div id="chartContainer">
        <MapChart />
      </div>
    </div>
  );
}

export default App;