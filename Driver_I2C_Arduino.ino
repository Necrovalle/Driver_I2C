/*********************************************
 *          Firmware de driver I2C           *
 *  Version: 0.8 a                           *
 *  Fecha: Noviembre del 2018                *
 *  Desarrollador: Miguel Orozco             *
 *  Direccion del repositorio:               *
 *  https://github.com/Necrovalle/Driver_I2C *
 *********************************************/

 //---------------- LIBRERIAS ----------------
#include <Wire.h>

//----------------- VARIABLES ----------------
byte dirI2C = 0x01;       //Direccion de este driver como esclavo
float verFirmware = 0.8;  //Version del firmware para consulta
char msgSerial;           //Para lectura de la entrada del puerto serie
int numChars;             //Cantidad de caracteres a mandar por I2C
byte salidaI2C[2];        //Arreglo de los datos que se enviaran por I2C
int contSalida;       //Contador de bytes de salida por I2C

//--------------- CONFIGURACION --------------
void setup() 
{
  //Activacion de servicios de comunicacion
  Wire.begin(dirI2C);
  Wire.onRequest(Peticion_Maestro);
  Wire.onReceive(Escritura_Maestro);
  Serial.begin(9600);

  //Definicion de conexiones digitales
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

}

//------------------- MAIN ------------------
void loop() 
{
  //Atencion a la peticion de lectura/escritura de parametros 
  if (Serial.available() > 0)
  {
    msgSerial = Serial.read();
    switch (msgSerial)
    {
      case 'A':
        Serial.println("-- I2C driver configuration --");
        Serial.print("Address: 0x");
        Serial.println(dirI2C, HEX);
        Serial.print("Firmware version: ");
        Serial.println(verFirmware);
        break;

       default:
         Serial.println("* ERROR: command not found *");
         break;
    }
    msgSerial = 'x';
  }
}

//---------------- FUNCIONES ---------------
void Peticion_Maestro()
{
  /*****************************************
   *  Envio de datos a peticion del maetro *
   *   con previa configuracion de valores *
   * @param: Sin parametros                *
   * @return: Sin retornos                 *
   *****************************************/
      Wire.write(salidaI2C[contSalida]);
      contSalida=1;
}


void Escritura_Maestro(int numBytes)
{
  /*************************************************
   *      Lectura de la petición del maestro       *
   *        y ejecucion de tareas indicadas.       *
   *        La codificacion de las tareas en       * 
   *           el manual del repositorio.          *
   * @param: numBytes es el numero de bytes a leer *
   * @return: Sin retornos                         *
   *************************************************/
  byte entI2C;    //Dato recibido del maestro

  while (Wire.available()>0)
  {
    entI2C = Wire.read();
  }
  contSalida = 0;

  switch (entI2C)
  {
    case 0xA0:
      Lectura_Analoga(0);
      break;

    case 0xA1:
      Lectura_Analoga(1);
      break;

    case 0xA2:
      Lectura_Analoga(2);
      break;

    case 0xA3:
      Lectura_Analoga(3);
      break;

    case 0xD0:
      Lectura_Digital(10);
      break;

    case 0xD1:
      Lectura_Digital(11);
      break;

    case 0xE1:
      Escritura_Digital(12, HIGH);
      break;

    case 0xF1:
      Escritura_Digital(13, HIGH);
      break;

    case 0xE0:
      Escritura_Digital(12, LOW);
      break;

    case 0xF0:
      Escritura_Digital(13, LOW);
      break;

    default:
      salidaI2C[0] = 0x00;
      salidaI2C[1] = 0x00;
      break;
  }
}


void Lectura_Analoga(int canalAnalogo)
{
  /*****************************************************
   *  Adquiere el valor del canal analogo seleccionado *
   *   y prepara este valor para ser mandado por I2C   *
   * @param: canalAnalogo es el identificador de pin   *
   * @return: Sin retorno                              *
   *****************************************************/
  int voltajeMedido;    //Alojamiento del valo del ADC espesificado
  int masSigni;         //byte mas significativo
  int menSigni;         //byte menos significativo

  voltajeMedido = analogRead(canalAnalogo);
  //Cambio del valor de 10 bits a 2 byts
  masSigni = (byte)(voltajeMedido/256);
  menSigni = voltajeMedido - (masSigni * 256);
  salidaI2C[0] = masSigni;
  salidaI2C[1] = menSigni;
}

void Lectura_Digital(int pinLectura)
{
  /******************************************************
   *  Adquire el valor dela entrada digital configurada *
   *       y lo prepara para ser enviada por I2C        *
   * @param: pinLectura es el numero de pin a ser leido *
   * @return: Sin retorno                               *
   ******************************************************/
   boolean estadoDigital;   //Almacenamiento del estado digital leido
   
   estadoDigital = digitalRead(pinLectura);
   if (estadoDigital)
   {
      salidaI2C[0] = 68;
      salidaI2C[1] = 1;
   }
   else
   {
      salidaI2C[0] = 68;
      salidaI2C[1] = 0;
   }
}

void Escritura_Digital(int pinEscritura, boolean estadoSalida)
{
  /*******************************************************
   *    Da salida al valor de estado logico espesificado *
   *              en el canal espesificado               *
   *  @param: pinEscritura es el pin de escritura        *
   *          estadoSalida es el nivel logico a escribir *
   *  @return: Sin retorno                               *
   *******************************************************/
   digitalWrite(pinEscritura, estadoSalida);
   salidaI2C[0] = 65;
   salidaI2C[1] = 75;
}

