import { useState } from 'react';
import reactLogo from './assets/react.svg';
import viteLogo from '/vite.svg';
import './App.css';
import Graph from './components/Graph';

function App() {
  const [count, setCount] = useState(0)

  return (
    <>
      <div class="container">
        <label for="Sensor">Escoja un sensor</label>
        <select name="grafico" id="Sensor" onchange="fetchAndPlotData()">
            <option value="">-Escoja un campo-</option>
            <option value="Mag">Magnetico</option>
            <option value="Acc">Gravitacional</option>
            <option value="Temp">Temperatura</option>
        </select>
    </div>
    <div id="3d-graph">
    <Graph/>
    </div>
    </>
  )
}

export default App
