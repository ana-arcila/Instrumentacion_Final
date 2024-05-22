#include <ESP32Servo.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>


// Bluetooth
    //#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
    const char *pin = "1234"; // Change this to more secure PIN.

    String device_name = "ESP32";

    #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
    #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
    #endif

    #if !defined(CONFIG_BT_SPP_ENABLED)
    #error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
    #endif

    BluetoothSerial SerialBT;

// Crear un objeto Servo
    Servo myServo;

    // Pin donde se conectará el servomotor
    const int servoPin = 14;

// Sensor de distancia
    const int trigPin = 33;  
    const int echoPin = 32; 

    float duration, distance;   

// Motores
    // Motor A
    int IN1 = 4;
    int IN2 = 18;

    // Motor B
    //int ENB = 5;
    int IN3 = 19;
    int IN4 = 23;

    #define LED 2

// Magnetometro
    float mag_x, mag_y, mag_z;

    Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


void setup() {
  	Serial.begin(115200);

// Bluetooth
	SerialBT.begin(device_name); //Bluetooth device name
    Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
    //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
    #ifdef USE_PIN
        SerialBT.setPin(pin);
        Serial.println("Using PIN");
    #endif

// Servo
  	// Asignar el pin al objeto Servo
	myServo.attach(servoPin);
	pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);

  	// Inicializar el Serial para la depuración

// Motores
	pinMode(LED, OUTPUT);

    //pinMode(LED_PIN, OUTPUT);
    pinMode (IN1, OUTPUT); //Configuramos los pines del L298N como salida
    pinMode (IN2, OUTPUT);
    pinMode (IN3, OUTPUT);
    pinMode (IN4, OUTPUT);

   // Magnetometro
    // sensors_event_t event; 
    // mag.getEvent(&event);
}

void loop() {
  
	// for (int i=0; i<=181; i=i+5)
  	// {
	// 	myServo.write(i);
	// 	distancia();
    // 	delay(100);
  	// }  
  	// myServo.write(0);
  	// delay(500);
	
	if (SerialBT.available()) 
    {   
        Serial.write(SerialBT.read());
        char data = SerialBT.read();

		if (data == 'D')
        // Hacer barrido del sensor de distancia
		{
			Serial.println("Tomando medidas");
			barrido();
		}

        else if (data == 'M')
        // Tomar medidas del magnetometro
        {
            Serial.println("Magnetometro");
            sensors_event_t event; 
            mag.getEvent(&event);
            mag_x = event.magnetic.x;
            mag_y = event.magnetic.y;
            mag_z = event.magnetic.z;
            Serial.print("X: ");Serial.print(mag_x);
            Serial.print(",");
            Serial.print(" Y: ");Serial.print(mag_y);
            Serial.print(",");
            Serial.print(" Z: ");Serial.println(mag_z);
        }


        else if (data == 'F') 
        {
            digitalWrite(LED, HIGH);
            Serial.println("Adelante");
            Adelante();
        }
        else if (data == 'B')
        {
            digitalWrite(LED, HIGH);
            Serial.println("Atras");
            Atras();
        }
        else if (data == 'R')
        {
            Serial.println("Derecha");
            digitalWrite(LED, HIGH);
            Derecha();
        }
        else if (data == 'L')
        {
            Serial.println("Izquierda");
            digitalWrite(LED, HIGH);
            Izquierda();
        }
        else if (data == 'f' or data == 'b' or data == 'r' or data == 'l')
        {
            Serial.println("Parar");
            digitalWrite(LED, LOW);
            Parar();
        }
        
    }

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




// Mover Carrito
void Adelante ()
{
    //Direccion motor A
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    //analogWrite (ENA, 255); //Velocidad motor A
    //Direccion motor B
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    //analogWrite (ENB, 255); //Velocidad motor B
}

void Atras ()
{
    //Direccion motor A
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    //analogWrite (ENA, 128); //Velocidad motor A
    //Direccion motor B
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    //analogWrite (ENB, 128); //Velocidad motor B
}

void Derecha ()
{
    //Direccion motor A
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    //analogWrite (ENA, 200); //Velocidad motor A
    //Direccion motor B
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    //analogWrite (ENB, 100); //Velocidad motor A
}

void Izquierda ()
{
    //Direccion motor A
    digitalWrite (IN1, LOW);
    digitalWrite (IN2, HIGH);
    //analogWrite (ENA, 50); //Velocidad motor A
    //Direccion motor B
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    //analogWrite (ENB, 150); //Velocidad motor A
}

void Parar ()
{
 //Direccion motor A
 digitalWrite (IN1, LOW);
 digitalWrite (IN2, LOW);
//  analogWrite (ENA, 0); //Velocidad motor A
 //Direccion motor B
 digitalWrite (IN3, LOW);
 digitalWrite (IN4, LOW);
//  analogWrite (ENB, 0); //Velocidad motor A
}