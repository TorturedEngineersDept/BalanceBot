// Fetch map information from JSON file
fetch('/data/map_processed/map.json')
    .then(response => response.json())
    .then(mapInfo => {
        const mapImage = document.getElementById('map');
        mapImage.src = mapInfo.image;

        const mapResolution = mapInfo.resolution;
        const mapOrigin = mapInfo.origin;
        const waypoints = mapInfo.waypoints;

        // Function to convert map coordinates to pixel positions
        function mapToPixel(x, y) {
            const pixelX = (x - mapOrigin[0]) / mapResolution;
            const pixelY = mapImage.height - (y - mapOrigin[1]) / mapResolution; // Invert Y axis
            return { pixelX, pixelY };
        }

        // Function to add waypoints to the map
        function addWaypoints() {
            const mapContainer = document.getElementById('map-container');

            waypoints.forEach((waypoint, index) => {
                const { x, y, image } = waypoint;
                const { pixelX, pixelY } = mapToPixel(x, y);

                const waypointElement = document.createElement('div');
                waypointElement.classList.add('waypoint');
                waypointElement.style.left = `${pixelX}px`;
                waypointElement.style.top = `${pixelY}px`;

                waypointElement.addEventListener('click', () => {
                    const cameraImage = document.getElementById('camera-image');
                    cameraImage.src = image;
                    cameraImage.style.display = 'block';
                });

                mapContainer.appendChild(waypointElement);
            });
        }

        // Run the function after the map image has loaded
        mapImage.onload = addWaypoints;
    })
    .catch(error => console.error('Error loading map information:', error));
