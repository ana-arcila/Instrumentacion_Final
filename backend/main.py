from fastapi import FastAPI
from config.mogo_db import collection
from fastapi.responses import PlainTextResponse
from routers import sensor_data
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

origins = [
    "*",  # Permitir todas las fuentes
    "http://localhost",  # Permitir localhost
    "http://localhost:5500",  # Permitir localhost en el puerto 5500
    "http://localhost:5173",  # Permitir localhost en el puerto 5500
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

app.include_router(prefix="/carrodata", router=sensor_data.router)