// Este ESP32 toma medidas y envia datos por wifi

#include <WiFi.h>
#include <HTTPClient.h>
// #include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <MPU6050.h>

#include <HardwareSerial.h>

#include <ESP32Servo.h>


// ----------------------------------------- Bluetooth -----------------------------------------
    //#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
    // const char *pin = "1234"; // Change this to more secure PIN.

    // String device_name = "ESP32";

    // #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
    // #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
    // #endif

    // #if !defined(CONFIG_BT_SPP_ENABLED)
    // #error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
    // #endif

    // BluetoothSerial SerialBT;

// ----------------------------------------- WiFi -----------------------------------------
    const char* ssid = "MOVISTAR";
    const char* password = "97616504";

    // URL y cabeceras de la solicitud
    // const char* serverName = "https://data.mongodb-api.com/app/data-YOUR_ID/endpoint/data/v1/action/insertOne";
    const char* serverName = "https://us-east-1.aws.data.mongodb-api.com/app/data-rgxyxuf/endpoint/data/v1/action/insertOne";
    const char* apiKey = "TWAL6hIQBruvlFs6ZeAvIx9AXtafp1teeGqM1crP7gjeoagx3l1Aka3YzHa0c0KM";


// ----------------------------------------- Servo -----------------------------------------
    Servo myServo;

    // Pin donde se conectará el servomotor
    const int servoPin = 14;

    // ----------------------------------------- Sensor Distancia -----------------------------------------
// Sensor de distancia
    const int trigPin = 33;  
    const int echoPin = 32; 

    float duration, distance;  

// ----------------------------------------- Motores -----------------------------------------
    // Motor A
    int IN1 = 4;
    int IN2 = 18;

    // Motor B
    //int ENB = 5;
    int IN3 = 19;
    int IN4 = 23;

    #define LED 2

// ----------------------------------------- Magnetometro -----------------------------------------
    MPU6050 mpu;
    Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

    int16_t ax, ay, az;
    int16_t gx, gy, gz;

// ----------------------------------------- Comunicacion con el otro ESP32 -----------------------------------------
    HardwareSerial MySerial(2);

void setup() {

    Serial.begin(115200);
    MySerial.begin(115200, SERIAL_8N1, 16, 17); // 16 (RX) y 17 (TX)

// Bluetooth
    // SerialBT.begin(device_name); //Bluetooth device name
    // Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
    // //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
    // #ifdef USE_PIN
    //     SerialBT.setPin(pin);
    //     Serial.println("Using PIN");
    // #endif

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

  	// Inicializar el Serial para la depuración

// Motores      Los motores en el otro ESP32
	// pinMode(LED, OUTPUT);

    // //pinMode(LED_PIN, OUTPUT);
    // pinMode (IN1, OUTPUT); //Configuramos los pines del L298N como salida
    // pinMode (IN2, OUTPUT);
    // pinMode (IN3, OUTPUT);
    // pinMode (IN4, OUTPUT);

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

}

void loop() {
    
    // Acelearación y giroscopio
    mpu.getAcceleration(&ax, &ay, &az);
    mpu.getRotation(&gx, &gy, &gz);

    // Magnetometro
    sensors_event_t event;
    mag.getEvent(&event);

    float mx = event.magnetic.x;
    float my = event.magnetic.y;
    float mz = event.magnetic.z;

    enviarDatosPosicion(ax, ay, az, gx, gy, gz, mx, my, mz);

    if (MySerial.available()) {
        char data = MySerial.read();

        if (data == 'T')
        {
            Serial.println("Tomando medidas");
            barrido();
        }
    }

    delay(50);


}




void barrido()
{
	for (int i=0; i<=181; i=i+5)
  	{
		myServo.write(i);
		Serial.print("Angulo: ");
		Serial.print(i);
		Serial.print("  -  Distancia: ");
		Serial.println(distancia());
		// distancia();
		delay(100);
  	}  
  	myServo.write(0);
  	delay(500);

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
	// Serial.print("Distance: ");  
	// Serial.println(distance); 
	return distance;
}

void enviarDatosPosicion(float ax, float ay, float az, 
                         float gx, float gy, float gz, 
                         float mx, float my, float mz)
{   // Enviar datos de la aceleración, giroscopio y magnetometro
    if (WiFi.status() == WL_CONNECTED) 
    {
        HTTPClient http;

        http.begin(serverName); // Especifica la URL del servidor
        http.addHeader("Content-Type", "application/json");
        http.addHeader("api-key", apiKey);

        // Datos en formato JSON
        // String jsonData = "{\"dataSource\":\"Cluster0\",\"database\":\"IOT_Carrito\",\"collection\":\"Datos\",\"document\":{\"t\":\"{}\",\"x\":\"{}\",\"y\":{}}}".format(t,x,y);
        //  ---------------------------------------------   Modificar   --------------------------------------------------------------------------------------------------------------------------------------------------------
        String jsonData = "{\"dataSource\":\"Cluster0\",\"database\":\"IOT_Carrito\",\"collection\":\"Datos\",\"document\":{\"t\":\"" + String(ax) + "\",\"x\":\"" + String(ay) + "\",\"y\":\"" + String(az) + "\"}}";
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

void enviarDatosDistancia(String Cadena)
{   // Enviar datos de la distancia
    if (WiFi.status() == WL_CONNECTED) 
    {
        HTTPClient http;

        http.begin(serverName); // Especifica la URL del servidor
        http.addHeader("Content-Type", "application/json");
        http.addHeader("api-key", apiKey);

        // Datos en formato JSON
        // String jsonData = "{\"dataSource\":\"Cluster0\",\"database\":\"IOT_Carrito\",\"collection\":\"Datos\",\"document\":{\"t\":\"{}\",\"x\":\"{}\",\"y\":{}}}".format(t,x,y);
        String jsonData = "{\"dataSource\":\"Cluster0\",\"database\":\"IOT_Carrito\",\"collection\":\"Datos\",\"document\":{\"Medidas\":\"" + Cadena +  "\"}}";

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