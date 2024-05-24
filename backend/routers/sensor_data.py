from fastapi import APIRouter
from fastapi.responses import JSONResponse
from config.mogo_db import collection
from models.carrodat_models import CarroDataGET

router = APIRouter()

def parse_mongo_document(document):
    return {
        "_id": str(document["_id"]),
        "distance": int(document["distance"]),
        "acceleration": int(document["acceleration"]),
        "hall": float(document["hall"]),
        "temperature": float(document["temperature"]),
    }

@router.get("/", tags=["Sensor Data"], response_model=list[CarroDataGET])
def get_sensordata(distance: int = None, hall: float = None, acceleration: int = None):
    try:
        query = {}
        if distance is not None:
            query["distance"] = distance
        if hall is not None:
            query["hall"] = hall
        if acceleration is not None:
            query["acceleration"] = acceleration
        
        cursor = collection.find(query)
        data = [parse_mongo_document(doc) for doc in cursor]
        return data
    except Exception as e:
        return JSONResponse(status_code=500, content={"message": str(e)})

@router.get("/dist/", tags=["Sensor Data"], response_model=list[CarroDataGET])
def get_sensordata_by_dist(distance: int):
    try:
        cursor = collection.find({"distance": distance})
        data = [parse_mongo_document(doc) for doc in cursor]
        return data
    except Exception as e:
        return JSONResponse(status_code=500, content={"message": str(e)})
    
@router.get("/hall/", tags=["Sensor Data"], response_model=list[CarroDataGET])
def get_sensordata_by_hall(hall: float):
    try:
        cursor = collection.find({"hall": hall})
        data = [parse_mongo_document(doc) for doc in cursor]
        return data
    except Exception as e:
        return JSONResponse(status_code=500, content={"message": str(e)})
    

@router.get("/acc/", tags=["Sensor Data"], response_model=list[CarroDataGET])
def get_sensordata_by_acceleration(acceleration: int):
    try:
        cursor = collection.find({"acceleration": acceleration})
        data = [parse_mongo_document(doc) for doc in cursor]
        return data
    except Exception as e:
        return JSONResponse(status_code=500, content={"message": str(e)})

@router.get("/all/", tags=["Sensor Data"], response_model=list[CarroDataGET])
def get_all_sensordata():
    try:
        cursor = collection.find({})
        data = [parse_mongo_document(doc) for doc in cursor]
        return data
    except Exception as e:
        return JSONResponse(status_code=500, content={"message": str(e)})
