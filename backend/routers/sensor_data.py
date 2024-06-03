import math
from fastapi import APIRouter, HTTPException
from fastapi.responses import JSONResponse
from bson import ObjectId
from config.mogo_db import collection
from models.carrodat_models import CarroDataGET
import json

router = APIRouter()

def parse_mongo_document(document):
    try:
        # Convert strings to floats for calculation
        mx = float(document["mx"])
        my = float(document["my"])
        mz = float(document["mz"])

        # Calculate the magnitude of the magnetic field
        magnetic_field_magnitude = math.sqrt(mx**2 + my**2 + mz**2)

        # Parse the Medidas field from string to a list of floats if necessary
        if isinstance(document["Medidas"], str):
            medidas_list = json.loads(document["Medidas"])
        else:
            medidas_list = document["Medidas"]

        return {
            "_id": str(document["_id"]),  # Correctly handle ObjectId
            "x": float(document["x"]),
            "y": float(document["y"]),
            "mx": float(document["mx"]),
            "my": float(document["my"]),
            "mz": float(document["mz"]),
            "temperatura": float(document["temperatura"]),
            "presion": float(document["presion"]),
            "altura": float(document["altura"]),
            "Angulo": float(document["Angulo"]),
            "Medidas": [float(m) for m in medidas_list],
            "magnetic_field_magnitude": magnetic_field_magnitude
        }
    except KeyError as e:
        raise ValueError(f"Missing field in document: {e}")
    except ValueError as e:
        raise ValueError(f"Invalid value in document: {e}")

@router.get("/all/", tags=["Sensor Data"], response_model=list[CarroDataGET])
def get_all_shareddata():
    try:
        cursor = collection.find({})
        data = [parse_mongo_document(doc) for doc in cursor]
        return data
    except Exception as e:
        return JSONResponse(status_code=500, content={"message": str(e)})
