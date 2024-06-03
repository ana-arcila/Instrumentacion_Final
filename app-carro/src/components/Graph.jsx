import React, { useContext, useEffect, useState } from 'react';
import Plot from 'react-plotly.js';
import { DataContext } from '../context/DataContext';
import styles from './Graph.module.css';

function Graph({ selectedAxis }) {
    const { sensorData } = useContext(DataContext);

    // State to hold scatter plot data
    const [scatterData, setScatterData] = useState([]);

    // Effect to update scatter plot data when sensorData or selectedAxis changes
    useEffect(() => {
        if (sensorData.length > 0) {
            const newScatterData = {
                x: sensorData.map(row => row.x),
                y: sensorData.map(row => row.y),
                z: sensorData.map(row => row[selectedAxis]),
                mode: 'markers',
                type: 'scatter3d',
                marker: {
                    size: 8,
                    symbol: 'circle',
                    opacity: 0.8,
                },
            };
            setScatterData([newScatterData]);
        }
    }, [sensorData, selectedAxis]);

    const layout = {
        margin: {
            l: 0,
            r: 0,
            b: 0,
            t: 0,
        },
        scene: {
            xaxis: { title: 'X Axis' },
            yaxis: { title: 'Y Axis' },
            zaxis: { title: selectedAxis.charAt(0).toUpperCase() + selectedAxis.slice(1) },
        },
    };

    return (
        <div className={styles.container}>
            <Plot
                data={scatterData}
                layout={layout}
                style={{ width: '250%', height: '500px' }}
            />
        </div>
    );
}

export default Graph;
