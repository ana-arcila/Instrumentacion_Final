// Este ESP32 toma medidas y envia datos por wifi

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP085_U.h>
#include <MPU6050.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>


// ----------------------------------------- WiFi -----------------------------------------
    const char* ssid = "MOVISTAR";
    const char* password = "97616504";

    // URL y cabeceras de la solicitud
    // const char* serverName = "https://data.mongodb-api.com/app/data-YOUR_ID/endpoint/data/v1/action/insertOne";

    // Fernando
    const char* serverName = "https://us-east-1.aws.data.mongodb-api.com/app/data-rgxyxuf/endpoint/data/v1/action/insertOne";
    const char* apiKey = "TWAL6hIQBruvlFs6ZeAvIx9AXtafp1teeGqM1crP7gjeoagx3l1Aka3YzHa0c0KM";

    // Ana
    // const char* serverName = "https://us-east-1.aws.data.mongodb-api.com/app/data-dmxflhz/endpoint/data/v1/action/insertOne";
    // const char* apiKey = "f3L2c6jSnM13a72FvP6DyLTQeWM1ie4qJFALUWj8tZggzKSFDjVPDXKx6JhoGeck";


// ----------------------------------------- Servo -----------------------------------------
    Servo myServo;

    // Pin donde se conectará el servomotor
    const int servoPin = 14;

    // ----------------------------------------- Sensor Distancia -----------------------------------------
// Sensor de distancia
    const int trigPin = 33;  
    const int echoPin = 32; 

    float duration, distance;  


// ----------------------------------------- GY-88 -----------------------------------------
    Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
    Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

    MPU6050 mpu;
    
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    long tiempo_prev, dt;
    float girosc_ang_x, girosc_ang_y, girosc_ang_z, Angulo;
    float girosc_ang_x_prev, girosc_ang_y_prev, girosc_ang_z_prev;

// ----------------------------------------- Comunicacion con el otro ESP32 -----------------------------------------
    HardwareSerial MySerial(2);

    int Enviar = 0;
    int Enviados = 0;

void setup() {

    Serial.begin(115200);
    MySerial.begin(115200, SERIAL_8N1, 16, 17); // 16 (RX) y 17 (TX)


// Conexión a la red WiFi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }


    Serial.println("Connected to the WiFi network");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

// Servo
  	// Asignar el pin al objeto Servo
	myServo.attach(servoPin);
	pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);

// Magnetometro
    Wire.begin();
    mpu.initialize();

    if (mpu.testConnection()) {
        Serial.println("MPU6050 conectado correctamente");
    } else {
        Serial.println("Fallo en la conexión con el MPU6050");
    }


    if (!mag.begin()) {
        Serial.println("No se pudo encontrar un sensor HMC5883L, verifique la conexión.");
        while (1);
    }
    else
    {
        Serial.println("Sensor HMC5883L iniciado correctamente");
    }

    if(!bmp.begin()) {
    Serial.print("No se puede encontrar el sensor BMP085/BMP180, verifique las conexiones.");
    while(1);
    }
    else
    {
        Serial.println("Sensor BMP085/BMP180 iniciado correctamente");
    }

    tiempo_prev=millis();
}

String Medidas;
float mx, my, mz;
float temperatura,presion,altura;

void loop() {
    
    // Acelearación y giroscopio
    mpu.getAcceleration(&ax, &ay, &az);
    mpu.getRotation(&gx, &gy, &gz);

    dt = millis()-tiempo_prev;
    tiempo_prev=millis();
    girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;
    girosc_ang_z_prev=girosc_ang_z;



    if (MySerial.available()) 
    {
        char data = MySerial.read();

        if (data == 'T')
        {       
            // Distancias
            Serial.println("---------------------------------------------------------------------------------------------------------------------------------------");
            Serial.println("Tomando medidas");
            Medidas = barrido();
            

            // Magnetometro
            sensors_event_t mag_event;
            mag.getEvent(&mag_event);

            mx = mag_event.magnetic.x;
            my = mag_event.magnetic.y;
            mz = mag_event.magnetic.z;

            // BMP
            sensors_event_t bmp_event;
            bmp.getEvent(&bmp_event);

            bmp.getTemperature(&temperatura);
            presion = bmp_event.pressure;
            altura = bmp.pressureToAltitude(SENSORS_PRESSURE_SEALEVELHPA, presion);


            Angulo = girosc_ang_z;
            enviarDatos(0.0,0.0,mx,my,mz,temperatura,presion,altura,Angulo,Medidas);
        }
    }
    

    delay(50);


}




String barrido()
{   
    Enviar += 1;

  	myServo.write(0);
    delay(400);
    String Distancias = "[";
	for (int i=0; i<=181; i=i+5)
  	{
		myServo.write(i);
        if (i<180)
        {
            Distancias += String(distancia());
            Distancias += ",";
        }
        else
        {
            Distancias += String(distancia());
            Distancias += "]";
        }
		delay(100);
  	}  
  	myServo.write(0);
  	delay(500);
    return Distancias;

}


float distancia()
{
	digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  
	duration = pulseIn(echoPin, HIGH);
	distance = (duration*.0343)/2;  
	return distance;
}


// void enviarDatos(float x, float y, float Angulo, String Medidas)
void enviarDatos(float x, float y, 
                 float mx, float my, float mz,
                 float temperatura, float presion, float altura,
                 float Angulo, String Medidas)
{   // Enviar datos de la aceleración, giroscopio y magnetometro
    if (Enviar > Enviados)
    {
        Serial.println("Enviando Datos");
        Enviados += 1;
        if (WiFi.status() == WL_CONNECTED) 
        {
            HTTPClient http;

            http.begin(serverName); // Especifica la URL del servidor
            http.addHeader("Content-Type", "application/json");
            http.addHeader("api-key", apiKey);

            // Datos en formato JSON
            // String jsonData = "{\"dataSource\":\"Cluster0\",\"database\":\"IOT_Carrito\",\"collection\":\"Datos\",\"document\":{\"t\":\"{}\",\"x\":\"{}\",\"y\":{}}}".format(t,x,y);
            //  ---------------------------------------------   Modificar   --------------------------------------------------------------------------------------------------------------------------------------------------------
            // String jsonData = "{\"dataSource\":\"Cluster0\",\"database\":\"IOT_Carrito\",\"collection\":\"Datos\",\"document\":{\"x\":\"" + String(x) + "\",\"y\":\"" + String(y) + "\",\"Angulo\":\"" + String(Angulo) + "\",\"Medidas\":\"" + Medidas + "\"}}";
            String jsonData = "{\"dataSource\":\"Cluster0\",\"database\":\"IOT_Carrito\",\"collection\":\"Datos\",\"document\":{\"x\":\"" + String(x) + "\",\"y\":\"" + String(y) + "\",\"mx\":\"" + String(mx) + "\",\"my\":\"" + String(my) + "\",\"mz\":\"" + String(mz) + "\",\"temperatura\":\"" + String(temperatura) + "\",\"presion\":\"" + String(presion) + "\",\"altura\":\"" + String(altura) + "\",\"Angulo\":\"" + String(Angulo) + "\",\"Medidas\":\"" + Medidas + "\"}}";
            //  ---------------------------------------------   Modificar   --------------------------------------------------------------------------------------------------------------------------------------------------------


            // Envío de la solicitud POST
            int httpResponseCode = http.POST(jsonData);

            // Manejo de la respuesta del servidor
            if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode); // Código de respuesta
            Serial.println(response); // Respuesta del servidor
            } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
            }
            
            http.end(); // Cierra la conexión
        } 

        else 
        {
            Serial.println("WiFi Disconnected");
        }
    }
}


