from pydantic import BaseModel

class CommonsFilterParams(BaseModel):
    distance: float = None
    hall: float = None
    acceleration: int = None