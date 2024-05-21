from pymongo import ASCENDING, DESCENDING

def filter_db_ordered(db_cursor, sortby="date", lastest1st=True, skip=0, limit=0):
    """
    Aplica ordenamiento y filtrado a un cursor de base de datos MongoDB.

    Args:
        db_cursor: Cursor de la base de datos MongoDB.
        sortby (str, optional): Campo por el cual ordenar los datos. Por defecto es "date".
        lastest1st (bool, optional): Indica si los datos más recientes deben aparecer primero. Por defecto es True.
        skip (int, optional): Número de documentos a omitir. Por defecto es 0.
        limit (int, optional): Número máximo de documentos a devolver. Por defecto es 0 (sin límite).

    Returns:
        Cursor: Cursor de la base de datos MongoDB con ordenamiento y filtrado aplicado.
    """
    # Aplica ordenamiento por el campo especificado
    if sortby in ["temperature", "humidity", "date"]:
        db_cursor = db_cursor.sort(sortby, DESCENDING if lastest1st else ASCENDING)
    
    # Omite los primeros "skip" documentos si es necesario
    if skip:
        db_cursor = db_cursor.skip(skip)
    
    # Limita la cantidad de documentos devueltos si es necesario
    if limit:
        db_cursor = db_cursor.limit(limit)

    return db_cursor