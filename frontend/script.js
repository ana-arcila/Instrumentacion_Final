document.addEventListener('DOMContentLoaded', function() {
    fetchAndPlotData();
});

async function fetchAndPlotData() {
    const sensor = document.getElementById('Sensor').value;

    try {
        console.log('Fetching data from http://127.0.0.1:8000/carrodata/all/');
        const response = await fetch('http://127.0.0.1:8000/carrodata/all/');
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        console.log('Fetched data:', data);
        
        const x = unpack(data, 'acceleration');
        const y = unpack(data, 'hall');
        const z = unpack(data, 'temperature');

        console.log('Extracted data - x:', x, 'y:', y, 'z:', z);

        const trace1 = {
            x: x,
            y: y,
            z: z,
            mode: 'markers',
            marker: {
                size: 12,
                line: {
                    color: 'rgba(217, 217, 217, 0.14)',
                    width: 0.5
                },
                opacity: 0.8
            },
            type: 'scatter3d'
        };

        const layout = {
            margin: {
                l: 0,
                r: 0,
                b: 0,
                t: 0
            },
            scene: {
                xaxis: { title: 'Aceleracion' },
                yaxis: { title: 'Hall' },
                zaxis: { title: 'Temperatura' }
            }
        };

        Plotly.newPlot('3d-graph', [trace1], layout);
    } catch (error) {
        console.error('Error fetching or plotting data:', error);
        displayMessage(`Error fetching or plotting data: ${error.message}`, 'error');
    }
}

function unpack(rows, key) {
    return rows.map(function(row) { return row[key]; });
}

function displayMessage(message, type) {
    const messageDisplay = document.getElementById('message-display');
    messageDisplay.textContent = message;
    messageDisplay.style.color = type === 'error' ? 'red' : 'black';
}
