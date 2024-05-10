from fastapi import FastAPI
from typing import Optional, List
from pymongo import MongoClient
from pymongo.server_api import ServerApi
from dotenv import load_dotenv
from pydantic import BaseModel
import random
import os
import json  # Importing json module

load_dotenv()
uri = os.getenv("MONGO_URI")

client = MongoClient(uri, server_api=ServerApi('1'))
db = client['IOT-database']
collection = db["pruebas"]

try:
    client.admin.command('ping')
    print("Pinged your deployment. You successfully connected to MongoDB!")
except Exception as e:
    print(e)

app = FastAPI()

class CarroData(BaseModel):
    _id: Optional[str]
    distance: float
    acceleration: List[int]
    gyroscope: List[float]  # Assuming gyroscope data contains floats
    temperature: float
    hall: float


def main():
    try:
        dist = 800
        while dist > 50:
            # Generating dummy data for demonstration purposes
            acceleration_data = [random.randint(0, 100) for _ in range(3)]
            gyroscope_data = [random.uniform(-1, 1) for _ in range(3)]
            temperature_data = random.uniform(15, 40)
            hall_data = random.uniform(0, 5)

            # Creating an instance of CarroData with dummy data
            carro_data = {
                'distance': dist,
                'acceleration': acceleration_data,
                'gyroscope': gyroscope_data,
                'temperature': temperature_data,
                'hall': hall_data
            }

            # Printing JSON data for demonstration (you can save it to a file, send it to an API endpoint, etc.)
            
            result = collection.insert_one(carro_data)

            if result.inserted_id:
                print("Documento insertado exitosamente. ID:", result.inserted_id)
            else:
                print("Error al insertar el documento.")

            # Adjusting distance for the sake of the example
            dist -= 50

    except Exception as e:
        print(e)

    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    main()
