import React, { useState } from 'react';
import './App.css';
import Graph from './components/Graph';

function App() {
    const [selectedAxis, setSelectedAxis] = useState('magnetic_field_magnitude'); // Default to 'mz'

    const handleSensorChange = (event) => {
      const value = event.target.value;
      let axis = 'mx'; // Default to 'mx' if no match
      if (value === 'Mag') {
          axis = 'magnetic_field_magnitude'; // Use the full field name
      } else if (value === 'Temp') {
          axis = 'temperatura';
      } else if (value === 'Alt') {
          axis = 'altura';
      } else if (value === 'Pres') {
          axis = 'presion';
      }
      setSelectedAxis(axis);
  };
  

    return (
        <>
            <div className="container">
                <label htmlFor="Sensor">Choose a sensor field</label>
                <select name="grafico" id="Sensor" onChange={handleSensorChange}>
                    <option value="">-Choose a field-</option>
                    <option value="Mag">Magnetic Field Magnitude</option>
                    <option value="Alt">Altitude</option>
                    <option value="Temp">Temperature</option>
                    <option value="Pres">Pressure</option>
                </select>
            </div>
            <div id="3d-graph">
                <Graph selectedAxis={selectedAxis} />
            </div>
        </>
    );
}

export default App;
