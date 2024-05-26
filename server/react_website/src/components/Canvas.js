import React, { useEffect } from 'react';
import { setupController } from '../controller';

function Canvas() {
    useEffect(() => {
        setupController();
    }, []);

    return <canvas id="canvas"></canvas>;
}

export default Canvas;