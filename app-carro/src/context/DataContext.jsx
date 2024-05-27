import { createContext, useEffect, useState } from "react";

export const DataContext = createContext();

let render_time = 0;

export function DataContextProvider(props) {
    const [sensorData, setSensorData] = useState([]);
    const [device, setDevice] = useState("all");
    const [orderBy, setOrderBy] = useState("date");
    const [lastest1st, setLastest1st] = useState(false);
    const [limit, setLimit] = useState(0);
    const [fetch_flag, setFetch] = useState(false);

    useEffect(() => {
        if (render_time === 0) {
            render_time++;
            return;
        }
        fetchData();
    }, [fetch_flag]);

    async function fetchData() {
        const url = 
            `http://127.0.0.1:8000/carrodata/all`;

        try {
            console.log(`Fetching data from ${url}`);
            const response = await fetch(url);
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            const data = await response.json();
            console.log('Fetched data:', data);
            setSensorData(data);
        } catch (error) {
            console.error('Error fetching data:', error);
            displayMessage(`Error fetching data: ${error.message}`, 'error');
        }
    }

    function displayMessage(message, type) {
        const messageDisplay = document.getElementById('message-display');
        messageDisplay.textContent = message;
        messageDisplay.style.color = type === 'error' ? 'red' : 'black';
    }

    return (
        <DataContext.Provider value={{
            sensorData,
            fetch_flag,
            formValues: {
                device,
                orderBy,
                lastest1st,
                limit
            },
            valueSetters: {
                setDevice,
                setOrderBy,
                setLastest1st,
                setLimit,
                setFetch,
            }
        }}>
            {props.children}
        </DataContext.Provider>
    );
}
