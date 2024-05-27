import React, { useContext, useEffect } from 'react';
import Plot from 'react-plotly.js';
import { DataContext } from '../context/DataContext';

function Graph () {
    const { sensorData } = useContext(DataContext);

    const unpack = (rows, key) => {
        return rows.map(row => row[key]);
    };

    useEffect(() => {
        if (sensorData.length > 0) {
            console.log('Plotting data:', sensorData);
        }
    }, [sensorData]);

    const x = unpack(sensorData, 'acceleration');
    const y = unpack(sensorData, 'hall');
    const z = unpack(sensorData, 'temperature');

    const trace1 = {
        x: x,
        y: y,
        z: z,
        mode: 'markers',
        marker: {
            size: 12,
            line: {
                color: 'rgba(217, 217, 217, 0.14)',
                width: 0.5,
            },
            opacity: 0.8,
        },
        type: 'scatter3d',
    };

    const layout = {
        margin: {
            l: 0,
            r: 0,
            b: 0,
            t: 0,
        },
        scene: {
            xaxis: { title: 'Aceleracion' },
            yaxis: { title: 'Hall' },
            zaxis: { title: 'Temperatura' },
        },
    };

    return (
        <div>
            {sensorData.length > 0 ? (
                <Plot data={[trace1]} layout={layout} />
            ) : (
                <div>No data to display</div>
            )}
        </div>
    );
};

export default Graph;
