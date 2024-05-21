from fastapi import Depends
from typing import Annotated

# Función para definir los parámetros comunes de filtrado y ordenamiento
def commons_parameters(sortby: str = "date", lastest1st: bool = True, skip: int = 0, limit: int = 0):
    """
    Define los parámetros comunes para filtrado y ordenamiento de datos.

    Args:
        sortby (str, optional): Campo por el cual ordenar los datos. Por defecto es "date".
        lastest1st (bool, optional): Indica si los datos más recientes deben aparecer primero. Por defecto es True.
        skip (int, optional): Número de documentos a omitir. Por defecto es 0.
        limit (int, optional): Número máximo de documentos a devolver. Por defecto es 0 (sin límite).

    Returns:
        dict: Diccionario con los parámetros comunes.
    """
    return {"sortby": sortby, "lastest1st": lastest1st, "skip": skip, "limit": limit}

# Anotación de dependencia para los parámetros comunes
CommonsFilterParams = Annotated[dict, Depends(commons_parameters)]