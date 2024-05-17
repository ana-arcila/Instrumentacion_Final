from pymongo.mongo_client import MongoClient
from pymongo.server_api import ServerApi
from json import loads


class MyMongoClient:

    def __init__(self, uri, database, collection):
        self.uri = uri
        self.database = database
        self.collection = collection
        self.client = None
        self.db = None
        self.col = None

    def connect(self):
        try:
            self.client = MongoClient(self.uri, server_api=ServerApi('1'))
            self.admin.command('ping')
            self.db = self.client[self.database]
            self.col = self.db[self.collection]
            print("Conexión a MongoDB Atlas establecida")

        except Exception as e:
            print(f"No se pudo establecer la conexión a MongoDB Atlas: {e}")


    def insert_data(self, data):
        if not self.client:
            print("Error: No se ha establecido una conexión a MongoDB Atlas.")
            return 0
        
        try:
            data_formated = loads(data)
            result = self.collection.insert_one(data_formated)
            return result
        
        except Exception as e:
            print(f"Error al insertar datos en MongoDB Atlas: {e}")


    def disconnect(self):
        if self.client:
            self.client.close()
        print("Conexión a MongoDB Atlas cerrada.")