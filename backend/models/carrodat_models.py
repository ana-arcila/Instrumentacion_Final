from typing import List
from pydantic import BaseModel

class CarroDataGET(BaseModel):
    _id: str
    x: float
    y: float
    mx: float
    my: float
    mz: float
    temperatura: float
    presion: float
    altura: float
    Angulo: float
    Medidas: List[float]  # This remains a string
    magnetic_field_magnitude: float  # New field for magnetic field magnitude
