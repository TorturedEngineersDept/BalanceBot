import React from 'react';
import { createRoot } from 'react-dom/client';
import './index.css';
import ControlPage from './pages/ControlPage';
import { DashBoard } from './pages/Dashboard';
import reportWebVitals from './reportWebVitals';

const container = document.getElementById('root');
const root = createRoot(container);

root.render(
  <React.StrictMode>
    <DashBoard />
  </React.StrictMode>
);

reportWebVitals();
