from pymongo import MongoClient
from pymongo.server_api import ServerApi
from dotenv import load_dotenv
import os

load_dotenv()
uri = os.getenv("MONGO_URI")

if not uri:
    raise ValueError("MONGO_URI environment variable not set")

# Create a new client and connect to the server
client = MongoClient(uri, server_api=ServerApi('1'))
db = client["IOT-database"]
collection = db["pruebas"]


