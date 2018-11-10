/*********************************************
 *  Firmware de prueva de driver I2C         *
 *  Version: 0.4 a                           *
 *  Fecha: Noviembre del 2018                *
 *  Desarrollador: Miguel Orozco             *
 *  Direccion del repositorio:               *
 *  https://github.com/Necrovalle/Driver_I2C *
 *********************************************/
//---------------- LIBRERIAS ----------------
#include <Wire.h>

//----------------- VARIABLES ----------------
byte salidaI2C;     //Caracter de salida por I2C
byte entradaI2C;    //Caracter de entrada de i2C
char entSerial;     //Caracter de entrada por puerto serie

//--------------- CONFIGURACION --------------
void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Inicalizado");
}

//------------------- MAIN ------------------
void loop() 
{
  if (Serial.available()>0)
  {
    entSerial = Serial.read();

    switch (entSerial)
    {
      case 'A':
        salidaI2C = 0xA0;
        break;

      case 'B':
        salidaI2C = 0xA1;
        break;

      case 'C':
        salidaI2C = 0xA2;
        break;

      case 'D':
        salidaI2C = 0xA3;
        break;

      case 'E':
        salidaI2C = 0xD0;
        break;

      case 'F':
        salidaI2C = 0xD1;
        break;

      case 'G':
        salidaI2C = 0xE1;
        break;

      case 'H':
        salidaI2C = 0xF1;
        break;

      case 'g':
        salidaI2C = 0xE0;
        break;

      case 'h':
        salidaI2C = 0xF0;
        break;

      default:
        Serial.println("Comando no encontrado");
        salidaI2C = 0x00;
        break;
    }
    Serial.println(salidaI2C);
    Envio_I2C(salidaI2C);
    delay(10);
    Recepcion_I2C();
  }
}

//---------------- FUNCIONES ---------------
void Envio_I2C(byte codigoEnvio)
{
  /*************************************
   *   Envio de dato espesificado al   *
   *                esclavo            *
   * @param:  codigoEnvio dat a enviar *
   * @return: Sin retornos             *
   *************************************/
   Wire.beginTransmission(0x01); 
   delay(50);
   Wire.write(salidaI2C);
   Wire.endTransmission();
   delay(50);
   Serial.print("Enviado ");
   Serial.println(entSerial);
}

void Recepcion_I2C()
{
  /*************************************
   *   Recepcion de dato espesificado al   *
   *                esclavo            *
   * @param:  ninguno *
   * @return: Sin retornos             *
   *************************************/
   byte entI2C;   //Caracter leido
   Wire.requestFrom(0x01, 1);
   delay(150);
   Serial.println("Recibiendo...");
   while (Wire.available())
   {
      entI2C = Wire.read();
      Serial.println(entI2C);
   }
   Wire.requestFrom(0x01, 1);
   delay(5);
   Wire.requestFrom(0x01, 1);
   delay(5);
   while (Wire.available())
   {
      entI2C = Wire.read();
      Serial.println(entI2C);
   }
}













