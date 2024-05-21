from fastapi import FastAPI
from config.mogo_db import collection
from fastapi.responses import PlainTextResponse
from routers import sensor_data

app = FastAPI()

app.title = "UdeA - Carro mapeador"
app.version = "1.0.0"

@app.get("/", tags=["Home"])
def home():
    return PlainTextResponse("IOT API Home")

app.include_router(prefix="/carrodata", router=sensor_data.router)