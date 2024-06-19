import React, { useState, useEffect } from 'react';

const Map = ({ selectedIncidentProp }) => {
    const [mapData, setMapData] = useState(null);
    const [mapImage, setMapImage] = useState(null);
    const [imageDimensions, setImageDimensions] = useState({ width: 0, height: 0 });

    const handleWaypointClick = (incidentImage) => {
        selectedIncidentProp(incidentImage);  // Propagate the selected incident to the parent
    };

    useEffect(() => {
        // Fetch map information from JSON file
        fetch('/data/map_processed/map.json')
            .then(response => response.json())
            .then(data => setMapData(data))
            .catch(error => console.error('Error loading map information:', error));

        // Fetch map image
        fetch('/data/map_processed/map.png')
            .then(response => response.blob())
            .then(images => {
                // Then create a local URL for that image and set it
                let outside = URL.createObjectURL(images);
                setMapImage(outside);
                
                // Create an image object to get the dimensions
                const img = new Image();
                img.src = outside;
                img.onload = () => {
                    setImageDimensions({ width: img.width, height: img.height });
                };
            })
    }, []);

    const convertCoordsToPixels = (x, y, resolution, origin, imgWidth, imgHeight) => {
        const x_pixel = (x - origin[0]) / resolution;
        const y_pixel = imgHeight - (y - origin[1]) / resolution;
        return { x_pixel, y_pixel };
    };

    return (
        <div style={{ position: 'relative' }}>
            {mapImage && <img src={mapImage} alt="Map" style={{ width: '100%' }} />}
            {mapData && imageDimensions.width > 0 && imageDimensions.height > 0 && mapData.waypoints.map((waypoint, index) => {
                const { x_pixel, y_pixel } = convertCoordsToPixels(
                    waypoint.x,
                    waypoint.y,
                    mapData.resolution,
                    mapData.origin,
                    imageDimensions.width,
                    imageDimensions.height
                );
                return (
                    <div
                        key={index}
                        style={{
                            position: 'absolute',
                            left: `${x_pixel}px`,
                            top: `${y_pixel}px`,
                            width: '10px',
                            height: '10px',
                            backgroundColor: 'red',
                            borderRadius: '50%',
                            cursor: 'pointer',
                            transform: 'translate(-50%, -50%)'
                        }}
                        onClick={() => handleWaypointClick(waypoint.image)}  // Propagate the incident image URL to the parent component
                    />
                );
            })}
        </div>
    );
};

export default Map;
