import React from 'react';
import { createRoot } from 'react-dom/client';
import './index.css';
import ControlPage from './pages/ControlPage';
import reportWebVitals from './reportWebVitals';

const container = document.getElementById('root');
const root = createRoot(container);

root.render(
  <React.StrictMode>
    <ControlPage />
  </React.StrictMode>
);

reportWebVitals();
