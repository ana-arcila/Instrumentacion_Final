from fastapi import FastAPI
from fastapi.responses import PlainTextResponse
from fastapi.middleware.cors import CORSMiddleware
from routers import sensor_data  # Ensure this import is correct
from config.mogo_db import collection
import logging
from fastapi.responses import JSONResponse
import json


app = FastAPI()

origins = [
    "*",  
    "http://localhost",
    "http://localhost:5500",
    "http://localhost:5173",
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.title = "UdeA - Carro mapeador"
app.version = "1.0.0"

@app.get("/", tags=["Home"])
def home():
    return PlainTextResponse("IOT API Home")

# Add this in your main.py or sensor_data.py for testing
@app.get("/test_db_connection")
def test_db_connection():
    try:
        # Fetch a single document to test the connection
        doc = collection.find_one()
        if doc:
            return {"status": "success", "data": parse_mongo_document(doc)}
        else:
            return {"status": "success", "data": "No documents found"}
    except Exception as e:
        return JSONResponse(status_code=500, content={"message": str(e)})


app.include_router(sensor_data.router, prefix="/carrodata")

def parse_mongo_document(document):
    logging.debug("Parsing document: %s", document)
    try:
        return {
            "_id": str(document["_id"]),
            "Angulo": str(document["Angulo"]),
            "Medidas": json.loads(document["Medidas"]),  # Parse Medidas as a list
            "x": str(document["x"]),
            "y": str(document["y"])
        }
    except Exception as e:
        logging.error("Error parsing document: %s", str(e))
        return {}