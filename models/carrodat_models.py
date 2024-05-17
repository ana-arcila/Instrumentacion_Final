from pydantic import BaseModel

class CarroDataGET(BaseModel):
    _id: str
    distance: int
    acceleration: int
    hall: float
    temperature: float
