// Este ESP32 Mueve el carrito

#include "BluetoothSerial.h"
#include <HardwareSerial.h>



// ----------------------------------------- Bluetooth -----------------------------------------
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

// ----------------------------------------- Motores -----------------------------------------

#define LED 2
// Motor A
//int ENA = 10;
int IN1 = 23;
int IN2 = 22;

// Motor B
//int ENB = 5;
int IN3 = 19;
int IN4 = 18;


// ----------------------------------------- Comunicacion con el otro ESP32 -----------------------------------------
    HardwareSerial MySerial(2);


void setup() {
    Serial.begin(115200);
    MySerial.begin(115200, SERIAL_8N1, 16, 17); // 16 (RX) y 17 (TX)

// Bluetooth
    SerialBT.begin(device_name); //Bluetooth device name
    Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
    //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
    #ifdef USE_PIN
        SerialBT.setPin(pin);
        Serial.println("Using PIN");
    #endif

// Motores
    pinMode(LED, OUTPUT);

    //pinMode(LED_PIN, OUTPUT);
    pinMode (IN1, OUTPUT); //Configuramos los pines del L298N como salida
    pinMode (IN2, OUTPUT);
    pinMode (IN3, OUTPUT);
    pinMode (IN4, OUTPUT);
    
}

void loop() 
{
    if (SerialBT.available()) 
    {   
        Serial.write(SerialBT.read());
        char data = SerialBT.read();

        if (data == 'T')
        // Hacer barrido del sensor de distancia
		{
			Serial.println("Tomando medidas");
			MySerial.println("T");
            delay(2000); // Delay para que el servo vuelva al inicio
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