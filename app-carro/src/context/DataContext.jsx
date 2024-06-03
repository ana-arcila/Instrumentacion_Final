import { createContext, useEffect, useState } from "react";

export const DataContext = createContext();

export function DataContextProvider(props) {
    const [sensorData, setSensorData] = useState([]);
    const [device, setDevice] = useState("all");
    const [orderBy, setOrderBy] = useState("date");
    const [latestFirst, setLatestFirst] = useState(false);
    const [limit, setLimit] = useState(0);
    const [fetchFlag, setFetchFlag] = useState(false);

    useEffect(() => {
        const fetchData = async () => {
            try {
                const response = await fetch('http://127.0.0.1:8000/carrodata/all/');
                if (!response.ok) {
                    throw new Error('Failed to fetch sensor data');
                }
                const data = await response.json();
                console.log('Fetched Sensor Data:', data); // Log the fetched data
                setSensorData(data);
            } catch (err) {
                console.error(err);
            }
        };
    
        fetchData();
    }, [fetchFlag]);
    

    const toggleFetchFlag = () => {
        setFetchFlag(prevFlag => !prevFlag);
    };

    return (
        <DataContext.Provider value={{
            sensorData,
            setFetchFlag: toggleFetchFlag,
            device,
            setDevice,
            orderBy,
            setOrderBy,
            latestFirst,
            setLatestFirst,
            limit,
            setLimit
        }}>
            {props.children}
        </DataContext.Provider>
    );
}
